#include "competition/autonomous.h"

#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
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
      drive_sys.DriveToPointCmd({19, 23}, vex::forward, 1)->withTimeout(2),
      IntakeCmd(),
      drive_sys.TurnToHeadingCmd(180, 1)->withTimeout(1),
      DriveTankRawCmd(0.4, 0.4),

      // only intake long enough for reds
      new DelayCommand(1300),
      new Parallel({
        drive_sys.DriveToPointCmd({44, 23.75}, vex::reverse, 1)->withTimeout(3),
        new InOrder({new DelayCommand(100), LoadUpCmd(), LiftDownCmd(), IntakeStopCmd()}),
      }),
      ScoreUpperCmd(),





    };
    cc.run();
}

void skills() {
    CommandController cc{

    };
    cc.run();
}
