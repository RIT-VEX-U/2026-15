#include "competition/autonomous.h"
#include "robot-config.h"

void autonomous() {}

void skills() {}


#include "competition/autonomous.h"

#include "robot-config.h"
#include "subsystems/superstructure.h"

namespace {
    /**
     * test atuo
     */
    void auto_test() {
        CommandController cmd({
            // Drive forward 24 inches
            drive_sys.DriveForwardCmd(24.0, vex::forward, 0.5),
            
            // Turn 90 degrees
            drive_sys.TurnDegreesCmd(90.0, 0.5),
            
            // Drive forward again
            drive_sys.DriveForwardCmd(24.0, vex::forward, 0.5),
            
            // Turn back
            drive_sys.TurnDegreesCmd(-90.0, 0.5),
        });
        
        cmd.run();
    }
}

void autonomous() {
    printf("Autonomous start\n");

    while (imu.isCalibrating()) {
        vexDelay(10);
    }
    
    auto_test();
    
    printf("Autonomous finish\n");
}

void skills() {
    printf("Skills start\n");
    
    CommandController cmd({
        superstructure.IntakeCmd(),
        drive_sys.DriveForwardCmd(36.0, vex::forward, 0.8),
    });
    
    cmd.run();
    
    printf("Skills finish\n");
}