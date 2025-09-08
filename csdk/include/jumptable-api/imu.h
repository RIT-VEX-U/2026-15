#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kImuOrientationZUp = 0x00,
  kImuOrientationZDown = 0x10,
  kImuOrientationXUp = 0x20,
  kImuOrientationXDown = 0x30,
  kImuOrientationYUp = 0x40,
  kImuOrientationYDown = 0x50,
  kImuOrientationAuto = 0x80,
} V5ImuOrientationMode;

#pragma pack(push,1)
typedef struct {
  double x;
  double y;
  double z;
  double w;
} V5_DeviceImuRaw;

typedef struct {
  double a;
  double b;
  double c;
  double d;
} V5_DeviceImuQuaternion;

typedef struct {
  double pitch;
  double roll;
  double yaw;
} V5_DeviceImuAttitude;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceImuReset, void, (V5_DeviceT device), (device), 0x410)
JUMPTABLE_FUNC(vexDeviceImuHeadingGet, double, (V5_DeviceT device), (device), 0x414)
JUMPTABLE_FUNC(vexDeviceImuDegreesGet, double, (V5_DeviceT device), (device), 0x418)
JUMPTABLE_FUNC(vexDeviceImuQuaternionGet, void, (V5_DeviceT device, V5_DeviceImuQuaternion *data), (device, data), 0x41c)
JUMPTABLE_FUNC(vexDeviceImuAttitudeGet, void, (V5_DeviceT device, V5_DeviceImuAttitude *data), (device, data), 0x420)
JUMPTABLE_FUNC(vexDeviceImuRawGyroGet, void, (V5_DeviceT device, V5_DeviceImuRaw *data), (device, data), 0x424)
JUMPTABLE_FUNC(vexDeviceImuRawAccelGet, void, (V5_DeviceT device, V5_DeviceImuRaw *data), (device, data), 0x428)
JUMPTABLE_FUNC(vexDeviceImuStatusGet, uint32_t, (V5_DeviceT device), (device), 0x42c)
JUMPTABLE_FUNC(vexDeviceImuModeSet, void, (V5_DeviceT device, uint32_t mode), (device, mode), 0x438)
JUMPTABLE_FUNC(vexDeviceImuModeGet, uint32_t, (V5_DeviceT device), (device), 0x43c)
JUMPTABLE_FUNC(vexDeviceImuDataRateSet, void, (V5_DeviceT device, uint32_t rate), (device, rate), 0x444)

#ifdef __cplusplus
}
#endif

