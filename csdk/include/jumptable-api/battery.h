#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexBatteryVoltageGet, int32_t, (void), (), 0xa00)
JUMPTABLE_FUNC(vexBatteryCurrentGet, int32_t, (void), (), 0xa04)
JUMPTABLE_FUNC(vexBatteryTemperatureGet, double, (void), (), 0xa08)
JUMPTABLE_FUNC(vexBatteryCapacityGet, double, (void), (), 0xa0c)

#ifdef __cplusplus
}
#endif

