#pragma once

#include <stdint.h>
#include "jumptable.h"
#include "device.h"

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kAdiPortTypeAnalogIn = 0,
  kAdiPortTypeAnalogOut = 1,
  kAdiPortTypeDigitalIn = 2,
  kAdiPortTypeDigitalOut = 3,
  kAdiPortTypeSmartButton = 4,
  kAdiPortTypeSmartPot = 5,
  kAdiPortTypeLegacyButton = 6,
  kAdiPortTypeLegacyPotentiometer = 7,
  kAdiPortTypeLegacyLineSensor = 8,
  kAdiPortTypeLegacyLightSensor = 9,
  kAdiPortTypeLegacyGyro = 10,
  kAdiPortTypeLegacyAccelerometer = 11,
  kAdiPortTypeLegacyServo = 12,
  kAdiPortTypeLegacyPwm = 13,
  kAdiPortTypeQuadEncoder = 14,
  kAdiPortTypeSonar = 15,
  kAdiPortTypeLegacyPwmSlew = 16,
  kAdiPortTypeUndefined = 255,
} V5_AdiPortConfiguration;

typedef enum {
  kBumperReleased = 0,
  kBumperPressed = 1,
} V5_DeviceBumperState;

JUMPTABLE_FUNC(vexDeviceAdiPortConfigSet, void, (V5_DeviceT device, uint32_t port, V5_AdiPortConfiguration config), (device, port, config), 0x208)
JUMPTABLE_FUNC(vexDeviceAdiPortConfigGet, V5_AdiPortConfiguration, (V5_DeviceT device, uint32_t port), (device, port), 0x20c)
JUMPTABLE_FUNC(vexDeviceAdiValueSet, void, (V5_DeviceT device, uint32_t port, int32_t value), (device, port, value), 0x210)
JUMPTABLE_FUNC(vexDeviceAdiValueGet, int32_t, (V5_DeviceT device, uint32_t port), (device, port), 0x214)
JUMPTABLE_FUNC(vexDeviceAdiAddrLedSet, void, (V5_DeviceT device, uint32_t port, uint32_t *pData, uint32_t nOffset, uint32_t nLength, uint32_t options), (device, port, pData, nOffset, nLength, options), 0x21c)
JUMPTABLE_FUNC(vexDeviceBumperGet, V5_DeviceBumperState, (V5_DeviceT device), (device), 0x230)
JUMPTABLE_FUNC(vexDeviceGyroReset, void, (V5_DeviceT device), (device), 0x258)
JUMPTABLE_FUNC(vexDeviceGyroHeadingGet, double, (V5_DeviceT device), (device), 0x25c)
JUMPTABLE_FUNC(vexDeviceGyroDegreesGet, double, (V5_DeviceT device), (device), 0x260)
JUMPTABLE_FUNC(vexDeviceSonarValueGet, int32_t, (V5_DeviceT device), (device), 0x280)

#ifdef __cplusplus
}
#endif

