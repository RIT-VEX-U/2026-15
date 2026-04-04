#!/usr/bin/env python3

import argparse
import bisect
import csv
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Optional, Sequence, Tuple


BASE_HEADER = "idx,t,s,x,y,heading_deg,curvature,velocity,acceleration,omega"
RUNTIME_HEADER = "idx,t,x,y,heading_deg,velocity,omega,left_voltage,right_voltage"


@dataclass
class BaseRow:
    idx: int
    t: float
    s: float
    x: float
    y: float
    heading_deg: float
    curvature: float
    velocity: float
    acceleration: float
    omega: float


@dataclass
class RuntimeRow:
    idx: int
    t: float
    x: float
    y: float
    heading_deg: float
    velocity: float
    omega: float
    left_voltage: float
    right_voltage: float
    linear_voltage: float = 0.0
    angular_voltage: float = 0.0
    linear_accel: float = 0.0
    angular_accel: float = 0.0


@dataclass
class TrackingError:
    pos_err: float
    heading_err_deg: float
    velocity_err: float
    omega_err: float


@dataclass
class FitResult:
    kv: float
    ka: float
    rms_residual: float
    weighted_r2: float
    sample_count: int


def wrap_deg_error_deg(a: float, b: float) -> float:
    diff = (a - b + 180.0) % 360.0 - 180.0
    return diff


def lerp(a: float, b: float, u: float) -> float:
    return a + (b - a) * u


def lerp_heading_deg(a: float, b: float, u: float) -> float:
    return a + wrap_deg_error_deg(b, a) * u


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Fit TankDriveModel feedforward constants from trajectory follower CSV logs. "
            "Runtime CSV is required. Reference/base trajectory CSV is optional and is used "
            "for tracking-error diagnostics and weighting."
        )
    )
    parser.add_argument(
        "combined",
        nargs="?",
        help="Combined log file containing both printed CSV sections. If provided, --runtime/--reference are optional.",
    )
    parser.add_argument("--runtime", help="Runtime CSV with header: " + RUNTIME_HEADER)
    parser.add_argument("--reference", help="Base trajectory CSV with header: " + BASE_HEADER)
    parser.add_argument("--pos-scale", type=float, default=6.0, help="Position error weighting scale in inches.")
    parser.add_argument("--heading-scale", type=float, default=15.0, help="Heading error weighting scale in degrees.")
    parser.add_argument("--velocity-scale", type=float, default=24.0, help="Velocity error weighting scale in in/s.")
    parser.add_argument("--omega-scale", type=float, default=2.0, help="Omega error weighting scale in rad/s.")
    parser.add_argument(
        "--max-wheel-voltage",
        type=float,
        default=11.8,
        help="Discard samples where either wheel voltage magnitude exceeds this threshold.",
    )
    parser.add_argument("--min-linear-speed", type=float, default=3.0, help="Minimum |linear velocity| in in/s.")
    parser.add_argument("--min-linear-accel", type=float, default=8.0, help="Minimum |linear accel| in in/s^2.")
    parser.add_argument("--min-angular-speed", type=float, default=0.35, help="Minimum |omega| in rad/s.")
    parser.add_argument("--min-angular-accel", type=float, default=0.8, help="Minimum |alpha| in rad/s^2.")
    parser.add_argument("--smooth-radius", type=int, default=2, help="Radius for moving-average smoothing.")
    parser.add_argument("--robust-iters", type=int, default=3, help="Robust reweighting iterations.")
    return parser.parse_args()


def parse_combined_log(path: Path) -> Tuple[List[BaseRow], List[RuntimeRow]]:
    base_lines: List[str] = []
    runtime_lines: List[str] = []
    section: Optional[str] = None

    for raw_line in path.read_text().splitlines():
        line = raw_line.strip()
        if not line:
            continue
        if line == BASE_HEADER:
            section = "base"
            continue
        if line == RUNTIME_HEADER:
            section = "runtime"
            continue

        if section == "base":
            base_lines.append(line)
        elif section == "runtime":
            runtime_lines.append(line)

    return parse_base_lines(base_lines), parse_runtime_lines(runtime_lines)


def parse_csv_dicts(path: Path) -> Iterable[dict]:
    with path.open(newline="") as fp:
        reader = csv.DictReader(fp)
        for row in reader:
            yield row


def parse_base_lines(lines: Sequence[str]) -> List[BaseRow]:
    rows: List[BaseRow] = []
    for line in lines:
        parts = [p.strip() for p in line.split(",")]
        if len(parts) != 10:
            continue
        try:
            rows.append(
                BaseRow(
                    idx=int(parts[0]),
                    t=float(parts[1]),
                    s=float(parts[2]),
                    x=float(parts[3]),
                    y=float(parts[4]),
                    heading_deg=float(parts[5]),
                    curvature=float(parts[6]),
                    velocity=float(parts[7]),
                    acceleration=float(parts[8]),
                    omega=float(parts[9]),
                )
            )
        except ValueError:
            continue
    return rows


def parse_runtime_lines(lines: Sequence[str]) -> List[RuntimeRow]:
    rows: List[RuntimeRow] = []
    for line in lines:
        parts = [p.strip() for p in line.split(",")]
        if len(parts) != 9:
            continue
        try:
            rows.append(
                RuntimeRow(
                    idx=int(parts[0]),
                    t=float(parts[1]),
                    x=float(parts[2]),
                    y=float(parts[3]),
                    heading_deg=float(parts[4]),
                    velocity=float(parts[5]),
                    omega=float(parts[6]),
                    left_voltage=float(parts[7]),
                    right_voltage=float(parts[8]),
                )
            )
        except ValueError:
            continue
    return rows


def parse_base_csv(path: Path) -> List[BaseRow]:
    rows: List[BaseRow] = []
    for row in parse_csv_dicts(path):
        rows.append(
            BaseRow(
                idx=int(row["idx"]),
                t=float(row["t"]),
                s=float(row["s"]),
                x=float(row["x"]),
                y=float(row["y"]),
                heading_deg=float(row["heading_deg"]),
                curvature=float(row["curvature"]),
                velocity=float(row["velocity"]),
                acceleration=float(row["acceleration"]),
                omega=float(row["omega"]),
            )
        )
    return rows


def parse_runtime_csv(path: Path) -> List[RuntimeRow]:
    rows: List[RuntimeRow] = []
    for row in parse_csv_dicts(path):
        rows.append(
            RuntimeRow(
                idx=int(row["idx"]),
                t=float(row["t"]),
                x=float(row["x"]),
                y=float(row["y"]),
                heading_deg=float(row["heading_deg"]),
                velocity=float(row["velocity"]),
                omega=float(row["omega"]),
                left_voltage=float(row["left_voltage"]),
                right_voltage=float(row["right_voltage"]),
            )
        )
    return rows


def moving_average(values: Sequence[float], radius: int) -> List[float]:
    if radius <= 0:
        return list(values)
    out: List[float] = []
    for i in range(len(values)):
        lo = max(0, i - radius)
        hi = min(len(values), i + radius + 1)
        out.append(sum(values[lo:hi]) / float(hi - lo))
    return out


def differentiate(times: Sequence[float], values: Sequence[float]) -> List[float]:
    n = len(values)
    if n == 0:
        return []
    if n == 1:
        return [0.0]

    out = [0.0] * n
    for i in range(n):
        if i == 0:
            dt = times[1] - times[0]
            out[i] = 0.0 if abs(dt) < 1e-9 else (values[1] - values[0]) / dt
        elif i == n - 1:
            dt = times[-1] - times[-2]
            out[i] = 0.0 if abs(dt) < 1e-9 else (values[-1] - values[-2]) / dt
        else:
            dt = times[i + 1] - times[i - 1]
            out[i] = 0.0 if abs(dt) < 1e-9 else (values[i + 1] - values[i - 1]) / dt
    return out


def finalize_runtime_rows(rows: List[RuntimeRow], smooth_radius: int) -> None:
    if not rows:
        return

    times = [row.t for row in rows]
    vel = moving_average([row.velocity for row in rows], smooth_radius)
    omega = moving_average([row.omega for row in rows], smooth_radius)
    accel = moving_average(differentiate(times, vel), smooth_radius)
    alpha = moving_average(differentiate(times, omega), smooth_radius)

    for i, row in enumerate(rows):
        row.linear_voltage = 0.5 * (row.left_voltage + row.right_voltage)
        row.angular_voltage = 0.5 * (row.right_voltage - row.left_voltage)
        row.linear_accel = accel[i]
        row.angular_accel = alpha[i]


def interpolate_base_row(rows: Sequence[BaseRow], t: float) -> BaseRow:
    if not rows:
        raise ValueError("No reference rows available for interpolation")
    if t <= rows[0].t:
        return rows[0]
    if t >= rows[-1].t:
        return rows[-1]

    ts = [row.t for row in rows]
    idx = bisect.bisect_left(ts, t)
    prev_row = rows[idx - 1]
    next_row = rows[idx]
    dt = next_row.t - prev_row.t
    if abs(dt) < 1e-9:
        return prev_row

    u = (t - prev_row.t) / dt
    return BaseRow(
        idx=prev_row.idx,
        t=t,
        s=lerp(prev_row.s, next_row.s, u),
        x=lerp(prev_row.x, next_row.x, u),
        y=lerp(prev_row.y, next_row.y, u),
        heading_deg=lerp_heading_deg(prev_row.heading_deg, next_row.heading_deg, u),
        curvature=lerp(prev_row.curvature, next_row.curvature, u),
        velocity=lerp(prev_row.velocity, next_row.velocity, u),
        acceleration=lerp(prev_row.acceleration, next_row.acceleration, u),
        omega=lerp(prev_row.omega, next_row.omega, u),
    )


def compute_tracking_error(ref: BaseRow, actual: RuntimeRow) -> TrackingError:
    return TrackingError(
        pos_err=math.hypot(actual.x - ref.x, actual.y - ref.y),
        heading_err_deg=wrap_deg_error_deg(actual.heading_deg, ref.heading_deg),
        velocity_err=actual.velocity - ref.velocity,
        omega_err=actual.omega - ref.omega,
    )


def tracking_weight(err: TrackingError, args: argparse.Namespace) -> float:
    denom = 1.0
    denom += (err.pos_err / args.pos_scale) ** 2
    denom += (err.heading_err_deg / args.heading_scale) ** 2
    denom += (err.velocity_err / args.velocity_scale) ** 2
    denom += (err.omega_err / args.omega_scale) ** 2
    return 1.0 / denom


def huber_weight(residual: float, scale: float) -> float:
    if scale <= 1e-9:
        return 1.0
    threshold = 1.5 * scale
    mag = abs(residual)
    if mag <= threshold:
        return 1.0
    return threshold / mag


def weighted_zero_intercept_fit(
    samples: Sequence[Tuple[float, float, float, float]], robust_iters: int
) -> FitResult:
    if len(samples) < 2:
        raise ValueError("Not enough samples to fit")

    base_weights = [max(sample[3], 1e-9) for sample in samples]
    weights = list(base_weights)
    kv = 0.0
    ka = 0.0

    for _ in range(max(1, robust_iters)):
        s11 = s12 = s22 = t1 = t2 = 0.0
        for (v, a, u, _), w in zip(samples, weights):
            s11 += w * v * v
            s12 += w * v * a
            s22 += w * a * a
            t1 += w * v * u
            t2 += w * a * u

        det = s11 * s22 - s12 * s12
        if abs(det) < 1e-12:
            raise ValueError("Singular fit matrix; need more excitation in both velocity and acceleration")

        kv = (t1 * s22 - t2 * s12) / det
        ka = (s11 * t2 - s12 * t1) / det

        residuals = [u - (kv * v + ka * a) for (v, a, u, _) in samples]
        abs_residuals = sorted(abs(r) for r in residuals)
        scale = abs_residuals[len(abs_residuals) // 2] if abs_residuals else 0.0
        weights = [bw * huber_weight(r, scale) for bw, r in zip(base_weights, residuals)]

    residuals = [u - (kv * v + ka * a) for (v, a, u, _) in samples]
    weight_sum = sum(base_weights)
    rms = math.sqrt(sum(w * r * r for w, r in zip(base_weights, residuals)) / max(weight_sum, 1e-9))
    weighted_mean = sum(w * u for (_, _, u, _), w in zip(samples, base_weights)) / max(weight_sum, 1e-9)
    total_ss = sum(w * (u - weighted_mean) ** 2 for (_, _, u, _), w in zip(samples, base_weights))
    resid_ss = sum(w * r * r for w, r in zip(base_weights, residuals))
    r2 = 1.0 - resid_ss / total_ss if total_ss > 1e-9 else 0.0

    return FitResult(kv=kv, ka=ka, rms_residual=rms, weighted_r2=r2, sample_count=len(samples))


def select_linear_samples(
    rows: Sequence[RuntimeRow],
    ref_rows: Optional[Sequence[BaseRow]],
    args: argparse.Namespace,
) -> Tuple[List[Tuple[float, float, float, float]], List[TrackingError]]:
    samples: List[Tuple[float, float, float, float]] = []
    errors: List[TrackingError] = []

    for row in rows:
        if max(abs(row.left_voltage), abs(row.right_voltage)) > args.max_wheel_voltage:
            continue
        if abs(row.velocity) < args.min_linear_speed and abs(row.linear_accel) < args.min_linear_accel:
            continue

        weight = 1.0
        if ref_rows:
            err = compute_tracking_error(interpolate_base_row(ref_rows, row.t), row)
            errors.append(err)
            weight = tracking_weight(err, args)
        samples.append((row.velocity, row.linear_accel, row.linear_voltage, weight))

    return samples, errors


def select_angular_samples(
    rows: Sequence[RuntimeRow],
    ref_rows: Optional[Sequence[BaseRow]],
    args: argparse.Namespace,
) -> Tuple[List[Tuple[float, float, float, float]], List[TrackingError]]:
    samples: List[Tuple[float, float, float, float]] = []
    errors: List[TrackingError] = []

    for row in rows:
        if max(abs(row.left_voltage), abs(row.right_voltage)) > args.max_wheel_voltage:
            continue
        if abs(row.omega) < args.min_angular_speed and abs(row.angular_accel) < args.min_angular_accel:
            continue

        weight = 1.0
        if ref_rows:
            err = compute_tracking_error(interpolate_base_row(ref_rows, row.t), row)
            errors.append(err)
            weight = tracking_weight(err, args)
        samples.append((row.omega, row.angular_accel, row.angular_voltage, weight))

    return samples, errors


def summarize_tracking(rows: Sequence[RuntimeRow], ref_rows: Sequence[BaseRow]) -> str:
    if not rows or not ref_rows:
        return ""

    pos_sq = heading_sq = vel_sq = omega_sq = 0.0
    for row in rows:
        err = compute_tracking_error(interpolate_base_row(ref_rows, row.t), row)
        pos_sq += err.pos_err * err.pos_err
        heading_sq += err.heading_err_deg * err.heading_err_deg
        vel_sq += err.velocity_err * err.velocity_err
        omega_sq += err.omega_err * err.omega_err

    n = float(len(rows))
    return (
        f"Tracking RMS: pos={math.sqrt(pos_sq / n):.3f} in, "
        f"heading={math.sqrt(heading_sq / n):.3f} deg, "
        f"velocity={math.sqrt(vel_sq / n):.3f} in/s, "
        f"omega={math.sqrt(omega_sq / n):.3f} rad/s"
    )


def load_inputs(args: argparse.Namespace) -> Tuple[List[BaseRow], List[RuntimeRow]]:
    ref_rows: List[BaseRow] = []
    runtime_rows: List[RuntimeRow] = []

    if args.combined:
        ref_rows, runtime_rows = parse_combined_log(Path(args.combined))

    if args.reference:
        ref_rows = parse_base_csv(Path(args.reference))

    if args.runtime:
        runtime_rows = parse_runtime_csv(Path(args.runtime))

    if not runtime_rows:
        raise SystemExit("No runtime rows found. Provide --runtime or a combined log containing the runtime CSV section.")

    return ref_rows, runtime_rows


def print_fit(name: str, result: FitResult, unit_kv: str, unit_ka: str) -> None:
    print(f"{name}:")
    print(f"  samples={result.sample_count}")
    print(f"  kV={result.kv:.6f} {unit_kv}")
    print(f"  kA={result.ka:.6f} {unit_ka}")
    print(f"  rms_residual={result.rms_residual:.6f} V")
    print(f"  weighted_r2={result.weighted_r2:.4f}")


def main() -> None:
    args = parse_args()
    ref_rows, runtime_rows = load_inputs(args)
    finalize_runtime_rows(runtime_rows, args.smooth_radius)

    linear_samples, _ = select_linear_samples(runtime_rows, ref_rows if ref_rows else None, args)
    angular_samples, _ = select_angular_samples(runtime_rows, ref_rows if ref_rows else None, args)

    linear_fit = weighted_zero_intercept_fit(linear_samples, args.robust_iters)
    angular_fit = weighted_zero_intercept_fit(angular_samples, args.robust_iters)

    print_fit("Linear", linear_fit, "V/(in/s)", "V/(in/s^2)")
    print_fit("Angular", angular_fit, "V/(rad/s)", "V/(rad/s^2)")

    if ref_rows:
        print(summarize_tracking(runtime_rows, ref_rows))
        print("Reference weighting: enabled")
    else:
        print("Reference weighting: disabled")

    print()
    print("Suggested TankDriveModel feedforward values:")
    print(f"  kV_linear  = {linear_fit.kv:.6f}  // VpInps")
    print(f"  kA_linear  = {linear_fit.ka:.6f}  // VpInps2")
    print(f"  kV_angular = {angular_fit.kv:.6f}  // VspRad")
    print(f"  kA_angular = {angular_fit.ka:.6f}  // Vs2pRad")


if __name__ == "__main__":
    main()
