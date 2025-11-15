#include "competition/autonomous.h"
#include "robot-config.h"
#define PI 3.141592654

using namespace vex;

static constexpr double LOOKAHEAD =  10.0;

static void run_skills_route() {
    OdomSetPosition set_start(odom, Pose2d(0, 0, PI / 2));
    
    DriveForwardCommand color_line (
        drive_sys,
        *robot_config.drive_feedback,
        10.0,
        directionType::fwd,
        0.7,
        0.0
    );

    TurnDegreesCommand turn_match1 (
        drive_sys,
        *robot_config.turn_feedback,
        -45.0,
        0.6,
        0.0
    );

    DriveForwardCommand to_match1 (
        drive_sys,
        *robot_config.drive_feedback,
        24.0,
        directionType::fwd,
        0.7,
        0.0
    );

    TurnDegreesCommand turn_to_gate (
        drive_sys,
        *robot_config.turn_feedback,
        90.0,
        0.6,
        0.0
    );

    DriveForwardCommand gate (
        drive_sys,
        *robot_config.drive_feedback,
        36.0,
        directionType::fwd,
        0.8,
        0.0
    );

    TurnDegreesCommand turn_to_park (
        drive_sys,
        *robot_config.turn_feedback,
        -90.0,
        0.6,
        0.0
    );

    DriveForwardCommand park (
        drive_sys,
        *robot_config.drive_feedback,
        20.0,
        directionType::fwd,
        0.7,
        0.0
    );
    
    DriveStopCommand stop_drive(drive_sys);
    
    CommandController cc ({
        &set_start,
        &color_line,
        &turn_match1,
        &to_match1,
        &turn_to_gate,
        &gate,
        &turn_to_park,
        &park,                
        &stop_drive,
    });

    cc.printPathLogs = true;
    cc.run();
}



void autonomous() {
    run_skills_route();
}

void skills() {}

