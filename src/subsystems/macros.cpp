#include "core.h"
#include "core/utils/command_structure/auto_command.h"
#include "vex.h"
#include "robot-config.h"

#include "subsystems/macros.h"
#include <v5_apiuser.h>

bool MACROING = false;

namespace {
bool score_driver_active = false;
bool score_driver_going_up = true;
bool score_driver_hood_close_pending = false;
uint64_t score_driver_hood_close_deadline_us = 0;

void finish_score_macro_with_delayed_hood_close(uint64_t hood_close_delay_us) {
    const double bottom_threshold = 2.0;
    const uint64_t deadline_us = vexSystemHighResTimeGet() + hood_close_delay_us;

    intake_motors.stop();

    while (vexSystemHighResTimeGet() < deadline_us) {
        const double angle = from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180();
        if (angle > bottom_threshold) {
            lever_motors.spin(vex::reverse, 12.0, vex::volt);
        } else {
            lever_motors.spin(vex::reverse, 1.0, vex::volt);
        }
        vexDelay(10);
    }

    hood_sol.set(false);
    lever_motors.spin(vex::reverse, 1.0, vex::volt);
}

void score_driver_step(double stop_angle_up, double up_voltage) {
    const double angle = from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180();
    const double bottom_threshold = 2.0;

    MACROING = true;
    score_driver_hood_close_pending = false;
    hood_sol.set(true);
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    if (!score_driver_active) {
        score_driver_active = true;
        score_driver_going_up = angle <= bottom_threshold;
    }

    if (angle >= stop_angle_up) {
        score_driver_going_up = false;
    } else if (angle <= bottom_threshold) {
        score_driver_going_up = true;
    }

    if (score_driver_going_up) {
        lever_motors.spin(vex::forward, up_voltage, vex::volt);
    } else {
        lever_motors.spin(vex::reverse, 12.0, vex::volt);
    }
}
}

void score_upper_fast(double stop_angle_up) {
    MACROING = true;
    uint64_t start_us = vexSystemHighResTimeGet();
    // open hood
    hood_sol.set(true);
    // run intake
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    // lever up
    lever_motors.spin(vex::forward, 12.0, vex::volt);
    while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < stop_angle_up) {
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

    finish_score_macro_with_delayed_hood_close(1000000);

    MACROING = false;
}

void score_upper(double stop_angle_up) {
    MACROING = true;
    uint64_t start_us = vexSystemHighResTimeGet();
    // open hood
    hood_sol.set(true);
    // run intake
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    // lever up
    lever_motors.spin(vex::forward, 8.0, vex::volt);
    while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < stop_angle_up) {
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

    finish_score_macro_with_delayed_hood_close(1000000);

    MACROING = false;
}

void score_upper_twice(double stop_angle_up) {
    MACROING = true;
    uint64_t start_us = vexSystemHighResTimeGet();
    // open hood
    hood_sol.set(true);
    // run intake
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    // lever up
    lever_motors.spin(vex::forward, 8.0, vex::volt);
    while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < stop_angle_up) {
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

    // lever up
    lever_motors.spin(vex::forward, 8.0, vex::volt);
    while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < stop_angle_up) {
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

    finish_score_macro_with_delayed_hood_close(1000000);

    MACROING = false;
}
void score_upper_driver(double stop_angle_up) {
    score_driver_step(stop_angle_up, 8.0);
}

void score_upper_fast_driver(double stop_angle_up) {
    score_driver_step(stop_angle_up, 12.0);
}

void score_upper_slow_driver(double stop_angle_up) {
    score_driver_step(stop_angle_up, 5.0);
}

void score_lower_driver(double stop_angle_up) {
    score_driver_step(stop_angle_up, 4.0);
}

void score_driver_stop() {
    const double angle = from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180();
    const double bottom_threshold = 2.0;

    if (!score_driver_active && !score_driver_hood_close_pending) {
        return;
    }

    if (score_driver_active) {
        score_driver_hood_close_pending = true;
        score_driver_hood_close_deadline_us = vexSystemHighResTimeGet() + 1000000;
    }

    score_driver_active = false;
    score_driver_going_up = true;

    if (score_driver_hood_close_pending && vexSystemHighResTimeGet() >= score_driver_hood_close_deadline_us) {
        hood_sol.set(false);
        score_driver_hood_close_pending = false;
    }

    if (score_driver_hood_close_pending && angle > bottom_threshold) {
        lever_motors.spin(vex::reverse, 12.0, vex::volt);
    } else {
        lever_motors.spin(vex::reverse, 1.0, vex::volt);
    }
    MACROING = false;
}

bool score_driver_busy() {
    return score_driver_active || score_driver_hood_close_pending;
}

void score_upper_slow(double stop_angle_up) {
    MACROING = true;
    uint64_t start_us = vexSystemHighResTimeGet();
    // open hood
    hood_sol.set(true);
    // run intake
    intake_motors.spin(vex::forward, 12.0, vex::volt);

    // lever up
    lever_motors.spin(vex::forward, 5.0, vex::volt);
    while (from_degrees(lever_rotation_sensor.position(vex::deg)).wrapped_degrees_180() < stop_angle_up) {
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
    

    finish_score_macro_with_delayed_hood_close(1000000);

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

    finish_score_macro_with_delayed_hood_close(1000000);

    MACROING = false;
}

AutoCommand *IntakeCmd() {
  return new FunctionCommand([]() {
      intake_motors.spin(vex::forward, 12, vex::volt);
      return true;
  });
}

AutoCommand *OuttakeCmd() {
  return new FunctionCommand([]() {
      intake_motors.spin(vex::forward, -12, vex::volt);
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
      hood_sol = true;
      return true;
  });
}
AutoCommand *HoodCloseCmd() {
  return new FunctionCommand([]() {
      hood_sol = false;
      return true;
  });
}

AutoCommand *ScoreUpperCmd(double stop_angle_up) {
  return (new FunctionCommand([stop_angle_up]() {
      score_upper(stop_angle_up);
      return true;
  }));
}

AutoCommand *ScoreUpperTwiceCmd(double stop_angle_up) {
  return (new FunctionCommand([stop_angle_up]() {
      score_upper_twice(stop_angle_up);
      return true;
  }));
}

AutoCommand *ScoreUpperSlowCmd(double stop_angle_up) {
  return (new FunctionCommand([stop_angle_up]() {
      score_upper_slow(stop_angle_up);
      return true;
  }));
}

AutoCommand *ScoreLowerCmd() {
  return (new FunctionCommand([]() {
      score_lower();
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
