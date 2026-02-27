#pragma once

#include "core.h"
#include "vex.h"
#include "robot-config.h"

void score_upper();
void score_upper_slow();
void score_lower();

AutoCommand *IntakeCmd();
AutoCommand *IntakeStopCmd();

AutoCommand *WingUpCmd();
AutoCommand *WingDownCmd();

AutoCommand *LiftUpCmd();
AutoCommand *LiftDownCmd();

AutoCommand *LoadUpCmd();
AutoCommand *LoadDownCmd();

AutoCommand *HoodOpenCmd();
AutoCommand *HoodCloseCmd();

AutoCommand *ScoreUpperCmd();

AutoCommand *DebugCmd();

AutoCommand *DriveTankRawCmd(double left, double right);
AutoCommand *BrakeDriveCmd(vex::brakeType  brake_type);

