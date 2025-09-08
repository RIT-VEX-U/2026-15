#pragma once

#include <stdint.h>
#include "jumptable.h" 
#include "device.h"

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexDeviceDistanceDistanceGet, uint32_t, (V5_DeviceT device), (device), 0x500)
JUMPTABLE_FUNC(vexDeviceDistanceConfidenceGet, uint32_t, (V5_DeviceT device), (device), 0x504)
JUMPTABLE_FUNC(vexDeviceDistanceStatusGet, uint32_t, (V5_DeviceT device), (device), 0x508)
JUMPTABLE_FUNC(vexDeviceDistanceObjectSizeGet, int32_t, (V5_DeviceT device), (device), 0x518)
JUMPTABLE_FUNC(vexDeviceDistanceObjectVelocityGet, double, (V5_DeviceT device), (device), 0x51c)

#ifdef __cplusplus
}
#endif

