#include "competition/autonomous.h"

#include "core/utils/command_structure/command_controller.h"
#include "robot-config.h"
#include "subsystems/superstructure.h"

#include "subsystems/macros.h"


void red_left();
void red_right();

void blue_left();
void blue_right();

void skills();

void autonomous() {
    red_left();
}

void red_left() {
    CommandController cc{

    };
    cc.run();
}

void red_right() {
    CommandController cc{

    };
    cc.run();
}

void blue_left() {
    CommandController cc{

    };
    cc.run();
}

void blue_right() {
    CommandController cc{

    };
    cc.run();
}

void skills() {
    CommandController cc{

    };
    cc.run();
}
