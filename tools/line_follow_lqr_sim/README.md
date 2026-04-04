# Line-Following LQR Simulation

This tool simulates the reduced-order line-following LQR controller using the
current `TankDriveModel drive_model(...)` defined in `src/robot-config.cpp`.

## Setup

```bash
cd /home/pascal/vex/2026-15
./tools/line_follow_lqr_sim/setup_venv.sh
source tools/line_follow_lqr_sim/.venv/bin/activate
```

## Run

```bash
python tools/line_follow_lqr_sim/sim.py
```

That writes a PNG to `tools/line_follow_lqr_sim/line_follow_lqr_sim.png`.

Example with a different start pose and line:

```bash
python tools/line_follow_lqr_sim/sim.py \
  --x0 0 --y0 12 --heading-deg0 25 \
  --line-x 0 --line-y 0 --line-heading-deg 0 \
  --forward-input 0.6 \
  --output tools/line_follow_lqr_sim/test_run.png
```

Use `--reverse` to simulate following the same line while facing the reverse
direction.
