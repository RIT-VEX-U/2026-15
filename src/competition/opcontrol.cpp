#include "competition/opcontrol.h"
#include <v5_api.h>
#include "vex.h"

#include "core/subsystems/tank_drive.h"
#include "robot-config.h"
// #include "subsystems/superstructure.h"
#include <cmath>

bool MACROING = false;
bool drive_reversed = false;


void opcontrol_normal() {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Opcontrol normal");

    Controller.ButtonB.pressed([]() {
        drive_reversed = !drive_reversed;
    });

    Controller.ButtonY.pressed([]() {
        wing_sol.set(true);
    });
    Controller.ButtonY.released([]() {
        wing_sol.set(false);
    });

    Controller.ButtonRight.pressed([]() {
        lift_sol = !lift_sol;
    });
    Controller.ButtonUp.pressed([]() {
        load_sol = !load_sol;
    });

    Controller.ButtonL1.pressed([]() {
        MACROING = true;
        uint64_t start_us = vexSystemHighResTimeGet();
        // open hood
        hood_sol.set(true);
        // run intake
        intake_motors.spin(vex::forward, 12.0, vex::volt);

        // lever up
        lever_motors.spin(vex::forward, 8.0, vex::volt);
        while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < 97.0) {
            if (vexSystemHighResTimeGet() - start_us > 1000000*0.75) {
                break;
            }
            vexDelay(10);
        }

        // lever down
        lever_motors.spin(vex::reverse, 12.0, vex::volt);
        while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() > 2) {
            if (vexSystemHighResTimeGet() - start_us > 1000000*1.5) {
                break;
            }
            vexDelay(10);
        }
        
        // close hood
        hood_sol.set(false);

        // hold lever down
        lever_motors.spin(vex::reverse, 1.0, vex::volt);
        intake_motors.stop();

        MACROING = false;
    });
    
    while (true) {
        // === Drive ===
        double forward = (double)Controller.Axis3.position() / 100.0;
        double turning = (double)Controller.Axis1.position() / 100.0;
        if (drive_reversed) {
            forward = -forward;
        }
        drive_sys.drive_arcade(forward, turning);

        if (!MACROING) {
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

        vexDelay(10);
    }
}

/**
 * Three wire port test (A->F B->G X->H)
 */
void opcontrol() {
  opcontrol_normal();
}
