#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef enum {
  kTouchEventRelease = 0,
  kTouchEventPress = 1,
  kTouchEventPressAuto = 2,
} V5_TouchEvent;

#pragma pack(push,1)
typedef struct {
  V5_TouchEvent lastEvent;
  int16_t lastXpos;
  int16_t lastYpos;
  int32_t pressCount;
  int32_t releaseCount;
} V5_TouchStatus;
#pragma pack(pop)

JUMPTABLE_FUNC(vexTouchUserCallbackSet, void, (void (*callback)(V5_TouchEvent, int32_t, int32_t)), (callback), 0x960)
JUMPTABLE_FUNC(vexTouchDataGet, void, (V5_TouchStatus *status), (status), 0x964)

#ifdef __cplusplus
}
#endif

