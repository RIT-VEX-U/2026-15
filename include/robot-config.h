#pragma once
#include "core/utils/controls/pid_tuner.h"

#include "core.h"
#include "vex.h"


// ================ INPUTS ================
// Digital sensors

// Analog sensors



// ================ OUTPUTS ================
// Motors

// Pneumatics



// ================ SUBSYSTEMS ================
extern TankDrive drive_sys;
extern OdometryTank odom;

// ================ UTILS ================
extern vex::controller con;

void robot_init();

