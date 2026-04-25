#include "competition/opcontrol.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/trajectory/constraints/tank_voltage_constraint.h"
#include "core/utils/trajectory/constraints/centripetal_acceleration_constraint.h"
#include "core/utils/trajectory/trajectory.h"
#include "vex.h"

#include "core/subsystems/tank_drive.h"
#include "robot-config.h"
// #include "subsystems/superstructure.h"
#include <cmath>

#include "subsystems/macros.h"

#include "core/units/units.h"

bool drive_reversed = false;
bool intake_controllable = true;
bool enable_drive = true;
Trajectory make_example_curveo() {
  

  constexpr double radius = 24.0;
  constexpr double cx = 71.25;
  constexpr double cy = 71.0;
  constexpr double tangent_mag = 56;

  std::vector<HermitePoint> points = {
    HermitePoint::from_heading(cx, cy + radius, 0.0, tangent_mag),
    HermitePoint::from_heading(cx + radius, cy, -M_PI_2, tangent_mag),
    HermitePoint::from_heading(cx, cy - radius, M_PI, tangent_mag),
    HermitePoint::from_heading(cx - radius, cy, M_PI_2, tangent_mag),
    HermitePoint::from_heading(cx, cy + radius, 0.0, tangent_mag),
  };

  TrajectoryConfig config(90.000_inps, 90.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(true);
  config.set_track_width(12.000_in);
  config.add_constraint(CentripetalAccelerationConstraint(80.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.118581_VpInps, 0.016758_VpInps2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

void opcontrol_normal() {
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Opcontrol normal");

    // Controller.ButtonA.pressed([]() {
    //     enable_drive = false;
    //     CommandController cc{
    //       // drive_sys.TurnDegreesCmd(170, 1)->withTimeout(2),
    //       drive_sys.DriveForwardCmd(48, vex::forward, 0.7)->withTimeout(5),
    //     };
    //     cc.run();
    //     enable_drive = true;
    // });
    // Controller.ButtonX.pressed([]() {
    //     enable_drive = false;
    //     CommandController cc{
    //       // drive_sys.TurnDegreesCmd(170, 1)->withTimeout(2),
    //       drive_sys.DriveForwardCmd(24, vex::reverse, 1)->withTimeout(5),
    //     };
    //     cc.run();
    //     enable_drive = true;
    // });
    //
    Controller.ButtonA.pressed([]() {
        enable_drive = false;
        CommandController cc{
          drive_sys.FollowTrajectoryOpenLoopCmd(make_example_curveo()),
        };
        cc.run();
        enable_drive = true;
    });
    // Controller.ButtonX.pressed([]() {
    //     enable_drive = false;
    //     CommandController cc{
    //       drive_sys.FollowTrajectoryOpenLoopCmd(make_example_curveo()),
    //     };
    //     cc.run();
    //     enable_drive = true;
    // });

    Controller.ButtonB.pressed([]() {
        drive_reversed = !drive_reversed;
    });

    Controller.ButtonY.pressed([]() {
        if (lift_sol) {
            wing_sol = true;
        } else {
            wing_sol = false;
        }
    });
    Controller.ButtonY.released([]() {
        if (lift_sol) {
            wing_sol = false;
        } else {
            wing_sol = true;
        }
    });

    Controller.ButtonRight.pressed([]() {
        lift_sol = !lift_sol;
        if (!lift_sol) {
          wing_sol = false;
        } else {
          wing_sol = true;
        }
    });
    Controller.ButtonUp.pressed([]() {
        load_sol = !load_sol;
    });

    while (true) {
        const bool score_upper_fast_driver_held = Controller.ButtonLeft.pressing();
        const bool score_upper_driver_held = Controller.ButtonL1.pressing();
        const bool score_lower_driver_held = Controller.ButtonL2.pressing();
        intake_controllable = !(score_upper_fast_driver_held || score_upper_driver_held || score_lower_driver_held);

        if (enable_drive) {
        double forward = (double)Controller.Axis3.position() / 100.0;
        double turning = (double)Controller.Axis1.position() / 100.0;
        if (drive_reversed) {
            forward = -forward;
        }
        if (Controller.ButtonX.pressing()) {
          if (odom.get_position().y() < 23.75) {
            drive_sys.drive_line(forward, {11.75, 11.75}, from_degrees(0), line_cfg);
          } else if (odom.get_position().y() < 72) {
            drive_sys.drive_line(forward, {33.3, 33.3}, from_degrees(180), line_cfg);
          } else if (odom.get_position().y() < 118) {
            drive_sys.drive_line(forward, {108.5, 108.5}, from_degrees(0), line_cfg);
          } else {
            drive_sys.drive_line(forward, {129, 129}, from_degrees(180), line_cfg);
          }
        } else {

        drive_sys.drive_arcade(forward, turning);
        }

        if (intake_controllable) {
            if (Controller.ButtonR1.pressing()) {
                intake_motors.spin(vex::forward, 12.0, vex::volt);
            } else if (Controller.ButtonR2.pressing()) {
                intake_motors.spin(vex::reverse, 12.0, vex::volt);
            } else {
                intake_motors.stop();
            }
        }

        if (score_upper_fast_driver_held) {
            score_upper_fast_driver();
        } else if (score_upper_driver_held) {
            score_upper_driver();
        } else if (score_lower_driver_held) {
            if (!lift_sol) {
                score_lower_driver();
            } else {
                score_upper_slow_driver();
            }
        } else {
            score_driver_stop();
        }

        }

        vexDelay(10);
    }
}

/**
 * Three wire port test (A->F B->G X->H)
 */
void opcontrol() {
  left_motors.stop(vex::brakeType::coast);
  right_motors.stop(vex::brakeType::coast);
  wing_sol = false;
  lift_sol = true;

  opcontrol_normal();
}
