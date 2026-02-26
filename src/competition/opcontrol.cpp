#include "competition/opcontrol.h"
#include <v5_api.h>
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "vex.h"

#include "core/subsystems/tank_drive.h"
#include "robot-config.h"
// #include "subsystems/superstructure.h"
#include <cmath>

#include "subsystems/macros.h"

bool drive_reversed = false;
bool intake_controllable = true;
bool enable_drive = true;

void opcontrol_normal() {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Opcontrol normal");

    Controller.ButtonA.pressed([]() {
        enable_drive = false;
        CommandController cc{
          // drive_sys.TurnDegreesCmd(170, 1)->withTimeout(2),
          drive_sys.DriveForwardCmd(48, vex::forward, 0.7)->withTimeout(5),
        };
        cc.run();
        enable_drive = true;
    });
    Controller.ButtonX.pressed([]() {
        enable_drive = false;
        CommandController cc{
          // drive_sys.TurnDegreesCmd(170, 1)->withTimeout(2),
          drive_sys.DriveForwardCmd(24, vex::reverse, 1)->withTimeout(5),
        };
        cc.run();
        enable_drive = true;
    });

    Controller.ButtonB.pressed([]() {
        drive_reversed = !drive_reversed;
    });

    Controller.ButtonY.pressed([]() {
        wing_sol = true;
    });
    Controller.ButtonY.released([]() {
        wing_sol = false;
    });

    Controller.ButtonRight.pressed([]() {
        lift_sol = !lift_sol;
    });
    Controller.ButtonUp.pressed([]() {
        load_sol = !load_sol;
    });

    Controller.ButtonL1.pressed([]() {
        intake_controllable = false;
        score_upper();
        intake_controllable = true;
    });
    Controller.ButtonL2.pressed([]() {
        intake_controllable = false;
        if (!lift_sol) {
            score_lower();
        } else {
            score_upper_slow();
        }
        intake_controllable = true;
    });
    
    while (true) {
        if (enable_drive) {
        // === Drive ===
        double forward = (double)Controller.Axis3.position() / 100.0;
        double turning = (double)Controller.Axis1.position() / 100.0;
        if (drive_reversed) {
            forward = -forward;
        }
        drive_sys.drive_arcade(forward, turning);

        if (intake_controllable) {
            if (Controller.ButtonR1.pressing()) {
                intake_motors.spin(vex::forward, 12.0, vex::volt);
            } else if (Controller.ButtonR2.pressing()) {
                intake_motors.spin(vex::reverse, 12.0, vex::volt);
            } else {
                intake_motors.stop();
            }
        }

        // === Console Prints ===
        static int display_tick = 0;
        // if (++display_tick >= 10) {
            display_tick = 0;
            
            // printf("State: %s, ",
            //        superstructure.state_to_string());
            // printf("Lever: %.1f deg, ",
            //        superstructure.get_lever_angle());
            // printf("Lift: %s",
            //        superstructure.lift_is_up() ? "Extended, " : "Retracted, ");
            // printf("Drive: %s",
            //        drive_reversed ? "Reverse" : "Normal");
            // printf("\n");
        // }
        }

        vexDelay(10);
    }
}

/**
 * Three wire port test (A->F B->G X->H)
 */
void opcontrol() {
  opcontrol_normal();
}
