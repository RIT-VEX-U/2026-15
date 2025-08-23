#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  uint8_t id;
  uint8_t red;
  uint8_t grn;
  uint8_t blu;
  float hangle;
  float hdsat;
  uint32_t reserved;
} V5_DeviceAiVisionColor;

typedef struct {
  uint16_t xoffset;
  uint16_t yoffset;
  uint16_t width;
  uint16_t height;
  uint16_t angle;
} V5_DeviceAiVisionColorData;

typedef struct {
  int16_t x0;
  int16_t y0;
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
  int16_t x3;
  int16_t y3;
} V5_DeviceAiVisionTagData;

typedef struct {
  uint16_t xoffset;
  uint16_t yoffset;
  uint16_t width;
  uint16_t height;
  uint16_t score;
} V5_DeviceAiVisionModelData;

typedef struct {
  V5_DeviceAiVisionColorData color;
  V5_DeviceAiVisionTagData tag;
  V5_DeviceAiVisionModelData model;
} V5_DeviceAiVisionObjectData;

typedef struct {
  uint8_t id;
  uint8_t r_type;
  V5_DeviceAiVisionObjectData object;
} V5_DeviceAiVisionObject;

typedef struct {
  uint8_t id;
  uint8_t len;
  int16_t c1;
  int16_t c2;
  int16_t c3;
  int16_t c4;
  int16_t c5;
  int16_t c6;
  int16_t c7;
} V5_DeviceAiVisionCode;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceAiVisionClassNameGet, int32_t, (V5_DeviceT device, int32_t id, uint8_t *pName), (device, id, pName), 0xcd4)
JUMPTABLE_FUNC(vexDeviceAiVisionCodeGet, bool, (V5_DeviceT device, uint32_t id, V5_DeviceAiVisionCode *pCode), (device, id, pCode), 0xcc4)
JUMPTABLE_FUNC(vexDeviceAiVisionCodeSet, void, (V5_DeviceT device, V5_DeviceAiVisionCode *pCode), (device, pCode), 0xcc0)
JUMPTABLE_FUNC(vexDeviceAiVisionColorGet, bool, (V5_DeviceT device, uint32_t id, V5_DeviceAiVisionColor *pColor), (device, id, pColor), 0xcbc)
JUMPTABLE_FUNC(vexDeviceAiVisionColorSet, void, (V5_DeviceT device, V5_DeviceAiVisionColor *pColor), (device, pColor), 0xcb8)
JUMPTABLE_FUNC(vexDeviceAiVisionModeGet, uint32_t, (V5_DeviceT device), (device), 0xcac)
JUMPTABLE_FUNC(vexDeviceAiVisionModeSet, void, (V5_DeviceT device, uint32_t mode), (device, mode), 0xca8)
JUMPTABLE_FUNC(vexDeviceAiVisionObjectCountGet, int32_t, (V5_DeviceT device), (device), 0xcb0)
JUMPTABLE_FUNC(vexDeviceAiVisionObjectGet, int32_t, (V5_DeviceT device, uint32_t indexObj, V5_DeviceAiVisionObject *pObject), (device, indexObj, pObject), 0xcb4)
JUMPTABLE_FUNC(vexDeviceAiVisionSensorSet, void, (V5_DeviceT device, double brightness, double contrast), (device, brightness, contrast), 0xcd8)
JUMPTABLE_FUNC(vexDeviceAiVisionStatusGet, uint32_t, (V5_DeviceT device), (device), 0xcc8)
JUMPTABLE_FUNC(vexDeviceAiVisionTemperatureGet, double, (V5_DeviceT device), (device), 0xccc)

#ifdef __cplusplus
}
#endif

