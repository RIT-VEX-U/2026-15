#pragma once

#include "core.h"
#include "vex.h"

class Drive {
public:
    Drive(TankDrive &drive_sys, vex::inertial &imu);
    
    void arcade(double forward, double turn);
    
    void tank(double left, double right);

    void stop();

    double heading() const;

    void reset_heading();

    TankDrive &get_drive_sys();

private:
    TankDrive &drive_sys;
    vex::inertial &imu;
};
