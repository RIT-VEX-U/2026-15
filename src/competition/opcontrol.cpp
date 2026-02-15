#include "competition/opcontrol.h"

#include "robot-config.h"
#include "subsystems/superstructure.h"
#include <cmath>

namespace {
    constexpr double JOYSTICK_DEADBAND = 0.05;
    constexpr double LEVER_HOLD_TARGET_DEG = 300.0;

    double apply_deadband(double value) {
        return std::abs(value) < JOYSTICK_DEADBAND ? 0.0 : value;
    }

    double get_axis(const vex::controller::axis &axis) {
        return apply_deadband(axis.position() / 100.0);
    }

    struct ButtonState {
        bool pressed = false;
        bool just_pressed = false;
        bool just_released = false;
    };

    ButtonState r1, r2, l1, l2;
    ButtonState a, b, x, y;
    ButtonState up, down, left_btn, right_btn;

    bool drive_reversed = false;

    void update_button(ButtonState &state, bool current) {
        state.just_pressed = current && !state.pressed;
        state.just_released = !current && state.pressed;
        state.pressed = current;
    }

    void update_buttons() {
        update_button(r1, Controller.ButtonR1.pressing());
        update_button(r2, Controller.ButtonR2.pressing());
        update_button(l1, Controller.ButtonL1.pressing());
        update_button(l2, Controller.ButtonL2.pressing());
        update_button(a, Controller.ButtonA.pressing());
        update_button(b, Controller.ButtonB.pressing());
        update_button(x, Controller.ButtonX.pressing());
        update_button(y, Controller.ButtonY.pressing());
        update_button(up, Controller.ButtonUp.pressing());
        update_button(down, Controller.ButtonDown.pressing());
        update_button(left_btn, Controller.ButtonLeft.pressing());
        update_button(right_btn, Controller.ButtonRight.pressing());
    }
}




/**
 * Left Stick Y: Forward/Backward
 * Left Stick X: Turn
 * 
 * R1 (hold): Intake
 * R2 (hold): Outtake bottom
 * L1 (press): Outtake top
 * L2 (press): Outtake middle
 * 
 * TODO: ts still dont work gng idk why
 * A (press): Middle stepper - press 1-6 times within 500ms
 * X (press): Top stepper - press 1-6 times within 500ms
 * 
 * Up (hold): Lever up but manual - overrides stuff
 * Down (hold): Lever down but manual - overrides stuff
 * Left (press): Hold lever at LEVER_HOLD_TARGET_DEG (PD tuning) TODO: remove later
 * Right (press): Release lever hold TODO: remove later
 * 
 * Y (press): Toggle lift up and down
 *
 * B (press): Reverse drive toggle
 *
 * TODO: remove three-wire port tester after testing
 */
void opcontrol_normal() {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Opcontrol normal");

    Controller.ButtonY.pressed([]() {
        superstructure.wing_down();
    });
    Controller.ButtonY.released([]() {
        superstructure.wing_up();
    });

    Controller.ButtonRight.pressed([]() {
        superstructure.lift_toggle();
    });
    Controller.ButtonUp.pressed([]() {
        superstructure.load_toggle();
    });
    
    while (true) {
        update_buttons();

        // === Drive ===
        double fwd = get_axis(Controller.Axis3);
        double turn = get_axis(Controller.Axis1);
        if (drive_reversed) {
            fwd = -fwd;
        }
        drive.arcade(fwd, turn);

        // === Intake ===
        if (r1.just_pressed) {
            superstructure.intake();
        } else if (r1.just_released) {
            superstructure.stop();
        }

        // === Outtake Bottom ===
        if (r2.just_pressed) {
            superstructure.outtake_bottom();
        } else if (r2.just_released) {
            superstructure.stop();
        }

        // === Outtake Top ===
        if (l1.just_pressed) {
            superstructure.outtake_top();
        }

        // === Outtake Middle ===
        if (l2.just_pressed) {
            superstructure.outtake_middle();
        }

        // // === Stepper ===
        // if (a.just_pressed) {
        //     superstructure.stepper_press(Superstructure::StepperMiddle);
        // }
        // if (x.just_pressed) {
        //     superstructure.stepper_press(Superstructure::StepperTop);
        // }

        // === Reverse Drive Toggle ===
        if (b.just_pressed) {
            drive_reversed = !drive_reversed;
        }

        // === Console Prints ===
        static int display_tick = 0;
        if (++display_tick >= 10) {
            display_tick = 0;
            
            printf("State: %s, ",
                   superstructure.state_to_string());
            printf("Lever: %.1f deg, ",
                   superstructure.get_lever_angle());
            printf("Lift: %s",
                   superstructure.lift_is_up() ? "Extended, " : "Retracted, ");
            printf("Drive: %s",
                   drive_reversed ? "Reverse" : "Normal");
            printf("\n");
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
