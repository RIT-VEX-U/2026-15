#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kVisionModeNormal = 0,
  kVisionModeMixed = 1,
  kVisionModeLineDetect = 2,
  kVisionTypeTest = 3,
} V5VisionMode;

typedef enum {
  kVisionTypeNormal = 0,
  kVisionTypeColorCode = 1,
  kVisionTypeLineDetect = 2,
} V5VisionBlockType;

typedef enum {
  kVisionWBNormal = 0,
  kVisionWBStart = 1,
  kVisionWBManual = 2,
} V5VisionWBMode;

typedef enum {
  kVisionLedModeAuto = 0,
  kVisionLedModeManual = 1,
} V5VisionLedMode;

typedef enum {
  kVisionWifiModeOff = 0,
  kVisionWifiModeOn = 1,
} V5VisionWifiMode;

#pragma pack(push,1)
typedef struct {
  uint16_t signature;
  V5VisionBlockType r_type;
  uint16_t xoffset;
  uint16_t yoffset;
  uint16_t width;
  uint16_t height;
  uint16_t angle;
} V5_DeviceVisionObject;

typedef struct {
  uint8_t id;
  uint8_t flags;
  uint8_t pad[2];
  float range;
  int32_t uMin;
  int32_t uMax;
  int32_t uMean;
  int32_t vMin;
  int32_t vMax;
  int32_t vMean;
  uint32_t mRgb;
  uint32_t mType;
} V5_DeviceVisionSignature;

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t brightness;
} V5_DeviceVisionRgb;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceVisionModeSet, void, (V5_DeviceT device, V5VisionMode mode), (device, mode), 0x398)
JUMPTABLE_FUNC(vexDeviceVisionModeGet, V5VisionMode, (V5_DeviceT device), (device), 0x39c)
JUMPTABLE_FUNC(vexDeviceVisionObjectCountGet, int32_t, (V5_DeviceT device), (device), 0x3a0)
JUMPTABLE_FUNC(vexDeviceVisionObjectGet, int32_t, (V5_DeviceT device, uint32_t index, V5_DeviceVisionObject *object), (device, index, object), 0x3a4)
JUMPTABLE_FUNC(vexDeviceVisionSignatureSet, void, (V5_DeviceT device, V5_DeviceVisionSignature *signature), (device, signature), 0x3a8)
JUMPTABLE_FUNC(vexDeviceVisionSignatureGet, bool, (V5_DeviceT device, uint32_t id, V5_DeviceVisionSignature *signature), (device, id, signature), 0x3ac)
JUMPTABLE_FUNC(vexDeviceVisionBrightnessSet, void, (V5_DeviceT device, uint8_t percent), (device, percent), 0x3c0)
JUMPTABLE_FUNC(vexDeviceVisionBrightnessGet, uint8_t, (V5_DeviceT device), (device), 0x3c4)
JUMPTABLE_FUNC(vexDeviceVisionWhiteBalanceModeSet, void, (V5_DeviceT device, V5VisionWBMode mode), (device, mode), 0x3c8)
JUMPTABLE_FUNC(vexDeviceVisionWhiteBalanceModeGet, V5VisionWBMode, (V5_DeviceT device), (device), 0x3cc)
JUMPTABLE_FUNC(vexDeviceVisionWhiteBalanceSet, void, (V5_DeviceT device, V5_DeviceVisionRgb color), (device, color), 0x3c0)
JUMPTABLE_FUNC(vexDeviceVisionWhiteBalanceGet, V5_DeviceVisionRgb, (V5_DeviceT device), (device), 0x3c4)
JUMPTABLE_FUNC(vexDeviceVisionLedModeSet, void, (V5_DeviceT device, V5VisionLedMode mode), (device, mode), 0x3c8)
JUMPTABLE_FUNC(vexDeviceVisionLedModeGet, V5VisionLedMode, (V5_DeviceT device), (device), 0x3cc)
JUMPTABLE_FUNC(vexDeviceVisionLedBrigntnessSet, void, (V5_DeviceT device, uint8_t percent), (device, percent), 0x3d0)
JUMPTABLE_FUNC(vexDeviceVisionLedBrigntnessGet, uint8_t, (V5_DeviceT device), (device), 0x3d4)
JUMPTABLE_FUNC(vexDeviceVisionLedColorSet, void, (V5_DeviceT device, V5_DeviceVisionRgb color), (device, color), 0x3d8)
JUMPTABLE_FUNC(vexDeviceVisionLedColorGet, V5_DeviceVisionRgb, (V5_DeviceT device), (device), 0x3dc)
JUMPTABLE_FUNC(vexDeviceVisionWifiModeSet, void, (V5_DeviceT device, V5VisionWifiMode mode), (device, mode), 0x3e0)
JUMPTABLE_FUNC(vexDeviceVisionWifiModeGet, V5VisionWifiMode, (V5_DeviceT device), (device), 0x3e4)

#ifdef __cplusplus
}
#endif

