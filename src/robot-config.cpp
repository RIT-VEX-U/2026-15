#include "robot-config.h"
#include <v5_api.h>
#include "subsystems/superstructure.h"

// ========== Vex stuff ==========

vex::controller Controller;
vex::brain Brain;

// ========== Drive ==========
// Motors - Left (Back to front)
vex::motor left_1(PORT18, vex::gearSetting::ratio6_1, true);
vex::motor left_2(PORT17, vex::gearSetting::ratio6_1, false);
vex::motor left_3(PORT9, vex::gearSetting::ratio6_1, true);
vex::motor left_4(PORT7, vex::gearSetting::ratio6_1, false);
vex::motor left_5(PORT8, vex::gearSetting::ratio6_1, true);
vex::motor_group left_motors(left_1, left_2, left_3, left_4, left_5);

// Motors - Right (Back to front)
vex::motor right_1(PORT19, vex::gearSetting::ratio6_1, false);
vex::motor right_2(PORT16, vex::gearSetting::ratio6_1, true);
vex::motor right_3(PORT15, vex::gearSetting::ratio6_1, false);
vex::motor right_4(PORT13, vex::gearSetting::ratio6_1, true); 
vex::motor right_5(PORT11, vex::gearSetting::ratio6_1, false);
vex::motor_group right_motors(right_1, right_2, right_3, right_4, right_5);

// ========== Intake Roller ==========
vex::motor intake_motor_left(PORT10, vex::gearSetting::ratio6_1, true);
vex::motor intake_motor_right(PORT20, vex::gearSetting::ratio6_1, false);
vex::motor_group intake_motors(intake_motor_left, intake_motor_right);

// ========== Lever ==========
vex::motor lever_motor_left(PORT3, vex::gearSetting::ratio6_1, false);
vex::motor lever_motor_right(PORT1, vex::gearSetting::ratio6_1, true);
vex::motor_group lever_motors(lever_motor_left, lever_motor_right);
vex::rotation lever_rotation_sensor(PORT4, true);

// ========== IMU ==========
vex::inertial imu(PORT2);

// ========== Pistons ==========
vex::digital_out lift_sol(Brain.ThreeWirePort.G);
vex::digital_out hood_sol(Brain.ThreeWirePort.F);
vex::digital_out load_sol(Brain.ThreeWirePort.E);
vex::digital_out wing_sol(Brain.ThreeWirePort.H);

// ========= Robot Config ==========
robot_specs_t robot_config = {
    .robot_radius = 10,
    .dist_between_wheels = 12.0
};

// ========== Drive System ==========
TankDrive drive_sys(left_motors, right_motors, robot_config);
Drive drive(drive_sys, imu);

// ========== Superstructure ==========
const double SUPER_INTAKE_VOLTAGE = 12.0;
const double SUPER_OUTTAKE_VOLTAGE = 12.0;
const double SUPER_LEVER_UP_VOLTAGE = 10.0;
const double SUPER_LEVER_DOWN_VOLTAGE = 8.0;
const double SUPER_LEVER_UP_POSITION_TOP = 100;
const double SUPER_LEVER_UP_POSITION_MIDDLE = 50;
const double SUPER_LEVER_DOWN_POSITION = 0.0;
const double SUPER_LEVER_DEADBAND = 3.0;
const double SUPER_LEVER_HOLD_KP = 0.2;
const double SUPER_LEVER_HOLD_KD = 0.004;
const bool SUPER_LEVER_PID_INVERTED = false;
const double SUPER_LEVER_PID_DEADBAND = 2.0;
const int SUPER_STEPPER_STEPS = 6; // change as needed
const int SUPER_STEPPER_WINDOW_MS = 500;
const int SUPER_STEP_DELAY_MS = 300;
const int SUPER_UPDATE_RATE_MS = 10;

// Without lift pistons for now:
// Superstructure superstructure(
//     intake_motors,
//     lever_motors,
//     lever_rotation_sensor,
//     &lift_sol,
//     &hood_sol,
//     &load_sol,
//     &wing_sol,
//     SUPER_INTAKE_VOLTAGE,
//     SUPER_OUTTAKE_VOLTAGE,
//     SUPER_LEVER_UP_VOLTAGE,
//     SUPER_LEVER_DOWN_VOLTAGE,
//     SUPER_LEVER_UP_POSITION_TOP,
//     SUPER_LEVER_UP_POSITION_MIDDLE,
//     SUPER_LEVER_DOWN_POSITION,
//     SUPER_LEVER_DEADBAND,
//     SUPER_STEPPER_STEPS,
//     SUPER_STEPPER_WINDOW_MS,
//     SUPER_STEP_DELAY_MS,
//     SUPER_UPDATE_RATE_MS,
//     SUPER_LEVER_HOLD_KP,
//     SUPER_LEVER_HOLD_KD,
//     SUPER_LEVER_PID_DEADBAND,
//     SUPER_LEVER_PID_INVERTED
// );

// With lift pistons (uncomment when added):
// Superstructure superstructure(
//     intake_motors,
//     lever_motors,
//     lever_rotation_sensor,
//     &lift_piston_left,
//     &lift_piston_right,
//     SUPER_INTAKE_VOLTAGE,
//     SUPER_OUTTAKE_VOLTAGE,
//     SUPER_LEVER_UP_VOLTAGE,
//     SUPER_LEVER_DOWN_VOLTAGE,
//     SUPER_LEVER_UP_POSITION_TOP,
//     SUPER_LEVER_UP_POSITION_MIDDLE,
//     SUPER_LEVER_DOWN_POSITION,
//     SUPER_LEVER_DEADBAND,
//     SUPER_STEPPER_STEPS,
//     SUPER_STEPPER_WINDOW_MS,
//     SUPER_STEP_DELAY_MS,
//     SUPER_UPDATE_RATE_MS,
//     SUPER_LEVER_HOLD_KP,
//     SUPER_LEVER_HOLD_KD,
//     SUPER_LEVER_PID_DEADBAND,
//     SUPER_LEVER_PID_INVERTED
// );

uint64_t init_us;

void robot_init() {
    imu.calibrate();
    while (imu.isCalibrating()) {
      vexDelay(10);
    }
    printf("IMU Calibrated\n");
    // superstructure.start();

    init_us = vexSystemHighResTimeGet();

    printf("Robot initialized\n");
}
