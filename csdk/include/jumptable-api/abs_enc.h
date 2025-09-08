#pragma once

#include <stdint.h>
#include "jumptable.h" 
#include "device.h"

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexDeviceAbsEncReset, void, (V5_DeviceT device), (device), 0x488)
JUMPTABLE_FUNC(vexDeviceAbsEncPositionSet, void, (V5_DeviceT device, int32_t position), (device, position), 0x48c)
JUMPTABLE_FUNC(vexDeviceAbsEncPositionGet, int32_t, (V5_DeviceT device), (device), 0x490)
JUMPTABLE_FUNC(vexDeviceAbsEncVelocityGet, int32_t, (V5_DeviceT device), (device), 0x494)
JUMPTABLE_FUNC(vexDeviceAbsEncAngleGet, int32_t, (V5_DeviceT device), (device), 0x498)
JUMPTABLE_FUNC(vexDeviceAbsEncReverseFlagSet, void, (V5_DeviceT device, bool value), (device, value), 0x49c)
JUMPTABLE_FUNC(vexDeviceAbsEncReverseFlagGet, bool, (V5_DeviceT device), (device), 0x4a0)
JUMPTABLE_FUNC(vexDeviceAbsEncStatusGet, uint32_t, (V5_DeviceT device), (device), 0x4a4)
JUMPTABLE_FUNC(vexDeviceAbsEncDataRateSet, void, (V5_DeviceT device, uint32_t rate), (device, rate), 0x4a8)

#ifdef __cplusplus
}
#endif

