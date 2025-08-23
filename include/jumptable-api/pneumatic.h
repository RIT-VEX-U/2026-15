#pragma once

#include <stdint.h>
#include "device.h"
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  uint16_t flags;
  uint8_t m1_pwm;
  uint8_t m2_pwm;
  uint8_t m3_pwm;
  uint8_t m4_pwm;
  uint8_t m1_time;
  uint8_t m2_time;
  uint8_t m3_time;
  uint8_t m4_time;
  uint8_t comp_pwm;
} V5_DevicePneumaticCtrl;
#pragma pack(pop)

JUMPTABLE_FUNC(vexDevicePneumaticActuationStatusGet, uint32_t, (V5_DeviceT device, uint16_t *ac1, uint16_t *ac2, uint16_t *ac3, uint16_t *ac4), (device, ac1, ac2, ac3, ac4), 0xc28)
JUMPTABLE_FUNC(vexDevicePneumaticCompressorSet, void, (V5_DeviceT device, bool bState), (device, bState), 0xc08)
JUMPTABLE_FUNC(vexDevicePneumaticCtrlSet, void, (V5_DeviceT device, V5_DevicePneumaticCtrl *pCtrl), (device, pCtrl), 0xc10)
JUMPTABLE_FUNC(vexDevicePneumaticCylinderPwmSet, void, (V5_DeviceT device, uint32_t id, bool bState, uint8_t pwm), (device, id, bState, pwm), 0xc20)
JUMPTABLE_FUNC(vexDevicePneumaticCylinderSet, void, (V5_DeviceT device, uint32_t id, bool bState), (device, id, bState), 0xc0c)
JUMPTABLE_FUNC(vexDevicePneumaticPwmGet, uint32_t, (V5_DeviceT device), (device), 0xc1c)
JUMPTABLE_FUNC(vexDevicePneumaticPwmSet, void, (V5_DeviceT device, uint8_t pwm), (device, pwm), 0xc18)
JUMPTABLE_FUNC(vexDevicePneumaticStatusGet, uint32_t, (V5_DeviceT device), (device), 0xc14)

#ifdef __cplusplus
}
#endif

