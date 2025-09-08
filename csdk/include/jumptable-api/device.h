#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#define V5_MAX_DEVICE_PORTS 32u

// a device plugged into a smart port
typedef void *V5_Device;

// handle to an opaque V5_Device
typedef V5_Device *V5_DeviceT;

typedef enum {
  kDeviceTypeNoSensor        = 0,
  kDeviceTypeMotorSensor     = 2,
  kDeviceTypeLedSensor       = 3,
  kDeviceTypeAbsEncSensor    = 4,
  kDeviceTypeCrMotorSensor   = 5,
  kDeviceTypeImuSensor       = 6,
  kDeviceTypeRangeSensor     = 7, // obsolete
  kDeviceTypeDistanceSensor  = 7,
  kDeviceTypeRadioSensor     = 8,
  kDeviceTypeTetherSensor    = 9,
  kDeviceTypeBrainSensor     = 10,
  kDeviceTypeVisionSensor    = 11,
  kDeviceTypeAdiSensor       = 12,
  kDeviceTypeRes1Sensor      = 13,
  kDeviceTypeRes2Sensor      = 14,
  kDeviceTypeRes3Sensor      = 15,
  kDeviceTypeOpticalSensor   = 16,
  kDeviceTypeMagnetSensor    = 17,
  kDeviceTypeGpsSensor       = 20,
  kDeviceTypeAicameraSensor  = 26,
  kDeviceTypeLightTowerSensor = 27,
  kDeviceTypeArmDevice       = 28,
  kDeviceTypeAiVisionSensor  = 29,
  kDeviceTypePneumaticSensor = 30,
  kDeviceTypeMC55MotorSensor = 31,
  kDeviceTypeBumperSensor    = 0x40,
  kDeviceTypeGyroSensor      = 0x46,
  kDeviceTypeSonarSensor     = 0x47,
  kDeviceTypeGenericSensor   = 128,
  kDeviceTypeGenericSerial   = 129,
  kDeviceTypeUndefinedSensor = 255
} V5_DeviceType;

JUMPTABLE_FUNC(vexDevicesGetNumber, uint32_t, (void), (), 0x190)
JUMPTABLE_FUNC(vexDevicesGetNumberByType, uint32_t, (V5_DeviceType type), (type), 0x194)
JUMPTABLE_FUNC(vexDevicesGet, V5_DeviceT, (void), (), 0x198)
JUMPTABLE_FUNC(vexDeviceGetByIndex, V5_DeviceT, (uint32_t index), (index), 0x19c)
JUMPTABLE_FUNC(vexDeviceGetStatus, int32_t, (V5_DeviceType *buffer), (buffer), 0x1a0)
JUMPTABLE_FUNC(vexDeviceGetTimestamp, int32_t, (V5_DeviceT device), (device), 0x1b0)
JUMPTABLE_FUNC(vexDeviceGenericValueGet, double, (V5_DeviceT device), (device), 0x2a8)
JUMPTABLE_FUNC(vexDeviceButtonStateGet, uint32_t, (void), (), 0x1b4)

#ifdef __cplusplus
}
#endif

