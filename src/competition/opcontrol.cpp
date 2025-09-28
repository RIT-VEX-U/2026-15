#include "competition/opcontrol.h"
#include "robot-config.h"

void opcontrol() {
    con.ButtonA.pressed([](){
        printf("Attempting to move");
        odom.set_position({0,0,0});
        CommandController cc{
            drive_sys.DriveForwardCmd(24),
        };
        cc.run();
    });
}

