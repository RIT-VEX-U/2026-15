#include "subsystems/Drive.h"

Drive::Drive(TankDrive &drive_sys, vex::inertial &imu)
    : drive_sys(drive_sys), imu(imu) {}

void Drive::arcade(double forward, double turn) {
    drive_sys.drive_arcade(forward, turn,
                           1, TankDrive::BrakeType::ZeroVelocity);
}

void Drive::tank(double left, double right) {
    drive_sys.drive_tank(left, right, 1, TankDrive::BrakeType::ZeroVelocity);
}

void Drive::stop() {
    drive_sys.stop();
}

double Drive::heading() const {
    return imu.heading();
}

void Drive::reset_heading() {
    imu.resetHeading();
}

TankDrive &Drive::get_drive_sys() {
    return drive_sys;
}
