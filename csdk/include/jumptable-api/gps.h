#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  double x;
  double y;
  double z;
  double w;
} V5_DeviceGpsRaw;

typedef struct {
  double pitch;
  double roll;
  double yaw;

  double position_x;
  double position_y;
  double position_z;

  double az;
  double el;
  double rot;
} V5_DeviceGpsAttitude;

typedef struct {
  double x;
  double y;
  double z;
  double w;
} V5_DeviceGpsQuaternion;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceGpsReset, void, (V5_DeviceT device), (device), 0x5c8)
JUMPTABLE_FUNC(vexDeviceGpsHeadingGet, double, (V5_DeviceT device), (device), 0x5cc)
JUMPTABLE_FUNC(vexDeviceGpsDegreesGet, double, (V5_DeviceT device), (device), 0x5d0)
JUMPTABLE_FUNC(vexDeviceGpsQuaternionGet, void, (V5_DeviceT device, V5_DeviceGpsQuaternion *data), (device, data), 0x5d4)
JUMPTABLE_FUNC(vexDeviceGpsAttitudeGet, void, (V5_DeviceT device, V5_DeviceGpsAttitude *data, bool bRaw), (device, data, bRaw), 0x5d8)
JUMPTABLE_FUNC(vexDeviceGpsRawGyroGet, void, (V5_DeviceT device, V5_DeviceGpsRaw *data), (device, data), 0x5dc)
JUMPTABLE_FUNC(vexDeviceGpsRawAccelGet, void, (V5_DeviceT device, V5_DeviceGpsRaw *data), (device, data), 0x5e0)
JUMPTABLE_FUNC(vexDeviceGpsStatusGet, uint32_t, (V5_DeviceT device), (device), 0x5e4)
JUMPTABLE_FUNC(vexDeviceGpsModeSet, void, (V5_DeviceT device, uint32_t mode), (device, mode), 0x5f0)
JUMPTABLE_FUNC(vexDeviceGpsModeGet, uint32_t, (V5_DeviceT device), (device), 0x5f4)
JUMPTABLE_FUNC(vexDeviceGpsDataRateSet, void, (V5_DeviceT device, uint32_t rate), (device, rate), 0x5f8)
JUMPTABLE_FUNC(vexDeviceGpsOriginSet, void, (V5_DeviceT device, double ox, double oy), (device, ox, oy), 0x5fc)
JUMPTABLE_FUNC(vexDeviceGpsOriginGet, void, (V5_DeviceT device, double *ox, double *oy), (device, ox, oy), 0x600)
JUMPTABLE_FUNC(vexDeviceGpsRotationSet, void, (V5_DeviceT device, double value), (device, value), 0x604)
JUMPTABLE_FUNC(vexDeviceGpsRotationGet, double, (V5_DeviceT device), (device), 0x608)
JUMPTABLE_FUNC(vexDeviceGpsInitialPositionSet, void, (V5_DeviceT device, double initial_x, double initial_y, double initial_rotation), (device, initial_x, initial_y, initial_rotation), 0x60c)
JUMPTABLE_FUNC(vexDeviceGpsErrorGet, double, (V5_DeviceT device), (device), 0x614)

#ifdef __cplusplus
}
#endif

