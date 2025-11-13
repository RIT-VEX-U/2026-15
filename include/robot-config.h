#pragma once

#include "core.h"
#include "subsystems/Intake.h"
#include "vex.h"
#include <vex_brain.h>
#include <vex_optical.h>
#include <vex_triport.h>

extern vex::controller con;
extern vex::brain Brain;

//MOTORS

extern robot_specs_t robot_config;
extern OdometryTank odom;
extern TankDrive drive_sys; //define how robot moves
extern IntakeSys intake_sys;
// ================ INPUTS ================
// Digital sensors

// Analog sensors

// ================ OUTPUTS ================
// Motors

// Pneumatics
extern vex::digital_out intake_sol;
extern vex::digital_out match_loader_sol;
extern vex::optical intake_sensor;


// ================ SUBSYSTEMS ================

// ================ UTILS ================

void robot_init();

