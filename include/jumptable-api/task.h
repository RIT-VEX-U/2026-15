#pragma once

#include <stdint.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

typedef int32_t(*task_callback)(void);

JUMPTABLE_FUNC(vexTaskAdd, void, (int32_t (*callback)(void), int32_t interval, const char *label), (callback, interval, label), 0x028)
JUMPTABLE_FUNC(vexTaskAddWithPriority, void, (int32_t (*callback)(void), int32_t interval, const char *label, int32_t priority), (callback, interval, label, priority), 0x02c)
JUMPTABLE_FUNC(vexTaskAddWithArg, void, (int32_t (*callback)(void *), int32_t interval, void *args, const char *label), (callback, interval, args, label), 0x030)
JUMPTABLE_FUNC(vexTaskAddWithPriorityWithArg, void, (int32_t (*callback)(void *), int32_t interval, void *args, const char *label, int32_t priority), (callback, interval, args, label, priority), 0xf54)
JUMPTABLE_FUNC(vexTaskGetCallbackAndId, task_callback, (uint32_t index, int32_t *callback_id), (index, callback_id), 0x084)
JUMPTABLE_FUNC(vexTaskStopWithId, void, (int32_t (*callback)(void), int32_t callback_id), (callback, callback_id), 0xf58)
JUMPTABLE_FUNC(vexTaskSuspendWithId, void, (int32_t (*callback)(void), int32_t callback_id), (callback, callback_id), 0xf5c)
JUMPTABLE_FUNC(vexTaskResumeWithId, void, (int32_t (*callback)(void), int32_t callback_id), (callback, callback_id), 0xf60)
JUMPTABLE_FUNC(vexTaskPriorityGetWithId, int32_t, (int32_t (*callback)(void), int32_t callback_id), (callback, callback_id), 0xf64)
JUMPTABLE_FUNC(vexTaskPrioritySetWithId, void, (int32_t (*callback)(void), int32_t callback_id, int32_t priority), (callback, callback_id, priority), 0xf68)
JUMPTABLE_FUNC(vexTaskSleep, void, (uint32_t time), (time), 0x06c)
JUMPTABLE_FUNC(vexTaskYield, void, (void), (), 0x060)
JUMPTABLE_FUNC(vexBackgroundProcessing, void, (void), (), 0xf74)
JUMPTABLE_FUNC(vexTasksRun, void, (void), (), 0x05c)
JUMPTABLE_FUNC(vexTasksDump, void, (void), (), 0x080)
JUMPTABLE_FUNC(vexTaskGetIndex, int32_t, (int32_t (*callback)(void)), (callback), 0x068)
JUMPTABLE_FUNC(vexTaskGetTaskIndex, int32_t, (int32_t (*callback)(void)), (callback), 0x090)

#ifdef __cplusplus
}
#endif

