#pragma once

#include "core.h"
#include "vex.h"

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

extern OdometryLidarWrapper odom;

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

extern uint64_t init_us;

/**
 * Initialize the robot hardware and subsystems
 * Called once at startup before competition modes
 */
void robot_init();
