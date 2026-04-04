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
    def wheel_A(self) -> np.ndarray:
        M = np.array([[0.5, 0.5], [-1.0 / self.trackwidth_in, 1.0 / self.trackwidth_in]], dtype=float)
        M_inv = np.array([[1.0, -self.trackwidth_in * 0.5], [1.0, self.trackwidth_in * 0.5]], dtype=float)
        return M_inv @ self.chassis_A @ M

    @property
    def wheel_B(self) -> np.ndarray:
        M_inv = np.array([[1.0, -self.trackwidth_in * 0.5], [1.0, self.trackwidth_in * 0.5]], dtype=float)
        return M_inv @ self.chassis_B

    @property
    def max_linear_speed_inps(self) -> float:
        u = np.array([self.max_voltage, self.max_voltage], dtype=float)
        x_ss = np.linalg.solve(-self.chassis_A, self.chassis_B @ u)
        return abs(float(x_ss[0]))

    def chassis_to_wheels(self, linear_velocity: float, angular_velocity: float) -> np.ndarray:
        differential = angular_velocity * self.trackwidth_in * 0.5
        return np.array([linear_velocity - differential, linear_velocity + differential], dtype=float)


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


class LTVDifferentialDriveController:
    def __init__(
        self,
        model: DriveModel,
        dt: float,
        q_tolerances: np.ndarray,
        r_tolerances: np.ndarray,
        velocity_step: float,
        min_velocity: float,
    ) -> None:
        self.model = model
        self.dt = dt
        self.min_velocity = max(min_velocity, 1e-3)
        self.Q = np.diag(1.0 / np.square(q_tolerances))
        self.R = np.diag(1.0 / np.square(r_tolerances))
        self.schedule = {}

        max_v = model.max_linear_speed_inps
        v = -max_v
        while v <= max_v + 1e-9:
            linearized_v = v
            if abs(linearized_v) < self.min_velocity:
                linearized_v = -self.min_velocity if linearized_v < 0.0 else self.min_velocity

            A, B = self._linearized_error_dynamics(linearized_v)
            Ad = np.eye(5, dtype=float) + A * dt
            Bd = B * dt
            P = dare_sda(Ad, Bd, self.Q, self.R)
            K = np.linalg.solve(Bd.T @ P @ Bd + self.R, Bd.T @ P @ Ad)
            self.schedule[round(v, 6)] = K
            v += velocity_step

        self.sorted_schedule = np.array(sorted(self.schedule.keys()), dtype=float)

    def _linearized_error_dynamics(self, velocity_inps: float) -> tuple[np.ndarray, np.ndarray]:
        A = np.zeros((5, 5), dtype=float)
        B = np.zeros((5, 2), dtype=float)

        A[0, 3] = 0.5
        A[0, 4] = 0.5
        A[1, 2] = velocity_inps
        A[2, 3] = -1.0 / self.model.trackwidth_in
        A[2, 4] = 1.0 / self.model.trackwidth_in
        A[3:, 3:] = self.model.wheel_A
        B[3:, :] = self.model.wheel_B
        return A, B

    def gains(self, linear_velocity_inps: float) -> np.ndarray:
        target = linear_velocity_inps
        if abs(target) < self.min_velocity:
            target = -self.min_velocity if target < 0.0 else self.min_velocity
        idx = int(np.argmin(np.abs(self.sorted_schedule - target)))
        return self.schedule[float(self.sorted_schedule[idx])]

    def control(
        self,
        pose: np.ndarray,
        wheel_state: np.ndarray,
        line_point: np.ndarray,
        line_heading: float,
        wheel_ref: np.ndarray,
        nominal_voltage: np.ndarray,
    ) -> tuple[np.ndarray, np.ndarray]:
        line_dir = np.array([math.cos(line_heading), math.sin(line_heading)], dtype=float)
        rel = pose[:2] - line_point
        projection = line_point + np.dot(rel, line_dir) * line_dir

        pose_ref = np.array([projection[0], projection[1], line_heading], dtype=float)
        global_error = np.array(
            [
                pose_ref[0] - pose[0],
                pose_ref[1] - pose[1],
                wrap_to_pi(pose_ref[2] - pose[2]),
                wheel_ref[0] - wheel_state[0],
                wheel_ref[1] - wheel_state[1],
            ],
            dtype=float,
        )

        c = math.cos(pose[2])
        s = math.sin(pose[2])
        in_robot = np.eye(5, dtype=float)
        in_robot[0, 0] = c
        in_robot[0, 1] = s
        in_robot[1, 0] = -s
        in_robot[1, 1] = c
        robot_error = in_robot @ global_error

        linear_velocity = 0.5 * (wheel_state[0] + wheel_state[1])
        if abs(linear_velocity) < self.min_velocity:
            linear_velocity = 0.5 * (wheel_ref[0] + wheel_ref[1])

        K = self.gains(linear_velocity)
        feedback = K @ robot_error
        voltages = np.clip(nominal_voltage + feedback, -self.model.max_voltage, self.model.max_voltage)
        return voltages, robot_error


def forward_input_to_line_velocity(model: DriveModel, forward_back: float) -> float:
    forward_back = max(-1.0, min(1.0, forward_back))
    return forward_back * model.max_linear_speed_inps


def simulate(args: argparse.Namespace) -> None:
    model = parse_drive_model(Path(args.robot_config))
    controller = LTVDifferentialDriveController(
        model=model,
        dt=args.dt,
        q_tolerances=np.array([args.q_x, args.q_y, args.q_theta, args.q_left_vel, args.q_right_vel], dtype=float),
        r_tolerances=np.array([args.r_left, args.r_right], dtype=float),
        velocity_step=args.velocity_step,
        min_velocity=args.min_velocity,
    )

    pose = np.array([args.x0, args.y0, math.radians(args.heading_deg0)], dtype=float)
    wheel_state = np.array([0.0, 0.0], dtype=float)
    line_point = np.array([args.line_x, args.line_y], dtype=float)
    line_heading = math.radians(args.line_heading_deg + (180.0 if args.reverse else 0.0))
    times = np.arange(0.0, args.total_time + 1e-9, args.dt)

    log = {key: [] for key in [
        "t", "x", "y", "heading_deg", "left_v", "right_v", "left_ref", "right_ref", "left_cmd", "right_cmd",
        "robot_x_err", "robot_y_err", "theta_err_deg"
    ]}

    for t in times:
        if args.switch_time is not None and t >= args.switch_time:
            commanded_forward = -args.forward_input
        else:
            commanded_forward = args.forward_input

        if args.ramp_time > 0.0:
            forward_input = commanded_forward * min(1.0, t / args.ramp_time)
        else:
            forward_input = commanded_forward

        v_ref = forward_input_to_line_velocity(model, forward_input)
        wheel_ref = model.chassis_to_wheels(v_ref, 0.0)
        nominal_voltage = np.array([model.kv_linear * v_ref, model.kv_linear * v_ref], dtype=float)

        voltages, robot_error = controller.control(pose, wheel_state, line_point, line_heading, wheel_ref, nominal_voltage)

        wheel_dot = model.wheel_A @ wheel_state + model.wheel_B @ voltages
        wheel_state = wheel_state + wheel_dot * args.dt

        v_linear = 0.5 * (wheel_state[0] + wheel_state[1])
        omega = (wheel_state[1] - wheel_state[0]) / model.trackwidth_in
        pose[0] += v_linear * math.cos(pose[2]) * args.dt
        pose[1] += v_linear * math.sin(pose[2]) * args.dt
        pose[2] = wrap_to_pi(pose[2] + omega * args.dt)

        log["t"].append(t)
        log["x"].append(pose[0])
        log["y"].append(pose[1])
        log["heading_deg"].append(math.degrees(pose[2]))
        log["left_v"].append(wheel_state[0])
        log["right_v"].append(wheel_state[1])
        log["left_ref"].append(wheel_ref[0])
        log["right_ref"].append(wheel_ref[1])
        log["left_cmd"].append(voltages[0])
        log["right_cmd"].append(voltages[1])
        log["robot_x_err"].append(robot_error[0])
        log["robot_y_err"].append(robot_error[1])
        log["theta_err_deg"].append(math.degrees(robot_error[2]))

    fig, axes = plt.subplots(2, 2, figsize=(12, 9))
    fig.suptitle("Projected-Line LTV Simulation")

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

    axes[0, 1].plot(log["t"], log["robot_x_err"], label="robot-frame x err")
    axes[0, 1].plot(log["t"], log["robot_y_err"], label="robot-frame y err")
    axes[0, 1].plot(log["t"], log["theta_err_deg"], label="theta err (deg)")
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
    print(f"  final pose: x={pose[0]:.3f} in, y={pose[1]:.3f} in, heading={math.degrees(pose[2]):.3f} deg")
    print(f"  final robot-frame x error: {log['robot_x_err'][-1]:.3f} in")
    print(f"  final robot-frame y error: {log['robot_y_err'][-1]:.3f} in")
    print(f"  final heading error: {log['theta_err_deg'][-1]:.3f} deg")
    print(f"  plot: {output_path}")


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Simulate projected-line following with the full LTV differential controller.")
    parser.add_argument(
        "--robot-config",
        default=str(Path(__file__).resolve().parents[2] / "src" / "robot-config.cpp"),
        help="Path to robot-config.cpp containing TankDriveModel drive_model(...).",
    )
    parser.add_argument("--dt", type=float, default=0.01)
    parser.add_argument("--total-time", type=float, default=3.0)
    parser.add_argument("--forward-input", type=float, default=0.5)
    parser.add_argument("--ramp-time", type=float, default=0.2)
    parser.add_argument(
        "--switch-time",
        type=float,
        default=None,
        help="Time in seconds to flip the sign of the forward input. Default is no switch.",
    )
    parser.add_argument("--line-x", type=float, default=0.0)
    parser.add_argument("--line-y", type=float, default=0.0)
    parser.add_argument("--line-heading-deg", type=float, default=0.0)
    parser.add_argument("--reverse", action="store_true")
    parser.add_argument("--x0", type=float, default=0.0)
    parser.add_argument("--y0", type=float, default=18.0)
    parser.add_argument("--heading-deg0", type=float, default=35.0)
    parser.add_argument("--q-x", type=float, default=1000.0)
    parser.add_argument("--q-y", type=float, default=5.0)
    parser.add_argument("--q-theta", type=float, default=0.2)
    parser.add_argument("--q-left-vel", type=float, default=80.0)
    parser.add_argument("--q-right-vel", type=float, default=80.0)
    parser.add_argument("--r-left", type=float, default=12.0)
    parser.add_argument("--r-right", type=float, default=12.0)
    parser.add_argument("--velocity-step", type=float, default=0.5)
    parser.add_argument("--min-velocity", type=float, default=2.0)
    parser.add_argument("--plot-line-extent", type=float, default=120.0)
    parser.add_argument(
        "--output",
        default=str(Path(__file__).resolve().parent / "ltv_projected_line_sim.png"),
        help="PNG file to write.",
    )
    parser.add_argument("--dpi", type=int, default=160)
    return parser


if __name__ == "__main__":
    parser = build_arg_parser()
    args = parser.parse_args()
    if args.switch_time is None:
        args.switch_time = 0.5 * args.total_time
    simulate(args)
