#include "subsystems/Intake.h"
#include "robot-config.h"
#include "core/utils/formatting.h"

#include <cstdio>
#include <vex_global.h>
#include <vex_optical.h>
#include <vex_units.h>
IntakeSys::IntakeSys(vex::motor top_roller, vex::motor bottom_roller,
                     vex::optical intake_sensor, vex::optical outtake_sensor)
    : top_roller(top_roller), bottom_roller(bottom_roller), intake_sensor(intake_sensor), outtake_sensor(outtake_sensor){
  task = vex::task(thread_fn, this);

  intake_sensor.setLightPower(100);
  intake_sensor.objectDetectThreshold(120);
  
  outtake_sensor.setLightPower(100);
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

void IntakeSys::blockwatcher(){
  bool seeing_block_in = intake_sensor.isNearObject();
  bool seeing_block_out = outtake_sensor.isNearObject();
  if((intake_state == IN || intake_state == SCORE) && (seeing_block_in == false && saw_block_in == true)){
    current_blocks++;
    // held_blocks.push_back(get_block_color(intake_sensor));
    // printf("block went in! current blocks : %d\n", current_blocks);
  }
  else if((intake_state == OUT) && (seeing_block_in == false && saw_block_in == true)){
    current_blocks--;
    // if(held_blocks.size() < 1){
    //   held_blocks_valid = false;
    // } 
    // else{
    //   held_blocks.erase(held_blocks.begin());
    // }
    // printf("block went out! current blocks : %d\n", current_blocks);
  }
  if(intake_state == SCORE && (seeing_block_in == false && saw_block_in == true)){
    current_blocks--;
    // if(held_blocks.size() < 1){
    //   held_blocks_valid = false;
    // }
    // else{
    //  held_blocks.pop_back();
    // }
    // printf("block went out! current blocks : %d\n", current_blocks);
  }
  // if(held_blocks.size() > 8){
  //   held_blocks_valid = false;
  // }
  saw_block_in = seeing_block_in;
  saw_block_out = seeing_block_out;
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
      if (self.outtake_sensor.isNearObject()) {
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
    self.blockwatcher();
    // }
    // else if(!printed_blocks_invalid){
    //   printf("Error: held blocks are invalid\n");
    //   printed_blocks_invalid = true;
    // }

    vexDelay(10);
  }
  return 0;
}
