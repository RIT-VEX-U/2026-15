#pragma once

#include "core/utils/controls/pid.h"
#include "vex.h"

class AutoCommand;

/**
 * States:
 * - Idle: Default state where everything is off
 * - Intaking: Intake motors on
 * - OuttakingTop: Lift extended then lever
 * - OuttakingMiddle: Lift retracted then lever
 * - OuttakingBottom: Lift extended intake motors reversed
 * - Stepping: Multiple ball outtake (1-6 balls based on button presses)
 * - ManualLever: Sets voltage of lever motors
 *
 * Lever:
 * - For OuttakingTop/Middle: Goes up then down
 * - For OuttakingBottom: Nothing, gravity
 */
class Superstructure {
public:
    enum State {
        Idle,
        Intaking,
        OuttakingTop,
        OuttakingMiddle,
        OuttakingBottom,
        Stepping,
        LeverHold,
        ManualLever
    };

    enum StepperMode {
        StepperMiddle,
        StepperTop
    };

    Superstructure(
        vex::motor_group &intake_motors,
        vex::motor_group &lever_motors,
        vex::rotation &lever_rotation,
        vex::digital_out *lift_sol,
        vex::digital_out *hood_sol,
        vex::digital_out *load_sol,
        vex::digital_out *wing_sol,
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
    );

    void start();

    void intake();
    void intake(double volts);
    void outtake_top();
    void outtake_middle();
    void outtake_bottom();
    void stop();

    void stepper_press(StepperMode mode);

    void hold_lever(double target_deg);
    void release_lever_hold();

    void manual_lever_up(double volts = 8.0);
    void manual_lever_down(double volts = 8.0);
    void manual_lever_stop();

    void lift_up();
    void lift_down();
    void lift_toggle();
    bool lift_is_up() const;

    void hood_open();
    void hood_close();
    void hood_toggle();
    bool hood_is_open() const;

    void load_up();
    void load_down();
    void load_toggle();
    bool load_is_down() const;

    void wing_up();
    void wing_down();
    void wing_toggle();
    bool wing_is_down() const;

    State get_state() const;
    double get_lever_angle() const;
    const char *state_to_string() const;

    AutoCommand *IntakeCmd();
    AutoCommand *IntakeCmd(double volts);

    AutoCommand *OuttakeTopCmd();
    AutoCommand *OuttakeMiddleCmd();
    AutoCommand *OuttakeBottomCmd();

    AutoCommand *StopCmd();

    AutoCommand *LiftExtendCmd();
    AutoCommand *LiftRetractCmd();
    AutoCommand *StepperCmd(int count);

    AutoCommand *WingDownCmd();
    AutoCommand *WingUpCmd();

    AutoCommand *MatchloadDownCmd();
    AutoCommand *MatchloadUpCmd();

private:
    vex::motor_group &intake_motors;
    vex::motor_group &lever_motors;
    vex::rotation &lever_rotation;
    vex::digital_out *lift_sol;
    vex::digital_out *hood_sol;
    vex::digital_out *load_sol;
    vex::digital_out *wing_sol;

    double intake_voltage;
    double outtake_voltage;
    double lever_up_voltage;
    double lever_down_voltage;
    double lever_up_position_top;
    double lever_up_position_middle;
    double lever_down_position;
    double lever_deadband;
    int stepper_steps;
    int stepper_window_ms;
    int step_delay_ms;
    int update_rate_ms;
    double lever_pd_kp;
    double lever_pd_kd;
    double lever_pid_deadband;
    bool lever_pid_inverted;

    State state = Idle;
    double intake_volts = 12.0;
    double manual_lever_volts = 0.0;

    bool lift_up_ = true;
    bool hood_open_ = true;
    bool load_down_ = false;
    bool wing_down_ = false;

    bool lever_reached_up = false;

    int stepper_count = 0;
    int stepper_timer = 0;
    bool stepper_executing = false;
    int stepper_balls_remaining = 0;
    int stepper_delay_timer = 0;
    double stepper_start_angle = 0.0;
    double stepper_target_angle = 0.0;
    double stepper_return_angle = 0.0;
    double stepper_total_degrees = 0.0;
    double stepper_step_degrees = 0.0;
    double stepper_tolerance = 0.0;
    double stepper_end_position = 0.0;
    StepperMode stepper_mode = StepperMiddle;
    bool stepper_up_phase = false;
    bool stepper_waiting_at_top = false;
    bool stepper_returning = false;

    double lever_hold_target = 0.0;
    PID::pid_config_t lever_pid_cfg = {0.0, 0.0, 0.0,
                                       1.0, 0.0,
                                       PID::ERROR_TYPE::LINEAR};
    PID lever_pid = PID(lever_pid_cfg);

    double compute_step_degrees(double start_position, double end_position) const;
    int compute_step_direction(double start_position, double end_position) const;

    vex::task *task = nullptr;
    bool task_started = false;
    static int thread_fn(void *ptr);

    void set_lift(bool up);
    void set_hood(bool open);
    void set_load(bool down);
    void set_wing(bool down);
    bool lever_at_target(double target) const;
    bool lever_at_down() const;

    void apply_lever_voltage(double voltage);
    void begin_stepper_execution(int count, StepperMode mode);

    double lever_pid_output(double target_deg);
    void reset_lever_pid(double target_deg);
};
