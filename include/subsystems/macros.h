#pragma once

#include "core.h"
#include "vex.h"
#include "robot-config.h"

void score_upper_fast(double stop_angle_up = 97);
void score_upper(double stop_angle_up = 97);
void score_upper_fast_driver(double stop_angle_up = 97);
void score_upper_driver(double stop_angle_up = 97);
void score_upper_slow_driver(double stop_angle_up = 97);
void score_lower_driver(double stop_angle_up = 97);
void score_driver_stop();
bool score_driver_busy();
void score_upper_slow(double stop_angle_up = 97);
void score_lower();

AutoCommand *IntakeCmd();
AutoCommand *OuttakeCmd();
AutoCommand *IntakeStopCmd();

AutoCommand *WingUpCmd();
AutoCommand *WingDownCmd();

AutoCommand *LiftUpCmd();
AutoCommand *LiftDownCmd();

AutoCommand *LoadUpCmd();
AutoCommand *LoadDownCmd();

AutoCommand *HoodOpenCmd();
AutoCommand *HoodCloseCmd();

AutoCommand *ScoreUpperCmd(double stop_angle_up = 97);
AutoCommand *ScoreUpperSlowCmd(double stop_angle_up = 97);
AutoCommand *ScoreLowerCmd();

AutoCommand *DebugCmd();

AutoCommand *DriveTankRawCmd(double left, double right);
AutoCommand *BrakeDriveCmd(vex::brakeType  brake_type);
