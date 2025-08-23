#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexCompetitionStatus, uint32_t, (void), (), 0x9d8)
JUMPTABLE_FUNC(vexCompetitionControl, void, (uint32_t data), (data), 0x9dc)

#ifdef __cplusplus
}
#endif

