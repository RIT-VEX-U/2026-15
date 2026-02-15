#include "subsystems/superstructure.h"
#include "core/utils/command_structure/auto_command.h"
#include "vex.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

Superstructure::Superstructure(
    vex::motor_group &intake_motors,
    vex::motor_group &lever_motors,
    vex::rotation &lever_rotation,
    vex::digital_out *lift_left,
    vex::digital_out *lift_right,
    double intake_voltage,
    double outtake_voltage,
    double lever_up_voltage,
    double lever_down_voltage,
    double lever_up_position_top,
    double lever_up_position_middle,
    double lever_down_position,
    double lever_deadband,
    int stepper_steps,
    int stepper_window_ms,
    int step_delay_ms,
    int update_rate_ms,
    double lever_pd_kp,
    double lever_pd_kd,
    double lever_pid_deadband,
    bool lever_pid_inverted
) : intake_motors(intake_motors),
    lever_motors(lever_motors),
    lever_rotation(lever_rotation),
    lift_left(lift_left),
    lift_right(lift_right),
    intake_voltage(intake_voltage),
    outtake_voltage(outtake_voltage),
    lever_up_voltage(lever_up_voltage),
    lever_down_voltage(lever_down_voltage),
    lever_up_position_top(lever_up_position_top),
    lever_up_position_middle(lever_up_position_middle),
    lever_down_position(lever_down_position),
    lever_deadband(lever_deadband),
    stepper_steps(stepper_steps),
    stepper_window_ms(stepper_window_ms),
    step_delay_ms(step_delay_ms),
    update_rate_ms(update_rate_ms),
    lever_pd_kp(lever_pd_kp),
    lever_pd_kd(lever_pd_kd),
    lever_pid_deadband(lever_pid_deadband),
    lever_pid_inverted(lever_pid_inverted),
    intake_volts(intake_voltage)
{
    lever_pid_cfg.p = lever_pd_kp;
    lever_pid_cfg.i = 0.0;
    lever_pid_cfg.d = lever_pd_kd;
    lever_pid_cfg.deadband = lever_pid_deadband;
    lever_pid_cfg.on_target_time = 0.0;
    lever_pid_cfg.error_method = PID::ERROR_TYPE::LINEAR;
    lever_pid.set_limits(-12.0, 12.0);
}

void Superstructure::start() {
    if (task_started) return;
    task_started = true;
    task = new vex::task(thread_fn, this);
}

// === Commands for states ===

void Superstructure::intake() {
    intake(intake_voltage);
}

void Superstructure::intake(double volts) {
    intake_volts = volts;
    state = Intaking;
}

void Superstructure::outtake_top() {
    lever_reached_up = false;
    set_lift(true);
    state = OuttakingTop;
}

void Superstructure::outtake_middle() {
    lever_reached_up = false;
    set_lift(false);
    state = OuttakingMiddle;
}

void Superstructure::outtake_bottom() {
    set_lift(true);
    state = OuttakingBottom;
}

void Superstructure::stop() {
    stepper_executing = false;
    stepper_count = 0;
    state = Idle;
}

// === Stepper ===

void Superstructure::stepper_press(StepperMode mode) {
    if (stepper_executing) return;

    // First press
    if (stepper_count == 0 || stepper_mode != mode) {
        stepper_count = 0;
        stepper_timer = stepper_window_ms;
        stepper_mode = mode;
    }

    stepper_count++;

    // Max presses so just reset
    if (stepper_count >= stepper_steps) {
        stepper_count = stepper_steps;
        stepper_timer = 0;
    }

    stepper_end_position = (stepper_mode == StepperTop) ? lever_up_position_top :
                                                          lever_up_position_middle;
}

void Superstructure::hold_lever(double target_deg) {
    lever_hold_target = target_deg;
    reset_lever_pid(target_deg);
    state = LeverHold;
}

void Superstructure::release_lever_hold() {
    if (state == LeverHold) state = Idle;
}

// === Manual Lever ===
// should override other stuff

void Superstructure::manual_lever_up(double volts) {
    manual_lever_volts = volts;
    state = ManualLever;
}

void Superstructure::manual_lever_down(double volts) {
    manual_lever_volts = -volts;
    state = ManualLever;
}

void Superstructure::manual_lever_stop() {
    manual_lever_volts = 0.0;
    if (state == ManualLever) state = Idle;
}

// === Lift Control ===

void Superstructure::lift_extend() {
    set_lift(true);
}

void Superstructure::lift_retract() {
    set_lift(false);
}

void Superstructure::lift_toggle() {
    set_lift(!lift_extended);
}

bool Superstructure::lift_is_extended() const {
    return lift_extended;
}

Superstructure::State Superstructure::get_state() const {
    return state;
}

void Superstructure::set_lift(bool extend) {
    lift_extended = extend;
    if (lift_left != nullptr) {
        lift_left->set(extend);
    }
    if (lift_right != nullptr) {
        lift_right->set(extend);
    }
}

// === Statuses ===

double Superstructure::get_lever_angle() const {
    return lever_rotation.position(vex::rotationUnits::deg);
}

bool Superstructure::lever_at_target(double target) const {
    return fabs(get_lever_angle() - target) <= lever_deadband;
}

bool Superstructure::lever_at_down() const {
    return fabs(get_lever_angle() - lever_down_position) <= lever_deadband;
}

double Superstructure::lever_pid_output(double target_deg) {
    lever_pid.set_target(target_deg);
    double output = lever_pid.update(get_lever_angle());
    if (lever_pid_inverted) {
        output = -output;
    }
    return output;
}

void Superstructure::reset_lever_pid(double target_deg) {
    lever_pid.init(get_lever_angle(), target_deg);
}

double Superstructure::compute_step_degrees(double start_position,
                                            double end_position) const {
    double full_travel = fabs(end_position - start_position);
    if (stepper_steps <= 0) return full_travel;
    return full_travel / static_cast<double>(stepper_steps);
}

int Superstructure::compute_step_direction(double start_position,
                                           double end_position) const {
    return (end_position >= start_position) ? 1 : -1;
}

void Superstructure::apply_lever_voltage(double voltage) {
    if (fabs(voltage) < 0.05) {
        lever_motors.stop(vex::brakeType::brake);
    } else if (voltage > 0.0) {
        lever_motors.spin(vex::forward, voltage, vex::volt);
    } else {
        lever_motors.spin(vex::reverse, -voltage, vex::volt);
    }
}

void Superstructure::begin_stepper_execution(int count, StepperMode mode) {
    stepper_executing = true;
    stepper_balls_remaining = count;
    stepper_count = 0;
    stepper_delay_timer = 0;
    stepper_up_phase = true;
    stepper_waiting_at_top = false;
    stepper_returning = false;
    state = Stepping;

    stepper_mode = mode;
    stepper_end_position = (mode == StepperTop) ? lever_up_position_top :
                                                  lever_up_position_middle;

    stepper_start_angle = get_lever_angle();
    const double per_step = compute_step_degrees(lever_down_position,
                                                 stepper_end_position);
    double total_travel = per_step * static_cast<double>(count);
    const double full_travel = per_step * static_cast<double>(stepper_steps);
    if (total_travel > full_travel) total_travel = full_travel;

    const int dir = compute_step_direction(stepper_start_angle,
                                           stepper_end_position);
    stepper_return_angle = stepper_start_angle;
    stepper_target_angle = stepper_start_angle + (static_cast<double>(dir) * total_travel);
    stepper_step_degrees = per_step;
    stepper_total_degrees = total_travel;
    stepper_tolerance = std::max(1.0, std::min(lever_deadband, per_step * 0.1));

    reset_lever_pid(stepper_target_angle);
    printf("stepper execute: balls=%d total_deg=%.2f target=%.2f start=%.2f\n",
           count, total_travel, stepper_target_angle, stepper_start_angle);
}

const char *Superstructure::state_to_string() const {
    switch (state) {
        case Idle:            return "Idle";
        case Intaking:        return "Intaking";
        case OuttakingTop:    return "OuttakingTop";
        case OuttakingMiddle: return "OuttakingMiddle";
        case OuttakingBottom: return "OuttakingBottom";
        case Stepping:        return "Stepping";
        case LeverHold:       return "LeverHold";
        case ManualLever:     return "ManualLever";

        default: return "Something has gone terrible wrong run for your lives";
    }
}

// === Background Stuff ===

int Superstructure::thread_fn(void *ptr) {
    Superstructure &self = *static_cast<Superstructure *>(ptr);

    while (true) {
        // === Stepper Timer ===
        if (self.stepper_count > 0 && !self.stepper_executing) {
            self.stepper_timer -= self.update_rate_ms;
            if (self.stepper_timer <= 0) {
                self.begin_stepper_execution(self.stepper_count,
                                             self.stepper_mode);
            }
        }

        // === State Machine ===
        // Defaults
        self.intake_motors.stop(vex::brakeType::coast);
        self.lever_motors.stop(vex::brakeType::brake);

        switch (self.state) {
        case Idle:
            // defaults
            break;

        case Intaking:
            self.intake_motors.spin(vex::forward,
                                    self.intake_volts,
                                    vex::volt);
            break;

        // OuttakingTop falls through to OuttakingMiddle
        // lever_target should be computed based on which state we are in
        case OuttakingTop:
        case OuttakingMiddle: {
            double lever_target =
                (self.state == OuttakingTop) ? self.lever_up_position_top :
                                               self.lever_up_position_middle;
            if (!self.lever_reached_up) {
                self.lever_motors.spin(vex::forward,
                                       self.lever_up_voltage,
                                       vex::volt);
                if (self.lever_at_target(lever_target)) {
                    self.lever_reached_up = true;
                }
            } else {
                self.lever_motors.spin(vex::reverse,
                                       self.lever_down_voltage,
                                       vex::volt);
                if (self.lever_at_down()) {
                    self.state = Idle;
                }
            }
            break;
        }

        case OuttakingBottom:
            self.intake_motors.spin(vex::reverse,
                                    self.outtake_voltage,
                                    vex::volt);
            break;

        case Stepping:
            if (!self.stepper_executing) {
                if (self.stepper_count <= 0) self.state = Idle;
                // default brake while waiting
                break;
            }

            if (self.stepper_up_phase) {
                self.apply_lever_voltage(
                    self.lever_pid_output(self.stepper_target_angle)
                );
                if (fabs(self.get_lever_angle() - self.stepper_target_angle)
                    <= self.stepper_tolerance) {
                    self.stepper_up_phase = false;
                    self.stepper_waiting_at_top = true;
                    self.stepper_delay_timer = self.step_delay_ms;
                    printf("stepper reached target\n");
                }
            } else if (self.stepper_waiting_at_top) {
                self.apply_lever_voltage(
                    self.lever_pid_output(self.stepper_target_angle)
                );
                self.stepper_delay_timer -= self.update_rate_ms;
                if (self.stepper_delay_timer <= 0) {
                    self.stepper_waiting_at_top = false;
                    self.stepper_returning = true;
                    self.reset_lever_pid(self.stepper_return_angle);
                    printf("stepper return\n");
                }
            } else if (self.stepper_returning) {
                self.apply_lever_voltage(
                    self.lever_pid_output(self.stepper_return_angle)
                );
                if (fabs(self.get_lever_angle() - self.stepper_return_angle)
                    <= self.stepper_tolerance) {
                    self.stepper_returning = false;
                    self.stepper_executing = false;
                    self.state = Idle;
                    printf("stepper complete\n");
                }
            } else {
                self.stepper_executing = false;
                self.state = Idle;
                printf("stepper aborted\n");
            }
            break;

        case LeverHold:
            self.apply_lever_voltage(
                self.lever_pid_output(self.lever_hold_target)
            );
            break;

        case ManualLever:
            self.apply_lever_voltage(self.manual_lever_volts);
            break;
        }

        vexDelay(self.update_rate_ms);
    }

    return 0;
}

// === Commands for auto ===

AutoCommand *Superstructure::IntakeCmd() {
    return new FunctionCommand([this]() {
        this->intake();
        return true;
    });
}

AutoCommand *Superstructure::IntakeCmd(double volts) {
    return new FunctionCommand([this, volts]() {
        this->intake(volts);
        return true;
    });
}

AutoCommand *Superstructure::OuttakeTopCmd() {
    return new FunctionCommand([this]() {
        this->outtake_top();
        return true;
    });
}

AutoCommand *Superstructure::OuttakeMiddleCmd() {
    return new FunctionCommand([this]() {
        this->outtake_middle();
        return true;
    });
}

AutoCommand *Superstructure::OuttakeBottomCmd() {
    return new FunctionCommand([this]() {
        this->outtake_bottom();
        return true;
    });
}

AutoCommand *Superstructure::StopCmd() {
    return new FunctionCommand([this]() {
        this->stop();
        return true;
    });
}

AutoCommand *Superstructure::LiftExtendCmd() {
    return new FunctionCommand([this]() {
        this->lift_extend();
        return true;
    });
}

AutoCommand *Superstructure::LiftRetractCmd() {
    return new FunctionCommand([this]() {
        this->lift_retract();
        return true;
    });
}

AutoCommand *Superstructure::StepperCmd(int count) {
    return new FunctionCommand([this, count]() {
        int c = std::min(count, this->stepper_steps);
        this->set_lift(false);
        this->begin_stepper_execution(c, StepperMiddle);
        return true;
    });
}