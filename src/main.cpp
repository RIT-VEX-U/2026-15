#include "jumptable-api/display.h"
#include "jumptable-api/system.h"
#include "jumptable-api/serial.h"
#include "jumptable-api/task.h"

#include <stdio.h>
#include <cstdio>

extern uint32_t *__bss_start;
extern uint32_t *__bss_end;
extern uint32_t *__sbss_start;
extern uint32_t *__sbss_end;

extern void *__impure_ptr_slot;

extern void __libc_init_array(void);
extern void __libc_fini_array(void);

extern "C" {
__attribute__((used, section(".vectors")))
static const uint32_t vexCodeSig[8] = {
  0x35585658u, 0x00000000u, 0x00000001u, 0x00000000u,
  0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u
};

static int vex_putc(char c, FILE *f) {
  (void)f;
  if (c == '\n') (void)vexSerialWriteChar(0, '\r');
  (void)vexSerialWriteChar(0, (uint8_t)c);
  return (unsigned char)c;
}

static int vex_getc(FILE *f) {
  (void)f;
  int32_t v;
  do { v = vexSerialReadChar(0); } while (v < 0);
  return (unsigned char)v;
}

static FILE __stdio = FDEV_SETUP_STREAM(vex_putc, vex_getc, NULL, _FDEV_SETUP_RW);

FILE *const stdin = &__stdio;
FILE *const stdout = &__stdio;
FILE *const stderr = &__stdio;
}

/**
 * Entry point to the program. No code should be placed here;
 * instead use competition/opcontrol.cpp and
 * competition/autonomous.cpp
 */
int main() {
  int i = 0;
  // vexDisplayPrintf(20, 50, 255, "printf\n");
  while (true) {
    printf("%d\n", i);
    i++;
    vexTasksRun();
  }

}

extern "C" {
void vexStartup(void) __attribute__((section(".boot")));
void vexStartup() {

  // Zero from __bss_start__ to __bss_end__
  uint32_t *p = __bss_start;
  uint32_t *end = __bss_end;
  while (p < end) {
    *p++ = 0;
  }

  // // Zero from __sbss_start__ to __sbss_end__
  // uint32_t *p2 = __sbss_start;
  // uint32_t *end2 = __sbss_end;
  // while (p2 < end2) {
  //   *p++ = 0;
  // }


  main();

  
}
}
