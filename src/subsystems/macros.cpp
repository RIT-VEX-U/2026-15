#include "core.h"
#include "core/utils/command_structure/auto_command.h"
#include "vex.h"
#include "robot-config.h"

#include "subsystems/macros.h"

bool MACROING = false;

void score_upper() {
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

    vexDelay(100);
    
    // close hood
    hood_sol.set(false);

    // hold lever down
    lever_motors.spin(vex::reverse, 1.0, vex::volt);
    intake_motors.stop();

    MACROING = false;
}

void score_upper_slow() {
    MACROING = true;
    uint64_t start_us = vexSystemHighResTimeGet();
    // open hood
    hood_sol.set(true);
    // run intake
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    // lever up
    lever_motors.spin(vex::forward, 5.0, vex::volt);
    while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < 97.0) {
        if (vexSystemHighResTimeGet() - start_us > 1000000*1) {
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
}

void score_lower() {
    MACROING = true;
    uint64_t start_us = vexSystemHighResTimeGet();
    // open hood
    hood_sol.set(true);
    // run intake
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    // lever up
    lever_motors.spin(vex::forward, 4.0, vex::volt);
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
}

AutoCommand *IntakeCmd() {
  return new FunctionCommand([]() {
      intake_motors.spin(vex::forward, 12, vex::volt);
      return true;
  });
}

AutoCommand *IntakeStopCmd() {
  return new FunctionCommand([]() {
      intake_motors.stop();
      return true;
  });
}

AutoCommand *WingUpCmd() {
  return new FunctionCommand([]() {
      wing_sol = false;
      return true;
  });
}
AutoCommand *WingDownCmd() {
  return new FunctionCommand([]() {
      wing_sol = true;
      return true;
  });
}

AutoCommand *LiftUpCmd() {
  return new FunctionCommand([]() {
      lift_sol = true;
      return true;
  });
}
AutoCommand *LiftDownCmd() {
  return new FunctionCommand([]() {
      lift_sol = false;
      return true;
  });
}

AutoCommand *LoadUpCmd() {
  return new FunctionCommand([]() {
      load_sol = false;
      return true;
  });
}
AutoCommand *LoadDownCmd() {
  return new FunctionCommand([]() {
      load_sol = true;
      return true;
  });
}

AutoCommand *HoodOpenCmd() {
  return new FunctionCommand([]() {
      hood_sol = false;
      return true;
  });
}
AutoCommand *HoodCloseCmd() {
  return new FunctionCommand([]() {
      hood_sol = true;
      return true;
  });
}

AutoCommand *ScoreUpperCmd() {
  return (new FunctionCommand([]() {
      score_upper();
      return true;
  }));
}

AutoCommand *DebugCmd() {
  return new Async(new FunctionCommand([]() {
        while (true) {
          printf("X: %0.03f, Y: %0.03f, T: %0.03f\n", odom.get_position().x(), odom.get_position().y(), odom.get_position().rotation().wrapped_degrees_360());
          vexDelay(100);
        }
        return true;
  }));
}

AutoCommand *DriveTankRawCmd(double left, double right) {
  return new FunctionCommand([left, right]() {
    drive_sys.drive_tank_raw(left, right);
    return true;
  });
}

AutoCommand *BrakeDriveCmd(vex::brakeType  brake_type) {
  return new FunctionCommand([brake_type]() {
    left_motors.stop(brake_type);
    right_motors.stop(brake_type);
    return true;
  });
}
