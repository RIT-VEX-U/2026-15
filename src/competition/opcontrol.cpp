#include "competition/opcontrol.h"

#include "robot-config.h"
#include <cstdio>

void testing();
bool enable_drive = true;
bool tank_drive = true;




void opcontrol() {
  con.ButtonLeft.pressed([](){
    tank_drive = !tank_drive;
  });
  con.ButtonR1.pressed([](){
    intake_sys.intake();
  });
  con.ButtonR2.pressed([](){
    intake_sys.intake_score();
  });
  con.ButtonL1.pressed([](){
;    intake_sys.outtake();
  });
  con.ButtonY.pressed([](){
    intake_sol.set(!intake_sol.value());
  });
  con.ButtonRight.pressed([](){
    match_loader_sol.set(!match_loader_sol.value());
  });

  con.ButtonA.pressed([](){
      CommandController cc{
      
      drive_sys.TurnDegreesCmd(120),
      };
      Brain.Screen.printAt(20, 20, "runned");
      enable_drive = false;
      cc.run();
      enable_drive = true;
  });
  while(true){
    double left;
    double right;
    if(!con.ButtonR1.pressing() && !con.ButtonL1.pressing() && !con.ButtonR2.pressing()){
      intake_sys.intake_stop();
    }
    if(tank_drive){
      left = (double)con.Axis3.position() / 100;
      right = (double)con.Axis2.position() / 100;
    }
    else{
      left = (double)con.Axis3.position() / 100;
      right = (double)con.Axis1.position() / 100;
    }
    if(left < 0.2 && left > -0.2){
      left = 0;
    }
    if(right < 0.2 && right > -0.2){
      right = 0;
    }
    if(enable_drive && tank_drive){
      drive_sys.drive_tank(left, right);
    }
    else if(enable_drive && !tank_drive){
      drive_sys.drive_arcade(left, right);
    }
  }
}
