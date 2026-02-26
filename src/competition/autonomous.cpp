#include "competition/autonomous.h"

#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/command_structure/drive_commands.h"
#include "core/utils/pure_pursuit.h"
#include "robot-config.h"
#include "subsystems/superstructure.h"

#include "subsystems/macros.h"

AutoCommand *DriveTankRawCmd(double left, double right) {
  return new FunctionCommand([left, right]() {
    drive_sys.drive_tank_raw(left, right);
    return true;
  });
}

void red_left();
void red_right();

void skills();

void autonomous() {
    red_right();
}

void red_left() {
    CommandController cc{
      LoadDownCmd(),
      drive_sys.DriveToPointCmd({19, 116}, vex::forward, 1)->withTimeout(2),
      IntakeCmd(),
      drive_sys.TurnToHeadingCmd(180, 1)->withTimeout(1),
      DriveTankRawCmd(0.4, 0.4),

      // only intake long enough for reds
      new DelayCommand(1300),
      new Parallel({
        drive_sys.DriveToPointCmd({44, 118.75}, vex::reverse, 1)->withTimeout(3),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftDownCmd(), IntakeStopCmd()}),
      }),
      ScoreUpperCmd(),





    };
    cc.run();
}



void red_right() {
    CommandController cc{
      LoadDownCmd(),
      drive_sys.DriveToPointCmd({22, 23.75}, vex::forward, 1)->withTimeout(2),
      IntakeCmd(),
      drive_sys.TurnToHeadingCmd(180, 1)->withTimeout(1),
      DriveTankRawCmd(0.4, 0.4),

      // only intake long enough for reds
      new DelayCommand(1300),
      (new Parallel({
        drive_sys.DriveToPointCmd({42, 23.75}, vex::reverse, 1)->withTimeout(2),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftUpCmd(), IntakeStopCmd()}),
      }))->withTimeout(1.5),
      ScoreUpperCmd(),
      new DelayCommand(100),

      // drive_sys.DriveForwardCmd(18, vex::forward, 1)->withTimeout(1),
      drive_sys.DriveToPointCmd({24, 23.75}, vex::forward, 1)->withTimeout(1),
      

      // DriveTankRawCmd(0.2, 0.4),
      // new DelayCommand(200),
      // DriveTankRawCmd(0.4, 0.2),
      // new DelayCommand(200),
      drive_sys.DriveToPointCmd({52, 34}, vex::reverse, 0.8)->withTimeout(2),
      drive_sys.DriveToPointCmd({61, 34}, vex::reverse, 1)->withTimeout(3),
      // drive_sys.PurePursuitCmd(PurePursuit::Path({
      //       Translation2d{38, 40},
      //       Translation2d{39, 40},
      //       Translation2d{40, 40},
      //       Translation2d{41, 40},
      //       Translation2d{42, 40},
      //       Translation2d{43, 40},
      //       Translation2d{44, 40},
      //       Translation2d{45, 40},
      //       Translation2d{46, 40},
      //       Translation2d{47, 40},
      //       Translation2d{48, 40},
      //       Translation2d{49, 40},
      //       Translation2d{50, 40},
      //       Translation2d{51, 40},
      //       Translation2d{52, 40},
      //       Translation2d{53, 40},
      //       Translation2d{54, 40},
      //       Translation2d{55, 40},
      //       Translation2d{56, 40},
      //       }, 9), vex::reverse, 1)->withTimeout(5),

      





    };
    cc.run();
}

void skills() {
    CommandController cc{

    };
    cc.run();
}
