#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kLedColorBlack = 0x000000,
  kLedColorRed = 0xFF0000,
  kLedColorGreen = 0x00FF00,
  kLedColorBlue = 0x0000FF,
  kLedColorYellow = 0xFFFF00,
  kLedColorCyan = 0x00FFFF,
  kLedColorMagenta = 0xFF00FF,
  kLedColorWhite = 0xFFFFFF,
} V5_DeviceLedColor;

JUMPTABLE_FUNC(vexDeviceLedSet, void, (V5_DeviceT device, V5_DeviceLedColor value), (device, value), 0x1e0)
JUMPTABLE_FUNC(vexDeviceLedRgbSet, void, (V5_DeviceT device, uint32_t color), (device, color), 0x1e4)
JUMPTABLE_FUNC(vexDeviceLedGet, V5_DeviceLedColor, (V5_DeviceT device), (device), 0x1e8)
JUMPTABLE_FUNC(vexDeviceLedRgbGet, uint32_t, (V5_DeviceT device), (device), 0x1ec)

#ifdef __cplusplus
}
#endif

