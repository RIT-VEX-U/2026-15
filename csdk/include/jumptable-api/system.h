#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

#pragma pack(push,1)
typedef struct {
  uint32_t magic;   // must be 'VXV5' or 0x35585658
  uint32_t r_type;  // program type
  uint32_t owner;   // program originator
  uint32_t options; // program options
} vcodesig;
#pragma pack(pop)

#define V5_sig_magic  0x35585658u
#define EX_SIG_MAGIC  0x45585658u

#define V5_SIG_TYPE_USER      0u
#define V5_SIG_OWNER_SYS      0u
#define V5_SIG_OWNER_VEX      1u
#define V5_SIG_OWNER_PARTNER  2u

#define V5_SIG_OPTIONS_NONE   0u
#define V5_SIG_OPTIONS_INDG   (1u<<0) // invert default graphics colors
#define V5_SIG_OPTIONS_EXIT   (1u<<1) // kill threads when main exits
#define V5_SIG_OPTIONS_THDG   (1u<<2) // invert graphics based on theme

#pragma pack(push,1)
typedef struct {
  uint8_t ti_hour;
  uint8_t ti_min;
  uint8_t ti_sec;
  uint8_t ti_hund;
} vex_time_t;

typedef struct {
  uint16_t da_year;
  uint8_t da_day;
  uint8_t da_mon;
} vex_date_t;
#pragma pack(pop)

JUMPTABLE_FUNC(vexStdlibMismatchError, void, (uint32_t param_1, uint32_t param_2), (param_1, param_2), 0x010)
JUMPTABLE_FUNC(vexScratchMemoryPtr, int32_t, (void **ptr), (ptr), 0x01C)
JUMPTABLE_FUNC(vexScratchMemoryLock, bool, (void), (), 0x998)
JUMPTABLE_FUNC(vexScratchMemoryUnlock, void, (void), (), 0x99c)
JUMPTABLE_FUNC(vexSystemTimeGet, uint32_t, (void), (), 0x118)
JUMPTABLE_FUNC(vexGettime, vex_time_t, (void), (), 0x11c)
JUMPTABLE_FUNC(vexGetdate, vex_date_t, (void), (), 0x120)
JUMPTABLE_FUNC(vexSystemMemoryDump, void, (void), (), 0x124)
JUMPTABLE_FUNC(vexSystemDigitalIO, void, (uint32_t pin, uint32_t value), (pin, value), 0x128)
JUMPTABLE_FUNC(vexSystemStartupOptions, uint32_t, (void), (), 0x12c)
JUMPTABLE_FUNC(vexSystemExitRequest, void, (void), (), 0x130)
JUMPTABLE_FUNC(vexSystemHighResTimeGet, uint64_t, (void), (), 0x134)
JUMPTABLE_FUNC(vexSystemPowerupTimeGet, uint64_t, (void), (), 0x138)
JUMPTABLE_FUNC(vexSystemLinkAddrGet, uint32_t, (void), (), 0x13c)
JUMPTABLE_FUNC(vexSystemUsbStatus, uint32_t, (void), (), 0x174)
JUMPTABLE_FUNC(vexSystemTimerStop, void, (void), (), 0x8c0)
JUMPTABLE_FUNC(vexSystemTimerClearInterrupt, void, (void), (), 0x8c4)
JUMPTABLE_FUNC(vexSystemTimerReinitForRtos, int32_t, (uint32_t priority, void (*handler)(void*)), (priority, handler), 0x8c8)
JUMPTABLE_FUNC(vexSystemApplicationIRQHandler, void, (uint32_t ulICCIAR), (ulICCIAR), 0x8cc)
JUMPTABLE_FUNC(vexSystemWatchdogReinitRtos, int32_t, (void), (), 0x8d0)
JUMPTABLE_FUNC(vexSystemWatchdogGet, uint32_t, (void), (), 0x8d4)
JUMPTABLE_FUNC(vexSystemBoot, void, (void), (), 0x910)
JUMPTABLE_FUNC(vexSystemUndefinedException, void, (void), (), 0x914)
JUMPTABLE_FUNC(vexSystemFIQInterrupt, void, (void), (), 0x918)
JUMPTABLE_FUNC(vexSystemIQRQnterrupt, void, (void), (), 0x920)
JUMPTABLE_FUNC(vexSystemDataAbortInterrupt, void, (void), (), 0x924)
JUMPTABLE_FUNC(vexSystemPrefetchAbortInterrupt, void, (void), (), 0x928)

static inline uint32_t vexSystemVersion(void) {
  return *(volatile uint32_t*)( (uintptr_t)JUMPTABLE_START + 0x1000);
}

static inline uint32_t vexStdlibVersion(void) {
  return *(volatile uint32_t*)( (uintptr_t)JUMPTABLE_START + 0x1000);
}

#ifdef __cplusplus
}
#endif

