#include <stdint.h>
#include <stdio.h>

#define perf_marker( x ) \
    asm (   "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
            "addi zero,zero," #x ";\n"  \
        );

int main(int argc, char ** argv) {

  volatile int32_t* a1 = (int32_t*)0x000A0000;
  volatile int32_t* a2 = (int32_t*)0x000A1000;
  volatile int32_t* a3 = (int32_t*)0x000A2000;
  volatile int32_t* a4 = (int32_t*)0x000A3000;
  volatile int32_t* a5 = (int32_t*)0x000A4000;
  
  perf_marker( 1555 );

  for (int32_t k = 0; k < 3; k++) {
    *a1 = k;
    *a2 = k;
    *a3 = k;
    *a4 = k;
    *a5 = k;
    //printf(*a1);
  }

  perf_marker( 1666 );

  printf("Done!\n");

  return 0;
}