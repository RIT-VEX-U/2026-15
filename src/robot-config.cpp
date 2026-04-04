#include "robot-config.h"
#include <v5_api.h>
#include <v5_apiuser.h>
#include <vex_global.h>
#include <vex_units.h>
#include "core/subsystems/odometry/odometry_lidar_wrapper.h"
#include "subsystems/Lidar.h"
#include "subsystems/superstructure.h"
#include "core/utils/controls/state_space/ltv_differential_drive_controller.h"

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

PID::pid_config_t correction_pid_cfg{
  .p = 0.022, // 0.025
  .i = 0.00,
  .d = 0.002, // 0.004
  .deadband = 2,
  .on_target_time = 0.1,
  .error_method = PID::ERROR_TYPE::LINEAR,
};
PID::pid_config_t turn_pid_cfg{
  .p = 0.03, // 0.025
  .i = 0.00,
  .d = 0.002, // 0.004
  .deadband = 1,
  .on_target_time = 0.01,
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
    .correction_pid = correction_pid_cfg,
};


TankDriveModel drive_model(
  Length::from<inch_tag>(12),
  12_V,
  0.149114_VpInPs,
  0.018437_VpInPs2,
  1.088593_VpRadPs,
  0.100651_VpRadPs2
);

TankDriveObserver drive_observer(drive_model, 10_ms);

TankTrajectoryFollowerConfig trajectory_follower_config;

TankTrajectoryFollowerConfig line_cfg = [] {
  TankTrajectoryFollowerConfig cfg;
  cfg.q_tolerances = {{5, 1.5, 0.3, 20, 20}};
  cfg.r_tolerances = {{12, 12}};
  cfg.dt = 10_ms;
  cfg.max_velocity = 0_inps;
  cfg.velocity_step = 0.2_inps;
  cfg.stop_at_end = false;
  return cfg;
}();

uint64_t init_us;
struct pose_timestamp {
  uint64_t timestamp;
  float x;
  float y;
  float deg;
};

SerialLogger logger(vex::PORT12);
LidarReceiver lidar(vex::PORT5, 921600, &imu, &left_motors, &right_motors, &robot_config, &logger, &drive_observer);
OdometryLidarWrapper odom_impl(&lidar);
OdometryBase &odom = odom_impl;


TankDrive drive_sys(left_motors, right_motors, robot_config, &odom, &logger, &drive_model, &drive_observer);
Drive drive(drive_sys, imu);
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
Pose2d left_auto_pose(18, 89, from_degrees(90));
Pose2d curve_start(71.25, 95.0, from_degrees(0));
// Pose2d &auto_start_pose = left_auto_pose;
Pose2d &auto_start_pose = right_auto_pose;
// Pose2d &auto_start_pose = curve_start;

void robot_init() {
    // vexGenericSerialEnable(vex::PORT10, 0);
    // vexGenericSerialBaudrate(vex::PORT10, 9600);
    // uint8_t out = 0;
    // while (true) {
    //
    //   vexGenericSerialTransmit(vex::PORT10, &out, 1);
    //   out++;
    //   vexDelay(1000);
    // }
    // while (true) {
    //   printf("hi will\n");
    //   vexDelay(100);
    // }
    imu.calibrate();
    const uint64_t logger_connect_start_us = vexSystemHighResTimeGet();
    while (!logger.is_connected() && (vexSystemHighResTimeGet() - logger_connect_start_us) < 5000000) {
      logger.update();
    }

    while (imu.isCalibrating()) {
      vexDelay(10);
    }
    printf("IMU Calibrated\n");

    drive_observer.set_measurement_provider(
      [](units::Length &left_pos, units::Velocity &left_vel, units::Length &right_pos, units::Velocity &right_vel) {
        const double left_pos_in =
          (left_motors.position(vex::rotationUnits::rev) / robot_config.odom_gear_ratio) * M_PI * robot_config.odom_wheel_diam;
        const double right_pos_in = (right_motors.position(vex::rotationUnits::rev) / robot_config.odom_gear_ratio) * M_PI *
                                    robot_config.odom_wheel_diam;
        const double left_vel_inps = (left_motors.velocity(vex::velocityUnits::rpm) / 60.0 / robot_config.odom_gear_ratio) *
                                     M_PI * robot_config.odom_wheel_diam;
        const double right_vel_inps = (right_motors.velocity(vex::velocityUnits::rpm) / 60.0 / robot_config.odom_gear_ratio) *
                                      M_PI * robot_config.odom_wheel_diam;

        left_pos = units::Length::from<units::inch_tag>(left_pos_in);
        right_pos = units::Length::from<units::inch_tag>(right_pos_in);
        left_vel = units::Velocity::from<units::inches_per_second_tag>(left_vel_inps);
        right_vel = units::Velocity::from<units::inches_per_second_tag>(right_vel_inps);
      });
    drive_observer.start_async();
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
        {"right5", right_5},
        {"leverl", lever_motor_left},
        {"leverr", lever_motor_right},
      }),
      new screen::PIDPage(turn_pid, "turnpid"),
      new screen::PIDPage(drive_pid, "drivepid"),
      new screen::OdometryPage(odom, 15, 15, true),
    };
    screen::start_screen(Brain.Screen, pages);
    odom.set_position(auto_start_pose);
    Controller.Screen.print("started");
    printf("started\n");
    // superstructure.start();

    init_us = vexSystemHighResTimeGet();

    lidar.start();
    lidar.reset_ukf(auto_start_pose);
    bool logger_schema_sent = false;
    while (true) {
      if (!logger.is_connected()) {
        logger.update();
        logger_schema_sent = false;
        vexDelay(10);
        continue;
      }

      if (!logger_schema_sent) {
        logger.define_and_send_schema(0x01, "time:u64, x:f32, y:f32, t:f32");
        logger.define_and_send_schema(0x02, "time:u64, l:f32, r:f32");
        logger_schema_sent = true;
      }

      // lidar.resetUKF({48, 96, 180});
      Pose2d pose = drive_sys.get_position();
      uint64_t timestamp = vexSystemHighResTimeGet() - init_us;
      float x((float)pose.x());
      float y((float)pose.y());
      float t((float)pose.rotation().wrapped_degrees_360());
      float l((float)drive_sys.get_left_velocity());
      float r((float)drive_sys.get_right_velocity());
      logger.build(0x01)
         .add(timestamp)
         .add(x)
         .add(y)
         .add(t)
         .send();
      const double observer_data[3] = {(double)timestamp, (double)l, (double)r};
      logger.log(0x02, observer_data, 3);
      // printf("loop\n");
      vexDelay(10);
    }
}
