#pragma once

#include <stdint.h>
#include <stdarg.h>
#include "jumptable.h" 

#ifdef __cplusplus 
extern "C" { 
#endif

JUMPTABLE_FUNC(vexSerialWriteChar,       int32_t, (uint32_t channel, uint8_t c),                                        (channel, c),                                        0x898)
JUMPTABLE_FUNC(vexSerialWriteBuffer,     int32_t, (uint32_t channel, const uint8_t *data, uint32_t data_len),         (channel, data, data_len),                          0x89c)
JUMPTABLE_FUNC(vexSerialReadChar,        int32_t, (uint32_t channel),                                                 (channel),                                           0x8a0)
JUMPTABLE_FUNC(vexSerialPeekChar,        int32_t, (uint32_t channel),                                                 (channel),                                           0x8a4)
JUMPTABLE_FUNC(vexSerialWriteFree,       int32_t, (uint32_t channel),                                                 (channel),                                           0x8ac)
JUMPTABLE_FUNC(vex_vprintf,              int32_t, (const char *format, va_list args),                                  (format, args),                                      0x0f0)
JUMPTABLE_FUNC(vex_vsprintf,             int32_t, (char *out, const char *format, va_list args),                       (out, format, args),                                 0x0f4)
JUMPTABLE_FUNC(vex_vsnprintf,            int32_t, (char *out, uint32_t max_len, const char *format, va_list args),     (out, max_len, format, args),                       0x0f8)

int32_t vex_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int32_t result = vex_vprintf(format, args);
    va_end(args);
    return result;
}

int32_t vex_sprintf(char *out, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int32_t result = vex_vsprintf(out, format, args);
    va_end(args);
    return result;
}

int32_t vex_snprintf(char *out, uint32_t max_len, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int32_t result = vex_vsnprintf(out, max_len, format, args);
    va_end(args);
    return result;
}

#ifdef __cplusplus
}
#endif

