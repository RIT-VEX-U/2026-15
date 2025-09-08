#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  uint16_t clear;
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} V5_DeviceOpticalRaw;

typedef struct {
  double red;
  double green;
  double blue;
  double brightness;
} V5_DeviceOpticalRgb;

typedef struct {
  uint8_t udata;
  uint8_t ddata;
  uint8_t ldata;
  uint8_t rdata;
  uint8_t gesture_type;
  uint8_t padding;
  uint16_t count;
  uint32_t time;
} V5_DeviceOpticalGesture;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceOpticalHueGet, double, (V5_DeviceT device), (device), 0x528)
JUMPTABLE_FUNC(vexDeviceOpticalSatGet, double, (V5_DeviceT device), (device), 0x52c)
JUMPTABLE_FUNC(vexDeviceOpticalBrightnessGet, double, (V5_DeviceT device), (device), 0x530)
JUMPTABLE_FUNC(vexDeviceOpticalProximityGet, int32_t, (V5_DeviceT device), (device), 0x534)
JUMPTABLE_FUNC(vexDeviceOpticalRgbGet, void, (V5_DeviceT device, V5_DeviceOpticalRgb *data), (device, data), 0x538)
JUMPTABLE_FUNC(vexDeviceOpticalLedPwmSet, void, (V5_DeviceT device, int32_t value), (device, value), 0x53c)
JUMPTABLE_FUNC(vexDeviceOpticalLedPwmGet, int32_t, (V5_DeviceT device), (device), 0x540)
JUMPTABLE_FUNC(vexDeviceOpticalStatusGet, uint32_t, (V5_DeviceT device), (device), 0x544)
JUMPTABLE_FUNC(vexDeviceOpticalRawGet, void, (V5_DeviceT device, V5_DeviceOpticalRaw *data), (device, data), 0x548)
JUMPTABLE_FUNC(vexDeviceOpticalModeSet, void, (V5_DeviceT device, uint32_t mode), (device, mode), 0x550)
JUMPTABLE_FUNC(vexDeviceOpticalModeGet, uint32_t, (V5_DeviceT device), (device), 0x554)
JUMPTABLE_FUNC(vexDeviceOpticalGestureGet, uint32_t, (V5_DeviceT device, V5_DeviceOpticalGesture *pData), (device, pData), 0x558)
JUMPTABLE_FUNC(vexDeviceOpticalGestureEnable, void, (V5_DeviceT device), (device), 0x55c)
JUMPTABLE_FUNC(vexDeviceOpticalGestureDisable, void, (V5_DeviceT device), (device), 0x560)
JUMPTABLE_FUNC(vexDeviceOpticalProximityThreshold, void, (V5_DeviceT device, int32_t value), (device, value), 0x564)
JUMPTABLE_FUNC(vexDeviceOpticalIntegrationTimeSet, void, (V5_DeviceT device, double timeMs), (device, timeMs), 0xb40)
JUMPTABLE_FUNC(vexDeviceOpticalIntegrationTimeGet, double, (V5_DeviceT device), (device), 0xb44)

#ifdef __cplusplus
}
#endif

