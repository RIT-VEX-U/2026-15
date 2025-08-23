#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kMagnetDurationShort = 0,
  kMagnetDurationMedium = 1,
  kMagnetDurationLong = 2,
  kMagnetDurationExtraLong = 3,
} V5_DeviceMagnetDuration;

JUMPTABLE_FUNC(vexDeviceMagnetPowerSet, void, (V5_DeviceT device, int32_t value, int32_t time), (device, value, time), 0x578)
JUMPTABLE_FUNC(vexDeviceMagnetPowerGet, int32_t, (V5_DeviceT device), (device), 0x57c)
JUMPTABLE_FUNC(vexDeviceMagnetPickup, void, (V5_DeviceT device, V5_DeviceMagnetDuration duration), (device, duration), 0x580)
JUMPTABLE_FUNC(vexDeviceMagnetDrop, void, (V5_DeviceT device, V5_DeviceMagnetDuration duration), (device, duration), 0x584)
JUMPTABLE_FUNC(vexDeviceMagnetTemperatureGet, double, (V5_DeviceT device), (device), 0x588)
JUMPTABLE_FUNC(vexDeviceMagnetCurrentGet, double, (V5_DeviceT device), (device), 0x58c)
JUMPTABLE_FUNC(vexDeviceMagnetStatusGet, uint32_t, (V5_DeviceT device), (device), 0x590)

#ifdef __cplusplus
}
#endif

