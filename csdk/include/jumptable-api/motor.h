#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kV5MotorBrakeModeCoast = 0,
  kV5MotorBrakeModeBrake = 1,
  kV5MotorBrakeModeHold = 2,
} V5MotorBrakeMode;

typedef enum {
  kMotorControlModeOFF = 0,
  kMotorControlModeBRAKE = 1,
  kMotorControlModeHOLD = 2,
  kMotorControlModeSERVO = 3,
  kMotorControlModePROFILE = 4,
  kMotorControlModeVELOCITY = 5,
  kMotorControlModeUNDEFINED = 6,
} V5MotorControlMode;

typedef enum {
  kMotorEncoderDegrees = 0,
  kMotorEncoderRotations = 1,
  kMotorEncoderCounts = 2,
} V5MotorEncoderUnits;

typedef enum {
  kMotorGearSet_36 = 0,
  kMotorGearSet_18 = 1,
  kMotorGearSet_06 = 2,
} V5MotorGearset;

#pragma pack(push,1)
typedef struct {
  uint8_t kf;
  uint8_t kp;
  uint8_t ki;
  uint8_t kd;
  uint8_t filter;
  uint8_t pad1;
  uint16_t limit;
  uint8_t threshold;
  uint8_t loopspeed;
  uint8_t pad2[2];
} V5_DeviceMotorPid;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceMotorVelocitySet, void, (V5_DeviceT device, int32_t velocity), (device, velocity), 0x2d0)
JUMPTABLE_FUNC(vexDeviceMotorVelocityGet, int32_t, (V5_DeviceT device), (device), 0x2d4)
JUMPTABLE_FUNC(vexDeviceMotorActualVelocityGet, double, (V5_DeviceT device), (device), 0x2d8)
JUMPTABLE_FUNC(vexDeviceMotorDirectionGet, int32_t, (V5_DeviceT device), (device), 0x2dc)
JUMPTABLE_FUNC(vexDeviceMotorModeSet, void, (V5_DeviceT device, V5MotorControlMode mode), (device, mode), 0x2e0)
JUMPTABLE_FUNC(vexDeviceMotorModeGet, V5MotorControlMode, (V5_DeviceT device), (device), 0x2e4)
JUMPTABLE_FUNC(vexDeviceMotorPwmSet, void, (V5_DeviceT device, int32_t pwm), (device, pwm), 0x2e8)
JUMPTABLE_FUNC(vexDeviceMotorPwmGet, int32_t, (V5_DeviceT device), (device), 0x2ec)
JUMPTABLE_FUNC(vexDeviceMotorCurrentLimitSet, void, (V5_DeviceT device, int32_t limit), (device, limit), 0x2f0)
JUMPTABLE_FUNC(vexDeviceMotorCurrentLimitGet, int32_t, (V5_DeviceT device), (device), 0x2f4)
JUMPTABLE_FUNC(vexDeviceMotorCurrentGet, int32_t, (V5_DeviceT device), (device), 0x2f8)
JUMPTABLE_FUNC(vexDeviceMotorPowerGet, double, (V5_DeviceT device), (device), 0x2fc)
JUMPTABLE_FUNC(vexDeviceMotorTorqueGet, double, (V5_DeviceT device), (device), 0x300)
JUMPTABLE_FUNC(vexDeviceMotorEfficiencyGet, double, (V5_DeviceT device), (device), 0x304)
JUMPTABLE_FUNC(vexDeviceMotorTemperatureGet, double, (V5_DeviceT device), (device), 0x308)
JUMPTABLE_FUNC(vexDeviceMotorOverTempFlagGet, bool, (V5_DeviceT device), (device), 0x30c)
JUMPTABLE_FUNC(vexDeviceMotorCurrentLimitFlagGet, bool, (V5_DeviceT device), (device), 0x310)
JUMPTABLE_FUNC(vexDeviceMotorZeroVelocityFlagGet, bool, (V5_DeviceT device), (device), 0x314)
JUMPTABLE_FUNC(vexDeviceMotorZeroPositionFlagGet, bool, (V5_DeviceT device), (device), 0x318)
JUMPTABLE_FUNC(vexDeviceMotorReverseFlagSet, void, (V5_DeviceT device, bool reverse), (device, reverse), 0x31c)
JUMPTABLE_FUNC(vexDeviceMotorReverseFlagGet, bool, (V5_DeviceT device), (device), 0x320)
JUMPTABLE_FUNC(vexDeviceMotorEncoderUnitsSet, void, (V5_DeviceT device, V5MotorEncoderUnits units), (device, units), 0x324)
JUMPTABLE_FUNC(vexDeviceMotorEncoderUnitsGet, V5MotorEncoderUnits, (V5_DeviceT device), (device), 0x328)
JUMPTABLE_FUNC(vexDeviceMotorBrakeModeSet, void, (V5_DeviceT device, V5MotorBrakeMode mode), (device, mode), 0x32c)
JUMPTABLE_FUNC(vexDeviceMotorBrakeModeGet, V5MotorBrakeMode, (V5_DeviceT device), (device), 0x330)
JUMPTABLE_FUNC(vexDeviceMotorPositionSet, void, (V5_DeviceT device, double position), (device, position), 0x334)
JUMPTABLE_FUNC(vexDeviceMotorPositionGet, double, (V5_DeviceT device), (device), 0x338)
JUMPTABLE_FUNC(vexDeviceMotorPositionRawGet, int32_t, (V5_DeviceT device, uint32_t *timestamp), (device, timestamp), 0x33c)
JUMPTABLE_FUNC(vexDeviceMotorPositionReset, void, (V5_DeviceT device), (device), 0x340)
JUMPTABLE_FUNC(vexDeviceMotorTargetGet, double, (V5_DeviceT device), (device), 0x344)
JUMPTABLE_FUNC(vexDeviceMotorServoTargetSet, void, (V5_DeviceT device, double position), (device, position), 0x348)
JUMPTABLE_FUNC(vexDeviceMotorAbsoluteTargetSet, void, (V5_DeviceT device, double position, int32_t velocity), (device, position, velocity), 0x34c)
JUMPTABLE_FUNC(vexDeviceMotorRelativeTargetSet, void, (V5_DeviceT device, double position, int32_t velocity), (device, position, velocity), 0x350)
JUMPTABLE_FUNC(vexDeviceMotorFaultsGet, uint32_t, (V5_DeviceT device), (device), 0x354)
JUMPTABLE_FUNC(vexDeviceMotorFlagsGet, uint32_t, (V5_DeviceT device), (device), 0x358)
JUMPTABLE_FUNC(vexDeviceMotorVoltageSet, void, (V5_DeviceT device, int32_t voltage), (device, voltage), 0x35c)
JUMPTABLE_FUNC(vexDeviceMotorVoltageGet, int32_t, (V5_DeviceT device), (device), 0x360)
JUMPTABLE_FUNC(vexDeviceMotorGearingSet, void, (V5_DeviceT device, V5MotorGearset gearset), (device, gearset), 0x364)
JUMPTABLE_FUNC(vexDeviceMotorGearingGet, V5MotorGearset, (V5_DeviceT device), (device), 0x368)
JUMPTABLE_FUNC(vexDeviceMotorVoltageLimitSet, void, (V5_DeviceT device, int32_t limit), (device, limit), 0x36c)
JUMPTABLE_FUNC(vexDeviceMotorVoltageLimitGet, int32_t, (V5_DeviceT device), (device), 0x370)
JUMPTABLE_FUNC(vexDeviceMotorVelocityUpdate, void, (V5_DeviceT device, int32_t velocity), (device, velocity), 0x374)
JUMPTABLE_FUNC(vexDeviceMotorPositionPidSet, void, (V5_DeviceT device, V5_DeviceMotorPid *pid), (device, pid), 0x378)
JUMPTABLE_FUNC(vexDeviceMotorVelocityPidSet, void, (V5_DeviceT device, V5_DeviceMotorPid *pid), (device, pid), 0x37c)
JUMPTABLE_FUNC(vexDeviceMotorExternalProfileSet, void, (V5_DeviceT device, double position, int32_t velocity), (device, position, velocity), 0x380)

#ifdef __cplusplus
}
#endif

