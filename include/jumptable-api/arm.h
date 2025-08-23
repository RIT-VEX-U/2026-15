#pragma once 

#include <stdint.h>
#include "jumptable.h"
#include "device.h"

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  int32_t tip_x;
  int32_t tip_y;
  int32_t tip_z;
  int32_t tip_roll;
  int32_t tip_pitch;
  int32_t tip_yaw;
  int8_t pose;
  int16_t velocity;
} V5_DeviceArmTipPosition;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDeviceArmMoveTipCommandLinearAdv, void, (V5_DeviceT device, V5_DeviceArmTipPosition *position, double j6_rotation, uint16_t j6_velocity, bool relative), (device, position, j6_rotation, j6_velocity, relative), 0xb54)
JUMPTABLE_FUNC(vexDeviceArmMoveTipCommandJointAdv, void, (V5_DeviceT device, V5_DeviceArmTipPosition *position, double j6_rotation, uint16_t j6_velocity, bool relative), (device, position, j6_rotation, j6_velocity, relative), 0xb58)
JUMPTABLE_FUNC(vexDeviceArmTipPositionGetAdv, void, (V5_DeviceT device, V5_DeviceArmTipPosition *position), (device, position), 0xb5c)
JUMPTABLE_FUNC(vexDeviceArmPoseSet, void, (V5_DeviceT device, uint8_t pose, uint16_t velocity), (device, pose, velocity), 0xc30)
JUMPTABLE_FUNC(vexDeviceArmMoveTipCommandLinear, void, (V5_DeviceT device, int32_t x, int32_t y, int32_t z, uint8_t pose, uint16_t velocity, double rotation, uint16_t rot_velocity, bool relative), (device, x, y, z, pose, velocity, rotation, rot_velocity, relative), 0xc34)
JUMPTABLE_FUNC(vexDeviceArmMoveTipCommandJoint, void, (V5_DeviceT device, int32_t x, int32_t y, int32_t z, uint8_t pose, uint16_t velocity, double rotation, uint16_t rot_velocity, bool relative), (device, x, y, z, pose, velocity, rotation, rot_velocity, relative), 0xc38)
JUMPTABLE_FUNC(vexDeviceArmMoveJointsCommand, void, (V5_DeviceT device, double *positions, uint16_t *velocities, double j6_rotation, uint16_t j6_velocity, double j7_volts, uint16_t j7_timeout, uint16_t j7_i_limit, bool relative), (device, positions, velocities, j6_rotation, j6_velocity, j7_volts, j7_timeout, j7_i_limit, relative), 0xc3c)
JUMPTABLE_FUNC(vexDeviceArmSpinJoints, void, (V5_DeviceT device, double *velocities), (device, velocities), 0xc40)
JUMPTABLE_FUNC(vexDeviceArmSetJointPositions, void, (V5_DeviceT device, double *new_positions), (device, new_positions), 0xc44)
JUMPTABLE_FUNC(vexDeviceArmPickUpCommand, void, (V5_DeviceT device), (device), 0xc48)
JUMPTABLE_FUNC(vexDeviceArmDropCommand, void, (V5_DeviceT device), (device), 0xc4c)
JUMPTABLE_FUNC(vexDeviceArmMoveVoltsCommand, void, (V5_DeviceT device, double *voltages), (device, voltages), 0xc50)
JUMPTABLE_FUNC(vexDeviceArmFullStop, void, (V5_DeviceT device, uint8_t brakeMode), (device, brakeMode), 0xc54)
JUMPTABLE_FUNC(vexDeviceArmEnableProfiler, void, (V5_DeviceT device, uint8_t enable), (device, enable), 0xc58)
JUMPTABLE_FUNC(vexDeviceArmProfilerVelocitySet, void, (V5_DeviceT device, uint16_t linear_velocity, uint16_t joint_velocity), (device, linear_velocity, joint_velocity), 0xc5c)
JUMPTABLE_FUNC(vexDeviceArmSaveZeroValues, void, (V5_DeviceT device), (device), 0xc60)
JUMPTABLE_FUNC(vexDeviceArmForceZeroCommand, void, (V5_DeviceT device), (device), 0xc64)
JUMPTABLE_FUNC(vexDeviceArmClearZeroValues, void, (V5_DeviceT device), (device), 0xc68)
JUMPTABLE_FUNC(vexDeviceArmBootload, void, (V5_DeviceT device), (device), 0xc6c)
JUMPTABLE_FUNC(vexDeviceArmTipPositionGet, void, (V5_DeviceT device, int32_t *x, int32_t *y, int32_t *z), (device, x, y, z), 0xc70)
JUMPTABLE_FUNC(vexDeviceArmJointInfoGet, void, (V5_DeviceT device, double *positions, double *velocities, int32_t *currents), (device, positions, velocities, currents), 0xc74)
JUMPTABLE_FUNC(vexDeviceArmJ6PositionGet, double, (V5_DeviceT device), (device), 0xc78)
JUMPTABLE_FUNC(vexDeviceArmBatteryGet, int32_t, (V5_DeviceT device), (device), 0xc7c)
JUMPTABLE_FUNC(vexDeviceArmServoFlagsGet, int32_t, (V5_DeviceT device, uint32_t servoID), (device, servoID), 0xc80)
JUMPTABLE_FUNC(vexDeviceArmStatusGet, uint32_t, (V5_DeviceT device), (device), 0xc84)
JUMPTABLE_FUNC(vexDeviceArmDebugGet, uint32_t, (V5_DeviceT device, int32_t id), (device, id), 0xc88)
JUMPTABLE_FUNC(vexDeviceArmJointErrorsGet, void, (V5_DeviceT device, uint8_t *errors), (device, errors), 0xc8c)
JUMPTABLE_FUNC(vexDeviceArmJ6PositionSet, void, (V5_DeviceT device, int16_t position), (device, position), 0xc90)
JUMPTABLE_FUNC(vexDeviceArmStopJointsCommand, void, (V5_DeviceT device, int16_t *brakeModes), (device, brakeModes), 0xc94)
JUMPTABLE_FUNC(vexDeviceArmReboot, void, (V5_DeviceT device), (device), 0xc98)
JUMPTABLE_FUNC(vexDeviceArmTipOffsetSet, void, (V5_DeviceT device, int32_t x, int32_t y, int32_t z), (device, x, y, z), 0xc9c)

#ifdef __cplusplus
}
#endif

