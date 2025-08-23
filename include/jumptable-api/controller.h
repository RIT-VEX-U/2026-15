#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kControllerMaster = 0,
  kControllerPartner = 1,
} V5_ControllerId;

typedef enum {
  kV5ControllerOffline = 0,
  kV5ControllerTethered = 1,
  kV5ControllerVexnet = 2,
} V5_ControllerStatus;

typedef enum V5_ControllerIndex {
  AnaLeftX        =  0u,
  AnaLeftY        =  1u,
  AnaRightX       =  2u,
  AnaRightY       =  3u,
  AnaSpare1       =  4u,
  AnaSpare2       =  5u,
  Button5U        =  6u,
  Button5D        =  7u,
  Button6U        =  8u,
  Button6D        =  9u,
  Button7U        = 10u,
  Button7D        = 11u,
  Button7L        = 12u,
  Button7R        = 13u,
  Button8U        = 14u,
  Button8D        = 15u,
  Button8L        = 16u,
  Button8R        = 17u,
  ButtonSEL       = 18u,
  BatteryLevel    = 19u,
  ButtonAll       = 20u,
  Flags           = 21u,
  BatteryCapacity = 22u,

  // aliases
  Axis1           = AnaRightX,
  Axis2           = AnaRightY,
  Axis3           = AnaLeftY,
  Axis4           = AnaLeftX,
  ButtonL1        = Button5U,
  ButtonL2        = Button5D,
  ButtonR1        = Button6U,
  ButtonR2        = Button6D,
  ButtonUp        = Button7U,
  ButtonDown      = Button7D,
  ButtonLeft      = Button7L,
  ButtonRight     = Button7R,
  ButtonX         = Button8U,
  ButtonB         = Button8D,
  ButtonY         = Button8L,
  ButtonA         = Button8R
} V5_ControllerIndex;

JUMPTABLE_FUNC(vexControllerGet, int32_t, (V5_ControllerId id, V5_ControllerIndex index), (id, index), 0x1a4)
JUMPTABLE_FUNC(vexControllerConnectionStatusGet, V5_ControllerStatus, (V5_ControllerId id), (id), 0x1a8)
JUMPTABLE_FUNC(vexControllerTextSet, uint32_t, (uint32_t id, uint32_t line, uint32_t col, const uint8_t *buf), (id, line, col, buf), 0x1ac)

#ifdef __cplusplus
}
#endif

