#include "competition/autonomous.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "subsystems/Intake.h"
#include "robot-config.h"
#include <vex_global.h>

void autonomous() {
  CommandController cc {
    odom.SetPositionCmd(Pose2d(22.25, 55, 0)),

    // drive forward a bit
    drive_sys.DriveForwardCmd(13, vex::fwd, 0.6),

    // turn to pickup two from line
    drive_sys.TurnToHeadingCmd(-42, 0.5),
    drive_sys.DriveForwardCmd(38, vex::fwd, 0.6),
    intake_sys.IntakeCmd(),

    // pick them up
    drive_sys.DriveForwardCmd(5, vex::fwd, 0.3),
    new DelayCommand(1500),

    // go back and lineup for center lower goal
    drive_sys.DriveForwardCmd(28, vex::reverse, 0.5),
    drive_sys.TurnToHeadingCmd(45, 0.7),

    // drive to and score into lower goal
    drive_sys.DriveForwardCmd(15, vex::forward, 0.5),
    new DelayCommand(300),
    drive_sys.TurnToHeadingCmd(45, 0.7)->withTimeout(0.5),
    new DelayCommand(200),
    intake_sys.OutakeCmd(),
    new DelayCommand(3000),

    // go back and line up for matchload
    intake_sys.IntakeCmd(),
    drive_sys.DriveToPointCmd({36, 24}, vex::reverse, 0.5),
    drive_sys.TurnToHeadingCmd(180, 1),

    // drop matchload mech
    new DelayCommand(500),
    intake_sys.MatchLoaderDownCmd(),
    intake_sys.OutakeCmd(),
    new DelayCommand(300),
    intake_sys.IntakeCmd(),
    drive_sys.TurnToHeadingCmd(180),

    // slam matchload
    drive_sys.DriveForwardCmd(21, vex::forward, 0.5)->withTimeout(1.5),
    new DelayCommand(200),
    // odom.SetPositionCmd({odom.get_position().translation(), from_degrees(180)}),
    new DelayCommand(1000),
    // drive_sys.DriveForwardCmd(4, vex::reverse, 0.5),
    // new DelayCommand(300),
    // drive_sys.DriveForwardCmd(4, vex::forward, 0.7),
    // new DelayCommand(750),
    // drive_sys.DriveForwardCmd(4, vex::reverse, 0.5),
    // new DelayCommand(300),
    // drive_sys.DriveForwardCmd(4, vex::forward, 0.7),
    // new DelayCommand(750),
    // drive_sys.DriveForwardCmd(4, vex::reverse, 0.5),
    drive_sys.DriveToPointCmd({30, 23.75}, vex::reverse, 0.5),
    intake_sys.MatchloaderUpCmd(),
    new DelayCommand(500),
    drive_sys.TurnToHeadingCmd(0, 0.7),
    intake_sys.IntakeUpCmd(),
    new DelayCommand(100),
    drive_sys.DriveToPointCmd({44, 23.75}, vex::forward, 0.3),
    intake_sys.IntakeScoreCmd(),
    


  };

  cc.run();
}

void skills() {}

