#pragma once

#include "core.h"
#include "subsystems/Lidar.h"
#include "vex.h"

#include "core/utils/controls/state_space/ltv_differential_drive_controller.h"

#include "subsystems/Drive.h"
#include "subsystems/superstructure.h"

// === Hardware ===
extern vex::controller Controller;
extern vex::brain Brain;

// Drive motors
extern vex::motor_group left_motors;
extern vex::motor_group right_motors;
extern TankDrive drive_sys;

// Intake motors
extern vex::motor_group intake_motors;

// Lever motors and sensor
extern vex::motor_group lever_motors;
extern vex::rotation lever_rotation_sensor;

// Sensors
extern vex::inertial imu;

extern OdometryBase &odom;
extern LidarReceiver lidar;

// Lift pistons (uncomment when added)
extern vex::digital_out lift_sol;
extern vex::digital_out hood_sol;
extern vex::digital_out load_sol;
extern vex::digital_out wing_sol;

// === Subsystems ===
extern Drive drive;
extern Superstructure superstructure;

// === Configuration ===
extern robot_specs_t robot_config;
extern TankDriveModel drive_model;
extern TankDriveObserver drive_observer;
extern TankTrajectoryFollowerConfig trajectory_follower_config;
extern TankTrajectoryFollowerConfig line_cfg;

extern uint64_t init_us;

/**
 * Initialize the robot hardware and subsystems
 * Called once at startup before competition modes
 */
void robot_init();
