#include "robot-config.h"
#include <v5_api.h>
#include <vex_units.h>
#include "core/subsystems/odometry/odometry_lidar_wrapper.h"
#include "subsystems/Lidar.h"
#include "subsystems/superstructure.h"

vex::controller Controller;
vex::brain Brain;

vex::motor left_1(PORT18, vex::gearSetting::ratio6_1, true);
vex::motor left_2(PORT17, vex::gearSetting::ratio6_1, false);
vex::motor left_3(PORT9, vex::gearSetting::ratio6_1, true);
vex::motor left_4(PORT7, vex::gearSetting::ratio6_1, false);
vex::motor left_5(PORT8, vex::gearSetting::ratio6_1, true);
vex::motor_group left_motors(left_1, left_2, left_3, left_4, left_5);

vex::motor right_1(PORT19, vex::gearSetting::ratio6_1, false);
vex::motor right_2(PORT16, vex::gearSetting::ratio6_1, true);
vex::motor right_3(PORT15, vex::gearSetting::ratio6_1, false);
vex::motor right_4(PORT13, vex::gearSetting::ratio6_1, true); 
vex::motor right_5(PORT11, vex::gearSetting::ratio6_1, false);
vex::motor_group right_motors(right_1, right_2, right_3, right_4, right_5);

vex::motor intake_motor_left(PORT10, vex::gearSetting::ratio6_1, true);
vex::motor intake_motor_right(PORT20, vex::gearSetting::ratio6_1, false);
vex::motor_group intake_motors(intake_motor_left, intake_motor_right);

vex::motor lever_motor_left(PORT3, vex::gearSetting::ratio6_1, false);
vex::motor lever_motor_right(PORT1, vex::gearSetting::ratio6_1, true);
vex::motor_group lever_motors(lever_motor_left, lever_motor_right);
vex::rotation lever_rotation_sensor(PORT4, true);

vex::inertial imu(PORT2);

vex::digital_out lift_sol(Brain.ThreeWirePort.G);
vex::digital_out hood_sol(Brain.ThreeWirePort.F);
vex::digital_out load_sol(Brain.ThreeWirePort.E);
vex::digital_out wing_sol(Brain.ThreeWirePort.H);


PID::pid_config_t drive_pid_cfg{
   .p = 0.03,
   .d = 0.002,
   .deadband = 1.5,
   .on_target_time = 0.1,
};
PID drive_pid(drive_pid_cfg);

PID::pid_config_t turn_pid_cfg{
  .p = 0.022, // 0.025
  .i = 0.001,
  .d = 0.0018, // 0.004
  .deadband = 2,
  .on_target_time = 0.1,
  .error_method = PID::ERROR_TYPE::LINEAR,
};
PID turn_pid(turn_pid_cfg);

robot_specs_t robot_config = {
    .robot_radius = 10,
    .odom_wheel_diam = 2.75,
    .odom_gear_ratio = 1,
    .dist_between_wheels = 12.0,
    .drive_correction_cutoff = 5,
    .drive_feedback = &drive_pid,
    .turn_feedback = &turn_pid,
    .correction_pid = turn_pid_cfg,
};


TankDrive drive_sys(left_motors, right_motors, robot_config, &odom);
Drive drive(drive_sys, imu);

// ========== Superstructure ==========
// const double SUPER_INTAKE_VOLTAGE = 12.0;
// const double SUPER_OUTTAKE_VOLTAGE = 12.0;
// const double SUPER_LEVER_UP_VOLTAGE = 10.0;
// const double SUPER_LEVER_DOWN_VOLTAGE = 8.0;
// const double SUPER_LEVER_UP_POSITION_TOP = 100;
// const double SUPER_LEVER_UP_POSITION_MIDDLE = 50;
// const double SUPER_LEVER_DOWN_POSITION = 0.0;
// const double SUPER_LEVER_DEADBAND = 3.0;
// const double SUPER_LEVER_HOLD_KP = 0.2;
// const double SUPER_LEVER_HOLD_KD = 0.004;
// const bool SUPER_LEVER_PID_INVERTED = false;
// const double SUPER_LEVER_PID_DEADBAND = 2.0;
// const int SUPER_STEPPER_STEPS = 6; // change as needed
// const int SUPER_STEPPER_WINDOW_MS = 500;
// const int SUPER_STEP_DELAY_MS = 300;
// const int SUPER_UPDATE_RATE_MS = 10;

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
struct pose_timestamp {
  uint64_t timestamp;
  float x;
  float y;
  float deg;
};

SerialLogger logger(vex::PORT12);
LidarReceiver lidar(vex::PORT5, 921600, &imu, &left_motors, &right_motors, &robot_config, &logger);
OdometryLidarWrapper odom(&lidar);

/*                 144
 * y /---------------\ 144
 *   |    90deg      |
 *   |      ^        |
 *   |    < o > 0deg |
 *   |      v        |
 *   |    270deg     |
 * 0 \_______________/
 *   0               x
*/
Pose2d right_auto_pose(18, 54, from_degrees(270));
Pose2d left_auto_pose(18, 88, from_degrees(90));
Pose2d &auto_start_pose = right_auto_pose;

void robot_init() {
    // while (true) {
    //   printf("hi will\n");
    //   vexDelay(100);
    // }
    imu.calibrate();
    while (!logger.is_connected() && (vexSystemHighResTimeGet() - init_us) < 5000000) {
      logger.update();
    }

    while (imu.isCalibrating()) {
      vexDelay(10);
    }
    printf("IMU Calibrated\n");
    double dt = 0.01;

    // SYSID!!!!!
    // LINEAR QUASISTATIC
      // for (int i = 0; i < 1200; i++) {
      //   drive_sys.drive_tank_raw(-i * dt / 12.0, i * dt / 12.0);
      //   printf("%0.03f, %0.03f, %0.03f\n", i*dt, i*dt, deg2rad(-imu.gyroRate(vex::axisType::zaxis, vex::dps)));
      //   vexDelay(10);
      // }

    // LINEAR DYNAMIC
    //   for (int i = 0; i < 300; i++) {
    //     drive_sys.drive_tank_raw(-10, 10);
    //     printf("%0.03f, %0.03f, %0.03f\n", i*dt, 10.0, deg2rad(-imu.gyroRate(vex::axisType::zaxis, vex::dps)));
    //     vexDelay(10);
    //   }
    //
    // while (true);

    std::vector<screen::Page *> pages = {
      new screen::StatsPage({
        {"left1", left_1},
        {"left2", left_2},
        {"left3", left_3},
        {"left4", left_4},
        {"left5", left_5},
        {"right1", right_1},
        {"right2", right_2},
        {"right3", right_3},
        {"right4", right_4},
        {"right5", right_5}
      }),
      new screen::PIDPage(turn_pid, "turnpid"),
      new screen::PIDPage(drive_pid, "drivepid"),
      new screen::OdometryPage(odom, 15, 15, true),
    };
    screen::start_screen(Brain.Screen, pages);
    Controller.Screen.print("started");
    odom.set_position(auto_start_pose);
    printf("started\n");
    // superstructure.start();

    init_us = vexSystemHighResTimeGet();

    lidar.start();
    lidar.reset_ukf(auto_start_pose);
    logger.define_and_send_schema(0x01, "time:u64, x:f32, y:f32, t:f32");
    while (true) {
      // lidar.resetUKF({48, 96, 180});
      Pose2d pose = drive_sys.get_position();
      uint64_t timestamp = vexSystemHighResTimeGet() - init_us;
      float x((float)pose.x());
      float y((float)pose.y());
      float t((float)pose.rotation().wrapped_degrees_360());
      logger.build(0x01)
         .add(timestamp)
         .add(x)
         .add(y)
         .add(t)
         .send();
      // printf("loop\n");
      vexDelay(10);
    }
}
