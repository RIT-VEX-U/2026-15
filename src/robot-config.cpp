#include "robot-config.h"
#include <cstdio>
#include <v5_apiuser.h>
#include <vex_triport.h>
#include <vex_units.h>

// VEX
vex::controller con;
vex::brain Brain;

// MOTORS

vex::motor left1(PORT8, vex::gearSetting::ratio6_1, false);
vex::motor left2(PORT9, vex::gearSetting::ratio6_1, true);
vex::motor left3(PORT6, vex::gearSetting::ratio6_1, false);
vex::motor left4(PORT5, vex::gearSetting::ratio6_1, true);
vex::motor left5(PORT4, vex::gearSetting::ratio6_1, false);
vex::motor_group left_motors(left1, left2, left3, left4, left5);

vex::motor right1(PORT20, vex::gearSetting::ratio6_1, false);
vex::motor right2(PORT19, vex::gearSetting::ratio6_1, true);
vex::motor right3(PORT18, vex::gearSetting::ratio6_1, false);
vex::motor right4(PORT17, vex::gearSetting::ratio6_1, true);
vex::motor right5(PORT16, vex::gearSetting::ratio6_1, true);
vex::motor_group right_motors(right1, right2, right3, right4, right5);

vex::motor toproller(PORT12, vex::gearSetting::ratio6_1, false);
vex::motor bottomroller(PORT13, vex::gearSetting::ratio6_1, false);

vex::optical intake_sensor(PORT10);
vex::optical outtake_sensor(PORT15);

vex::digital_out intake_sol(Brain.ThreeWirePort.H);
vex::digital_out match_loader_sol(Brain.ThreeWirePort.G);

vex::inertial imu(PORT1);

PID::pid_config_t drive_pid_cfg{
    .p = 0.6,
    .i = 0,
    .d = 0,
    .deadband = 0.5,
    .on_target_time = 0.1,
};

PID drive_pid(drive_pid_cfg);

robot_specs_t robot_config = {
    .robot_radius = 10,
    .odom_wheel_diam = 1.75,
    .odom_gear_ratio = 0.75,
    .dist_between_wheels = 12.4,
    .drive_feedback = &drive_pid,
};

// OdometryTank odom(left_motors, right_motors, robot_config, &imu);
TankDrive drive_sys(left_motors, right_motors, robot_config); // define how robot movesrobot

IntakeSys intake_sys(toproller, bottomroller, intake_sensor, outtake_sensor);

void robot_init() {
  // imu.calibrate();
  // while(imu.isCalibrating()){
  //    vexDelay(10);
  // }
  Brain.Screen.printAt(20, 20, "bing");
  printf("\n\n\nStarted\n\n\n");
  // odom.start();
  //.while (true) {
  // std::cout << odom.get_pose() << "\n";
  // vexDelay(10);
  //}
}
