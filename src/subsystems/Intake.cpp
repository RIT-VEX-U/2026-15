#include "subsystems/Intake.h"
#include "core/utils/command_structure/auto_command.h"
#include "robot-config.h"
#include "core/utils/formatting.h"

#include <cstdio>
#include <vex_global.h>
#include <vex_optical.h>
#include <vex_units.h>
IntakeSys::IntakeSys(vex::motor top_roller, vex::motor bottom_roller,
                     vex::optical intake_sensor, vex::distance outtake_sensor)
    : top_roller(top_roller), bottom_roller(bottom_roller), intake_sensor(intake_sensor), outtake_sensor(outtake_sensor){
  task = vex::task(thread_fn, this);

  intake_sensor.setLightPower(100);
  intake_sensor.objectDetectThreshold(120);
};

void IntakeSys::intake(double volts) {
  this->intake_volts = volts;
  intake_state = IntakeState::IN;
}

void IntakeSys::outtake(double volts) {
  this->intake_volts = volts;
  intake_state = IntakeState::OUT;
}

void IntakeSys::intake_score(double volts) {
  intake_state = IntakeState::SCORE;
}

void IntakeSys::intake_stop() { intake_state = IntakeState::STOPPED; }

IntakeSys::BlockColor IntakeSys::get_block_color(vex::optical sensor){
  if(sensor.color() == red){
    return BlockColor::RED;
  }
  else if(sensor.color() == blue){
    return BlockColor::BLUE;
  }
  else{
    printf("ERROR: Color seen is not blue or red!\n");
    return BlockColor::ERROR;
  }
}

AutoCommand * IntakeSys::IntakeCmd() {
  return new FunctionCommand([&]() {
    this->intake();
    return true;
  });
}

AutoCommand * IntakeSys::IntakeStopCmd() {
  return new FunctionCommand([&]() {
    this->intake_stop();
    return true;
  });
}

AutoCommand * IntakeSys::IntakeScoreCmd() {
  return new FunctionCommand([&]() {
    this->intake_score();
    return true;
  });
}

AutoCommand * IntakeSys::OutakeCmd() {
  return new FunctionCommand([&]() {
    this->outtake(8);
    return true;
  });
}

AutoCommand * IntakeSys::IntakeUpCmd() {
  return new FunctionCommand([&]() {
    intake_sol.set(true);
    return true;
  });
}

AutoCommand * IntakeSys::IntakeDownCmd() {
  return new FunctionCommand([&]() {
    intake_sol.set(false);
    return true;
  });
}

AutoCommand * IntakeSys::MatchloaderUpCmd() {
  return new FunctionCommand([&]() {
    match_loader_sol.set(false);
    return true;
  });
}

AutoCommand * IntakeSys::MatchLoaderDownCmd() {
  return new FunctionCommand([&]() {
    match_loader_sol.set(true);
    return true;
  });
}

int IntakeSys::thread_fn(void *ptr) {
  IntakeSys &self = *(IntakeSys *)ptr;
  bool printed_blocks_invalid = false;

  while (true) {
    if(self.intake_sensor.isNearObject()){
      // printf("seeing object\n");
    } else{
      // printf("not seeing object\n");
    }
    if (self.intake_state == IntakeState::IN) {
      if (self.outtake_sensor.objectDistance(vex::distanceUnits::mm) < 30) {
        self.top_roller.stop();
      } else {
        self.top_roller.spin(vex::forward, self.intake_volts, vex::volt);
      }
      self.bottom_roller.spin(vex::reverse, self.intake_volts, vex::volt);
    } else if (self.intake_state == SCORE) {
      self.top_roller.spin(vex::forward, self.intake_volts, vex::volt);
      self.bottom_roller.spin(vex::reverse, self.intake_volts, vex::volt);
    } else if (self.intake_state == IntakeState::OUT) {
      self.top_roller.spin(vex::reverse, self.intake_volts, vex::volt);
      self.bottom_roller.spin(vex::forward, self.intake_volts, vex::volt);
    } else if (self.intake_state == IntakeState::STOPPED) {
      self.top_roller.stop();
      self.bottom_roller.stop();
    }
    // if(self.held_blocks_valid){
    // }
    // else if(!printed_blocks_invalid){
    //   printf("Error: held blocks are invalid\n");
    //   printed_blocks_invalid = true;
    // }

    vexDelay(10);
  }
  return 0;
}
