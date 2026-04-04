#!/usr/bin/env python3
import argparse
import math
import re
from dataclasses import dataclass
from pathlib import Path

import numpy as np


@dataclass
class DriveModel:
    trackwidth_in: float
    max_voltage: float
    kv_linear: float
    ka_linear: float
    kv_angular: float
    ka_angular: float


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


def build_full_linearization(model: DriveModel, nominal_speed_inps: float) -> tuple[np.ndarray, np.ndarray]:
    # x = [e_parallel, e_cte, e_theta, v_err, omega_err]
    # u = [delta_common_mode, delta_differential]
    A = np.zeros((5, 5), dtype=float)
    B = np.zeros((5, 2), dtype=float)

    A[0, 3] = 1.0
    A[1, 2] = nominal_speed_inps
    A[2, 4] = 1.0
    A[3, 3] = -model.kv_linear / model.ka_linear
    A[4, 4] = -model.kv_angular / model.ka_angular

    B[3, 0] = 1.0 / model.ka_linear
    B[4, 1] = -1.0 / model.ka_angular
    return A, B


def discretize_euler(A: np.ndarray, B: np.ndarray, dt: float) -> tuple[np.ndarray, np.ndarray]:
    n = A.shape[0]
    Ad = np.eye(n, dtype=float) + A * dt
    Bd = B * dt
    return Ad, Bd


def main() -> None:
    parser = argparse.ArgumentParser(description="Analyze whether common-mode participates in line-stabilizing LQR.")
    parser.add_argument(
        "--robot-config",
        default=str(Path(__file__).resolve().parents[2] / "src" / "robot-config.cpp"),
        help="Path to robot-config.cpp containing TankDriveModel drive_model(...).",
    )
    parser.add_argument("--dt", type=float, default=0.01)
    parser.add_argument("--nominal-speed", type=float, default=20.0, help="Nominal line speed in in/s.")
    parser.add_argument("--q-eparallel", type=float, default=1e6, help="Tolerance on along-line error.")
    parser.add_argument("--q-cte", type=float, default=5.0, help="Tolerance on cross-track error.")
    parser.add_argument("--q-theta", type=float, default=0.2, help="Tolerance on heading error in rad.")
    parser.add_argument("--q-v", type=float, default=1e6, help="Tolerance on linear velocity error.")
    parser.add_argument("--q-omega", type=float, default=1e6, help="Tolerance on angular velocity error.")
    parser.add_argument("--r-common", type=float, default=1.0, help="Tolerance on common-mode voltage.")
    parser.add_argument("--r-diff", type=float, default=0.5, help="Tolerance on differential voltage.")
    args = parser.parse_args()

    model = parse_drive_model(Path(args.robot_config))
    A, B = build_full_linearization(model, args.nominal_speed)
    Ad, Bd = discretize_euler(A, B, args.dt)

    q_tols = np.array([args.q_eparallel, args.q_cte, args.q_theta, args.q_v, args.q_omega], dtype=float)
    r_tols = np.array([args.r_common, args.r_diff], dtype=float)
    Q = np.diag(1.0 / np.square(q_tols))
    R = np.diag(1.0 / np.square(r_tols))

    P = dare_sda(Ad, Bd, Q, R)
    K = np.linalg.solve(Bd.T @ P @ Bd + R, Bd.T @ P @ Ad)

    np.set_printoptions(precision=6, suppress=True)
    print("A =")
    print(A)
    print("\nB =")
    print(B)
    print("\nK =")
    print(K)
    print("\nCommon-mode gains on [e_parallel, e_cte, e_theta, v_err, omega_err] =")
    print(K[0, :])
    print("Differential gains on [e_parallel, e_cte, e_theta, v_err, omega_err] =")
    print(K[1, :])


if __name__ == "__main__":
    main()
