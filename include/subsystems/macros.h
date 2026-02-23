#pragma once

#include "core.h"
#include "vex.h"
#include "robot-config.h"

void score_upper();

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
