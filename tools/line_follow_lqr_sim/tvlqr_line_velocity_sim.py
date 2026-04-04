#!/usr/bin/env python3
import argparse
import math
import re
from dataclasses import dataclass
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


@dataclass
class DriveModel:
    trackwidth_in: float
    max_voltage: float
    kv_linear: float
    ka_linear: float
    kv_angular: float
    ka_angular: float

    @property
    def chassis_A(self) -> np.ndarray:
        return np.array(
            [
                [-self.kv_linear / self.ka_linear, 0.0],
                [0.0, -self.kv_angular / self.ka_angular],
            ],
            dtype=float,
        )

    @property
    def chassis_B(self) -> np.ndarray:
        return np.array(
            [
                [0.5 / self.ka_linear, 0.5 / self.ka_linear],
                [-0.5 / self.ka_angular, 0.5 / self.ka_angular],
            ],
            dtype=float,
        )

    @property
    def max_linear_speed_inps(self) -> float:
        u = np.array([self.max_voltage, self.max_voltage], dtype=float)
        x_ss = np.linalg.solve(-self.chassis_A, self.chassis_B @ u)
        return abs(float(x_ss[0]))


def parse_drive_model(robot_config_path: Path) -> DriveModel:
    text = robot_config_path.read_text()
    match = re.search(
        r"TankDriveModel\s+drive_model\s*\(\s*"
        r"Length::from<inch_tag>\(([^)]+)\)\s*,\s*"
        r"([0-9eE+\-.]+)_V\s*,\s*"
        r"([0-9eE+\-.]+)_VpInPs\s*,\s*"
        r"([0-9eE+\-.]+)_VpInPs2\s*,\s*"
        r"([0-9eE+\-.]+)_VpRadPs\s*,\s*"
        r"([0-9eE+\-.]+)_VpRadPs2\s*"
        r"\)",
        text,
        re.MULTILINE,
    )
    if not match:
        raise RuntimeError(f"Could not parse TankDriveModel from {robot_config_path}")

    values = [float(match.group(i)) for i in range(1, 7)]
    return DriveModel(
        trackwidth_in=values[0],
        max_voltage=values[1],
        kv_linear=values[2],
        ka_linear=values[3],
        kv_angular=values[4],
        ka_angular=values[5],
    )


def wrap_to_pi(angle: float) -> float:
    return math.atan2(math.sin(angle), math.cos(angle))


def dare_sda(A: np.ndarray, B: np.ndarray, Q: np.ndarray, R: np.ndarray) -> np.ndarray:
    A_k = A.copy()
    G_k = B @ np.linalg.solve(R, B.T)
    H_k1 = Q.copy()

    for _ in range(10000):
        H_k = H_k1
        W = np.eye(H_k.shape[0], dtype=float) + G_k @ H_k
        W_inv = np.linalg.inv(W)

        V1 = W_inv @ A_k
        V2 = (W_inv @ G_k.T).T

        G_k = G_k + A_k @ V2 @ A_k.T
        H_k1 = H_k + V1.T @ H_k @ A_k
        A_k = A_k @ V1

        if np.linalg.norm(H_k1 - H_k) <= 1e-10 * max(1.0, np.linalg.norm(H_k1)):
            return H_k1

    raise RuntimeError("DARE SDA failed to converge")


class LineVelocityTVLQR:
    def __init__(
        self,
        model: DriveModel,
        dt: float,
        q_parallel: float,
        q_cte: float,
        q_theta: float,
        q_left_vel: float,
        q_right_vel: float,
        r_left: float,
        r_right: float,
        velocity_step: float,
        min_velocity: float,
    ) -> None:
        self.model = model
        self.dt = dt
        self.min_velocity = max(min_velocity, 1e-3)
        self.Q = np.diag(
            [
                1.0 / (q_parallel * q_parallel),
                1.0 / (q_cte * q_cte),
                1.0 / (q_theta * q_theta),
                1.0 / (q_left_vel * q_left_vel),
                1.0 / (q_right_vel * q_right_vel),
            ]
        )
        self.R = np.diag([1.0 / (r_left * r_left), 1.0 / (r_right * r_right)])
        self.schedule = {}

        max_v = model.max_linear_speed_inps
        v = -max_v
        while v <= max_v + 1e-9:
            linearized_v = v
            if abs(linearized_v) < self.min_velocity:
                linearized_v = -self.min_velocity if linearized_v < 0.0 else self.min_velocity

            A, B = self._linearized_dynamics(linearized_v)
            Ad = np.eye(5, dtype=float) + A * dt
            Bd = B * dt
            P = dare_sda(Ad, Bd, self.Q, self.R)
            K = np.linalg.solve(Bd.T @ P @ Bd + self.R, Bd.T @ P @ Ad)
            self.schedule[round(v, 6)] = K
            v += velocity_step

        self.sorted_schedule = np.array(sorted(self.schedule.keys()), dtype=float)

    def _linearized_dynamics(self, v_ref_inps: float) -> tuple[np.ndarray, np.ndarray]:
        A = np.zeros((5, 5), dtype=float)
        B = np.zeros((5, 2), dtype=float)

        # e_parallel_dot ~= average wheel-velocity error along the line
        A[0, 3] = 0.5
        A[0, 4] = 0.5
        # e_cte_dot ~= v_ref * e_theta
        A[1, 2] = v_ref_inps
        # e_theta_dot = omega_err = (v_r - v_l) / trackwidth
        A[2, 3] = -1.0 / self.model.trackwidth_in
        A[2, 4] = 1.0 / self.model.trackwidth_in

        wheel_A = self.model.wheel_A
        wheel_B = self.model.wheel_B
        A[3:, 3:] = wheel_A
        B[3:, :] = wheel_B
        return A, B

    def gains(self, v_ref_inps: float) -> np.ndarray:
        target = v_ref_inps
        if abs(target) < self.min_velocity:
            target = -self.min_velocity if target < 0.0 else self.min_velocity
        idx = int(np.argmin(np.abs(self.sorted_schedule - target)))
        return self.schedule[float(self.sorted_schedule[idx])]

    def control(
        self,
        x: float,
        y: float,
        theta: float,
        v_left: float,
        v_right: float,
        line_point: np.ndarray,
        line_heading: float,
        s_ref: float,
        v_ref_inps: float,
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        l_hat = np.array([math.cos(line_heading), math.sin(line_heading)], dtype=float)
        n_hat = np.array([-math.sin(line_heading), math.cos(line_heading)], dtype=float)

        position_error = np.array([x - line_point[0], y - line_point[1]], dtype=float) - s_ref * l_hat
        e_parallel = float(position_error @ l_hat)
        e_cte = float(position_error @ n_hat)
        e_theta = wrap_to_pi(theta - line_heading)

        wheel_ref = self.chassis_to_wheels(v_ref_inps, 0.0)
        error = np.array(
            [
                e_parallel,
                e_cte,
                e_theta,
                wheel_ref[0] - v_left,
                wheel_ref[1] - v_right,
            ],
            dtype=float,
        )

        K = self.gains(v_ref_inps)
        nominal_voltage = self.model.kv_linear * v_ref_inps
        u_ff = np.array([nominal_voltage, nominal_voltage], dtype=float)
        u_fb = -(K @ error)
        voltages = np.clip(u_ff + u_fb, -self.model.max_voltage, self.model.max_voltage)
        return voltages, error, wheel_ref

    def chassis_to_wheels(self, v_linear: float, omega: float) -> np.ndarray:
        differential = omega * self.model.trackwidth_in * 0.5
        return np.array([v_linear - differential, v_linear + differential], dtype=float)


@property
def _wheel_A(self) -> np.ndarray:
    M = np.array([[0.5, 0.5], [-1.0 / self.trackwidth_in, 1.0 / self.trackwidth_in]], dtype=float)
    M_inv = np.array([[1.0, -self.trackwidth_in * 0.5], [1.0, self.trackwidth_in * 0.5]], dtype=float)
    return M_inv @ self.chassis_A @ M


@property
def _wheel_B(self) -> np.ndarray:
    M_inv = np.array([[1.0, -self.trackwidth_in * 0.5], [1.0, self.trackwidth_in * 0.5]], dtype=float)
    return M_inv @ self.chassis_B


DriveModel.wheel_A = _wheel_A
DriveModel.wheel_B = _wheel_B


def forward_input_to_line_velocity(model: DriveModel, forward_back: float) -> float:
    forward_back = max(-1.0, min(1.0, forward_back))
    return forward_back * model.max_linear_speed_inps


def simulate(args: argparse.Namespace) -> None:
    model = parse_drive_model(Path(args.robot_config))
    controller = LineVelocityTVLQR(
        model=model,
        dt=args.dt,
        q_parallel=args.q_parallel,
        q_cte=args.q_cte,
        q_theta=args.q_theta,
        q_left_vel=args.q_left_vel,
        q_right_vel=args.q_right_vel,
        r_left=args.r_left,
        r_right=args.r_right,
        velocity_step=args.velocity_step,
        min_velocity=args.min_velocity,
    )

    x = args.x0
    y = args.y0
    theta = math.radians(args.heading_deg0)
    v_left = 0.0
    v_right = 0.0

    line_point = np.array([args.line_x, args.line_y], dtype=float)
    line_heading = math.radians(args.line_heading_deg + (180.0 if args.reverse else 0.0))
    s_ref = 0.0

    times = np.arange(0.0, args.total_time + 1e-9, args.dt)
    log = {key: [] for key in [
        "t", "x", "y", "heading_deg", "e_parallel", "cte", "etheta_deg", "left_v", "right_v", "v_ref",
        "left_ref", "right_ref", "left_cmd", "right_cmd"
    ]}

    for t in times:
        if args.ramp_time > 0.0:
            forward_input = args.forward_input * min(1.0, t / args.ramp_time)
        else:
            forward_input = args.forward_input
        v_ref = forward_input_to_line_velocity(model, forward_input)
        s_ref += v_ref * args.dt

        voltages, error, wheel_ref = controller.control(x, y, theta, v_left, v_right, line_point, line_heading, s_ref, v_ref)

        wheel_state = np.array([v_left, v_right], dtype=float)
        wheel_dot = model.wheel_A @ wheel_state + model.wheel_B @ voltages
        wheel_state = wheel_state + wheel_dot * args.dt
        v_left = float(wheel_state[0])
        v_right = float(wheel_state[1])

        v_linear = 0.5 * (v_left + v_right)
        omega = (v_right - v_left) / model.trackwidth_in
        x += v_linear * math.cos(theta) * args.dt
        y += v_linear * math.sin(theta) * args.dt
        theta = wrap_to_pi(theta + omega * args.dt)

        log["t"].append(t)
        log["x"].append(x)
        log["y"].append(y)
        log["heading_deg"].append(math.degrees(theta))
        log["e_parallel"].append(error[0])
        log["cte"].append(error[1])
        log["etheta_deg"].append(math.degrees(error[2]))
        log["left_v"].append(v_left)
        log["right_v"].append(v_right)
        log["v_ref"].append(v_ref)
        log["left_ref"].append(wheel_ref[0])
        log["right_ref"].append(wheel_ref[1])
        log["left_cmd"].append(voltages[0])
        log["right_cmd"].append(voltages[1])

    fig, axes = plt.subplots(2, 2, figsize=(12, 9))
    fig.suptitle("Line-Velocity TVLQR Simulation")

    axes[0, 0].plot(log["x"], log["y"], label="state")
    line_extent = args.plot_line_extent
    lx = np.array([-line_extent, line_extent], dtype=float)
    axes[0, 0].plot(
        line_point[0] + lx * math.cos(line_heading),
        line_point[1] + lx * math.sin(line_heading),
        "--",
        label="reference line",
    )
    axes[0, 0].axis("equal")
    axes[0, 0].set_xlabel("x (in)")
    axes[0, 0].set_ylabel("y (in)")
    axes[0, 0].legend()

    axes[0, 1].plot(log["t"], log["cte"], label="cross-track error")
    axes[0, 1].plot(log["t"], log["etheta_deg"], label="heading error (deg)")
    axes[0, 1].plot(log["t"], log["e_parallel"], label="along-line error")
    axes[0, 1].set_xlabel("time (s)")
    axes[0, 1].legend()

    axes[1, 0].plot(log["t"], log["left_v"], label="left velocity")
    axes[1, 0].plot(log["t"], log["right_v"], label="right velocity")
    axes[1, 0].plot(log["t"], log["left_ref"], "--", label="left ref")
    axes[1, 0].plot(log["t"], log["right_ref"], "--", label="right ref")
    axes[1, 0].set_xlabel("time (s)")
    axes[1, 0].legend()

    axes[1, 1].plot(log["t"], log["left_cmd"], label="left voltage")
    axes[1, 1].plot(log["t"], log["right_cmd"], label="right voltage")
    axes[1, 1].set_xlabel("time (s)")
    axes[1, 1].legend()

    fig.tight_layout()
    output_path = Path(args.output).resolve()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(str(output_path), dpi=args.dpi, bbox_inches="tight")
    plt.close(fig)

    print("Drive model from robot-config.cpp")
    print(f"  estimated max linear speed: {model.max_linear_speed_inps:.3f} in/s")
    print(f"  final pose: x={x:.3f} in, y={y:.3f} in, heading={math.degrees(theta):.3f} deg")
    print(f"  final along-line error: {log['e_parallel'][-1]:.3f} in")
    print(f"  final cross-track error: {log['cte'][-1]:.3f} in")
    print(f"  final heading error: {log['etheta_deg'][-1]:.3f} deg")
    print(f"  plot: {output_path}")


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Simulate a line-following TVLQR with user velocity input along the line.")
    parser.add_argument(
        "--robot-config",
        default=str(Path(__file__).resolve().parents[2] / "src" / "robot-config.cpp"),
        help="Path to robot-config.cpp containing TankDriveModel drive_model(...).",
    )
    parser.add_argument("--dt", type=float, default=0.01)
    parser.add_argument("--total-time", type=float, default=3.0)
    parser.add_argument("--forward-input", type=float, default=0.5, help="Desired speed along the line in [-1, 1].")
    parser.add_argument("--ramp-time", type=float, default=0.2)
    parser.add_argument("--line-x", type=float, default=0.0)
    parser.add_argument("--line-y", type=float, default=0.0)
    parser.add_argument("--line-heading-deg", type=float, default=0.0)
    parser.add_argument("--reverse", action="store_true")
    parser.add_argument("--x0", type=float, default=0.0)
    parser.add_argument("--y0", type=float, default=18.0)
    parser.add_argument("--heading-deg0", type=float, default=35.0)
    parser.add_argument("--q-parallel", type=float, default=1000.0)
    parser.add_argument("--q-cte", type=float, default=5.0)
    parser.add_argument("--q-theta", type=float, default=0.2)
    parser.add_argument("--q-left-vel", type=float, default=12.0)
    parser.add_argument("--q-right-vel", type=float, default=12.0)
    parser.add_argument("--r-left", type=float, default=12.0)
    parser.add_argument("--r-right", type=float, default=12.0)
    parser.add_argument("--velocity-step", type=float, default=0.5)
    parser.add_argument("--min-velocity", type=float, default=2.0)
    parser.add_argument("--plot-line-extent", type=float, default=120.0)
    parser.add_argument(
        "--output",
        default=str(Path(__file__).resolve().parent / "tvlqr_line_velocity_sim.png"),
        help="PNG file to write.",
    )
    parser.add_argument("--dpi", type=int, default=160)
    return parser


if __name__ == "__main__":
    simulate(build_arg_parser().parse_args())
