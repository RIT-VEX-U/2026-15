#include "competition/autonomous.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "subsystems/Intake.h"
#include "robot-config.h"
#include <vex_global.h>

void red_auto();
void skills();

void autonomous() {
  red_auto();
}

void red_auto() {
  CommandController cc {
    odom.SetPositionCmd(Pose2d(22.25, 55, 0)),

    // drive forward a bit
    drive_sys.DriveForwardCmd(13, vex::fwd, 0.5)->withTimeout(1),

    // turn to pickup two from line
    drive_sys.TurnToHeadingCmd(-46, 0.5)->withTimeout(1),
    drive_sys.DriveForwardCmd(36, vex::fwd, 0.3)->withTimeout(3),
    intake_sys.IntakeCmd(),

    // pick them up
    drive_sys.DriveForwardCmd(6, vex::fwd, 0.25)->withTimeout(3),
    new DelayCommand(1500),

    drive_sys.DriveForwardCmd(6, vex::reverse, 0.25)->withTimeout(3),

    // go back and lineup for center lower goal
    drive_sys.DriveForwardCmd(21, vex::reverse, 0.6)->withTimeout(2),
    drive_sys.TurnToHeadingCmd(45, 0.7)->withTimeout(1),

    // drive to and score into lower goal
    drive_sys.DriveForwardCmd(18, vex::forward, 0.5)->withTimeout(1),
    new DelayCommand(300),
    drive_sys.TurnToHeadingCmd(45, 0.7)->withTimeout(0.5),
    new DelayCommand(200),
    intake_sys.OutakeCmd(),
    new DelayCommand(600),
    intake_sys.IntakeStopCmd(),
    new DelayCommand(200),
    intake_sys.OutakeCmd(),
    new DelayCommand(600),
    intake_sys.IntakeStopCmd(),
    new DelayCommand(220),
    intake_sys.OutakeCmd(),
    new DelayCommand(3000),

    // go back and line up for matchload
    new Parallel({
      new InOrder({
        intake_sys.IntakeCmd(),
        new DelayCommand(750),
        intake_sys.IntakeStopCmd(),
      }),
      drive_sys.CurveToPointCmd({36, 24}, vex::reverse, 0.6)->withTimeout(3),
    }),
    drive_sys.TurnToHeadingCmd(180, 0.8)->withTimeout(1),

    // drop matchload mech
    new DelayCommand(500),
    intake_sys.MatchLoaderDownCmd(),
    intake_sys.OutakeCmd(),
    new DelayCommand(300),
    intake_sys.IntakeCmd(),
    drive_sys.TurnToHeadingCmd(180)->withTimeout(1),

    // slam matchload
    drive_sys.DriveForwardCmd(22, vex::forward, 0.35)->withTimeout(2),
    new DelayCommand(200),
    new DelayCommand(1000),

    // back away from matchload
    drive_sys.CurveToPointCmd({30, 21}, vex::reverse, 0.5)->withTimeout(2),
    intake_sys.MatchloaderUpCmd(),
    new DelayCommand(500),

    // turn and face goal
    drive_sys.TurnToHeadingCmd(0, 0.7)->withTimeout(1),
    intake_sys.IntakeUpCmd(),
    new DelayCommand(100),
    drive_sys.CurveToPointCmd({44, 21}, vex::forward, 0.2)->withTimeout(2),
    drive_sys.TurnToHeadingCmd(0)->withTimeout(1),

    // score
    intake_sys.IntakeScoreCmd(),
    new DelayCommand(3000),

    // climb
    new Parallel({
      new InOrder({
        intake_sys.IntakeStopCmd(),
        new DelayCommand(300),
        intake_sys.IntakeDownCmd(),
      }),
        drive_sys.CurveToPointCmd({12, 48}, vex::reverse, 0.6)->withTimeout(3),
      }),

    drive_sys.TurnToHeadingCmd(-90, 0.75)->withTimeout(1),
    drive_sys.DriveForwardCmd(20, vex::reverse, 0.45)->withTimeout(1),
  };

  cc.run();
}

void skills() {

}

