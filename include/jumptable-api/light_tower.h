#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexDeviceLightTowerBlinkSet, void, (V5_DeviceT device, uint8_t select, uint8_t mask, int32_t onTime, int32_t offTime), (device, select, mask, onTime, offTime), 0x5b8)
JUMPTABLE_FUNC(vexDeviceLightTowerColorSet, void, (V5_DeviceT device, uint32_t color_id, uint32_t value), (device, color_id, value), 0x5a4)
JUMPTABLE_FUNC(vexDeviceLightTowerRgbGet, uint32_t, (V5_DeviceT device), (device), 0x5a8)
JUMPTABLE_FUNC(vexDeviceLightTowerRgbSet, void, (V5_DeviceT device, uint32_t rgb_value, uint32_t xyw_value), (device, rgb_value, xyw_value), 0x5a0)
JUMPTABLE_FUNC(vexDeviceLightTowerStatusGet, uint32_t, (V5_DeviceT device), (device), 0x5b0)
JUMPTABLE_FUNC(vexDeviceLightTowerDebugGet, uint32_t, (V5_DeviceT device, int32_t id), (device, id), 0x5b4)
JUMPTABLE_FUNC(vexDeviceLightTowerXywGet, uint32_t, (V5_DeviceT device), (device), 0x5ac)

#ifdef __cplusplus
}
#endif

