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
from scipy.linalg import solve_discrete_are


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


def discretize_ab(A: np.ndarray, B: np.ndarray, dt: float) -> tuple[np.ndarray, np.ndarray]:
    n = A.shape[0]
    m = B.shape[1]
    M = np.zeros((n + m, n + m), dtype=float)
    M[:n, :n] = A
    M[:n, n:] = B
    Md = scipy_expm(M * dt)
    return Md[:n, :n], Md[:n, n:]


def scipy_expm(matrix: np.ndarray) -> np.ndarray:
    from scipy.linalg import expm

    return expm(matrix)


class LineFollowingLQR:
    def __init__(
        self,
        model: DriveModel,
        q_cte: float,
        q_theta: float,
        r_voltage: float,
        dt: float,
        velocity_step: float,
        min_velocity: float,
    ) -> None:
        self.model = model
        self.dt = dt
        self.min_velocity = max(min_velocity, 1e-3)
        self.q = np.diag([1.0 / (q_cte * q_cte), 1.0 / (q_theta * q_theta)])
        self.r = np.array([[1.0 / (r_voltage * r_voltage)]], dtype=float)
        self.schedule = {}

        v = -model.max_linear_speed_inps
        while v <= model.max_linear_speed_inps + 1e-9:
            linearized_v = v
            if abs(linearized_v) < self.min_velocity:
                linearized_v = -self.min_velocity if linearized_v < 0.0 else self.min_velocity

            A = np.array([[0.0, linearized_v], [0.0, 0.0]], dtype=float)
            B = np.array([[0.0], [-1.0 / model.ka_angular]], dtype=float)
            Ad, Bd = discretize_ab(A, B, dt)
            P = solve_discrete_are(Ad, Bd, self.q, self.r)
            K = np.linalg.solve(Bd.T @ P @ Bd + self.r, Bd.T @ P @ Ad)
            self.schedule[round(v, 6)] = K
            v += velocity_step

        self.sorted_schedule = np.array(sorted(self.schedule.keys()), dtype=float)

    def gains(self, forward_speed_inps: float) -> np.ndarray:
        target = forward_speed_inps
        if abs(target) < self.min_velocity:
            target = -self.min_velocity if target < 0.0 else self.min_velocity
        idx = int(np.argmin(np.abs(self.sorted_schedule - target)))
        return self.schedule[float(self.sorted_schedule[idx])]

    def control(
        self,
        pose: np.ndarray,
        wheel_vel_inps: np.ndarray,
        line_point_in: np.ndarray,
        line_heading_rad: float,
        common_mode_voltage: float,
        forward_velocity_ref_inps: float,
    ) -> tuple[np.ndarray, np.ndarray]:
        dx = pose[0] - line_point_in[0]
        dy = pose[1] - line_point_in[1]
        e_cte = -dx * math.sin(line_heading_rad) + dy * math.cos(line_heading_rad)
        e_theta = wrap_to_pi(pose[2] - line_heading_rad)

        linear_velocity = float(wheel_vel_inps.mean())
        if abs(linear_velocity) < self.min_velocity:
            linear_velocity = forward_velocity_ref_inps
        K = self.gains(linear_velocity)
        error = np.array([e_cte, e_theta], dtype=float)
        dV = float(-(K @ error.reshape(2, 1))[0, 0])

        left = np.clip(common_mode_voltage + dV, -self.model.max_voltage, self.model.max_voltage)
        right = np.clip(common_mode_voltage - dV, -self.model.max_voltage, self.model.max_voltage)
        return np.array([left, right], dtype=float), error


def forward_input_to_linear_velocity(model: DriveModel, forward_back: float) -> float:
    forward_back = max(-1.0, min(1.0, forward_back))
    return forward_back * model.max_linear_speed_inps


def forward_input_to_common_mode_voltage(model: DriveModel, forward_back: float) -> float:
    target_velocity = forward_input_to_linear_velocity(model, forward_back)
    return np.clip(model.kv_linear * target_velocity, -model.max_voltage, model.max_voltage)


def simulate(args: argparse.Namespace) -> None:
    model = parse_drive_model(Path(args.robot_config))
    controller = LineFollowingLQR(
        model=model,
        q_cte=args.q_cte,
        q_theta=args.q_theta,
        r_voltage=args.r_voltage,
        dt=args.dt,
        velocity_step=args.velocity_step,
        min_velocity=args.min_velocity,
    )

    pose = np.array([args.x0, args.y0, math.radians(args.heading_deg0)], dtype=float)
    chassis_state = np.array([0.0, 0.0], dtype=float)
    line_point = np.array([args.line_x, args.line_y], dtype=float)
    line_heading = math.radians(args.line_heading_deg + (180.0 if args.reverse else 0.0))

    wheel_state = np.zeros(2, dtype=float)
    times = np.arange(0.0, args.total_time + 1e-9, args.dt)

    log = {
        "t": [],
        "x": [],
        "y": [],
        "heading_deg": [],
        "v": [],
        "omega": [],
        "left_v": [],
        "right_v": [],
        "cte": [],
        "etheta_deg": [],
        "left_cmd": [],
        "right_cmd": [],
        "common_mode": [],
    }

    for t in times:
        if args.ramp_time > 0.0:
            forward_input = args.forward_input * min(1.0, t / args.ramp_time)
        else:
            forward_input = args.forward_input

        common_mode = forward_input_to_common_mode_voltage(model, forward_input)
        forward_ref = forward_input_to_linear_velocity(model, forward_input)
        voltages, error = controller.control(
            pose=pose,
            wheel_vel_inps=wheel_state,
            line_point_in=line_point,
            line_heading_rad=line_heading,
            common_mode_voltage=common_mode,
            forward_velocity_ref_inps=forward_ref,
        )

        chassis_dot = model.chassis_A @ chassis_state + model.chassis_B @ voltages
        chassis_state = chassis_state + chassis_dot * args.dt

        v = chassis_state[0]
        omega = chassis_state[1]
        wheel_state = np.array(
            [
                v - omega * model.trackwidth_in * 0.5,
                v + omega * model.trackwidth_in * 0.5,
            ],
            dtype=float,
        )

        pose[0] += v * math.cos(pose[2]) * args.dt
        pose[1] += v * math.sin(pose[2]) * args.dt
        pose[2] = wrap_to_pi(pose[2] + omega * args.dt)

        log["t"].append(t)
        log["x"].append(pose[0])
        log["y"].append(pose[1])
        log["heading_deg"].append(math.degrees(pose[2]))
        log["v"].append(v)
        log["omega"].append(omega)
        log["left_v"].append(wheel_state[0])
        log["right_v"].append(wheel_state[1])
        log["cte"].append(error[0])
        log["etheta_deg"].append(math.degrees(error[1]))
        log["left_cmd"].append(voltages[0])
        log["right_cmd"].append(voltages[1])
        log["common_mode"].append(common_mode)

    fig, axes = plt.subplots(2, 2, figsize=(12, 9))
    fig.suptitle("Line-Following LQR Simulation")

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
    axes[0, 1].set_xlabel("time (s)")
    axes[0, 1].legend()

    axes[1, 0].plot(log["t"], log["v"], label="linear velocity (in/s)")
    axes[1, 0].plot(log["t"], log["omega"], label="angular velocity (rad/s)")
    axes[1, 0].set_xlabel("time (s)")
    axes[1, 0].legend()

    axes[1, 1].plot(log["t"], log["left_cmd"], label="left command (V)")
    axes[1, 1].plot(log["t"], log["right_cmd"], label="right command (V)")
    axes[1, 1].plot(log["t"], log["common_mode"], "--", label="common mode (V)")
    axes[1, 1].set_xlabel("time (s)")
    axes[1, 1].legend()

    fig.tight_layout()
    output_path = Path(args.output).resolve()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(str(output_path), dpi=args.dpi, bbox_inches="tight")
    plt.close(fig)

    print("Drive model from robot-config.cpp")
    print(f"  trackwidth: {model.trackwidth_in:.3f} in")
    print(f"  max_voltage: {model.max_voltage:.3f} V")
    print(f"  kV_linear: {model.kv_linear:.6f} V/(in/s)")
    print(f"  kA_linear: {model.ka_linear:.6f} V/(in/s^2)")
    print(f"  kV_angular: {model.kv_angular:.6f} V/(rad/s)")
    print(f"  kA_angular: {model.ka_angular:.6f} V/(rad/s^2)")
    print(f"  estimated max linear speed: {model.max_linear_speed_inps:.3f} in/s")
    print(f"  final pose: x={pose[0]:.3f} in, y={pose[1]:.3f} in, heading={math.degrees(pose[2]):.3f} deg")
    print(f"  final cross-track error: {log['cte'][-1]:.3f} in")
    print(f"  final heading error: {log['etheta_deg'][-1]:.3f} deg")
    print(f"  plot: {output_path}")


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Simulate the reduced-order line-following LQR controller.")
    parser.add_argument(
        "--robot-config",
        default=str(Path(__file__).resolve().parents[2] / "src" / "robot-config.cpp"),
        help="Path to robot-config.cpp containing TankDriveModel drive_model(...).",
    )
    parser.add_argument("--dt", type=float, default=0.01, help="Simulation timestep in seconds.")
    parser.add_argument("--total-time", type=float, default=3.0, help="Total simulation duration in seconds.")
    parser.add_argument("--forward-input", type=float, default=0.5, help="User forward/backward command in [-1, 1].")
    parser.add_argument("--ramp-time", type=float, default=0.2, help="Seconds to ramp forward input from 0.")
    parser.add_argument("--line-x", type=float, default=0.0, help="A point on the reference line, x in inches.")
    parser.add_argument("--line-y", type=float, default=0.0, help="A point on the reference line, y in inches.")
    parser.add_argument("--line-heading-deg", type=float, default=0.0, help="Reference line heading in degrees.")
    parser.add_argument("--reverse", action="store_true", help="Follow the line facing the reverse direction.")
    parser.add_argument("--x0", type=float, default=0.0, help="Initial x position in inches.")
    parser.add_argument("--y0", type=float, default=18.0, help="Initial y position in inches.")
    parser.add_argument("--heading-deg0", type=float, default=35.0, help="Initial heading in degrees.")
    parser.add_argument("--q-cte", type=float, default=5.0, help="Cross-track tolerance for Bryson-style Q tuning.")
    parser.add_argument("--q-theta", type=float, default=0.2, help="Heading tolerance in radians for Bryson-style Q.")
    parser.add_argument("--r-voltage", type=float, default=0.5, help="Differential voltage tolerance for Bryson-style R.")
    parser.add_argument("--velocity-step", type=float, default=0.5, help="Gain-scheduling step in in/s.")
    parser.add_argument("--min-velocity", type=float, default=0.1, help="Minimum scheduled velocity in in/s.")
    parser.add_argument("--plot-line-extent", type=float, default=120.0, help="Half-length of the line shown in the path plot.")
    parser.add_argument(
        "--output",
        default=str(Path(__file__).resolve().parent / "line_follow_lqr_sim.png"),
        help="PNG file to write.",
    )
    parser.add_argument("--dpi", type=int, default=160, help="Output PNG resolution.")
    return parser


if __name__ == "__main__":
    simulate(build_arg_parser().parse_args())
