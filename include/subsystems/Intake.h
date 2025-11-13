#pragma once
#include "vex.h"
#include <vex_motor.h>
#include <vex_optical.h>
#include <vex_task.h>
#include <vector>

class IntakeSys {
public:
  IntakeSys(vex::motor top_roller, vex::motor bottom_roller, vex::optical intake_sensor, vex::optical outtake_sensor);
  enum IntakeState {
    IN,
    SCORE,
    OUT,
    STOPPED,
  };

  enum BlockColor {
    BLUE,
    RED,
    ERROR,
  };

  IntakeState get_intake_state() { return intake_state; }

  void intake(double volts = 12);

  void outtake(double volts = 12);

  void intake_score(double volts = 12);

  void intake_stop();

  IntakeSys::BlockColor get_block_color(vex::optical sensor);


  void blockwatcher();

  static int thread_fn(void *tr);

private:
  bool saw_block_in = false;
  bool saw_block_out = false;
  int current_blocks = 0;
  std::vector<BlockColor> held_blocks;
  bool held_blocks_valid = true;
  vex::motor top_roller;
  vex::motor bottom_roller;
  vex::optical intake_sensor;
  vex::optical outtake_sensor;
  vex::task task;
  double intake_volts;
  IntakeState intake_state;
};
