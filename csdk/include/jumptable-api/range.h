#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexDeviceRangeValueGet, int32_t, (V5_DeviceT device), (device), 0x4d8)

#ifdef __cplusplus
}
#endif

