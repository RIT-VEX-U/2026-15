#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define JUMPTABLE_START 0x037FC000u

#define JUMPTABLE_FUNC(name, ret, params, call_args, offset) \
inline ret name params { \
  return ((ret (* __attribute__((pcs("aapcs"))))params)(*(uintptr_t*)(JUMPTABLE_START + (offset))))call_args; \
}

#ifdef __cplusplus
}
#endif

