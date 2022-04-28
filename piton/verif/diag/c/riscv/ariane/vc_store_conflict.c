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

  int32_t* a1 = (int32_t*)0x84000000;
  int32_t* a2 = (int32_t*)0x84100000;
  int32_t* a3 = (int32_t*)0x84200000;
  int32_t* a4 = (int32_t*)0x84300000;
  int32_t* a5 = (int32_t*)0x84400000;
  
  perf_marker( 1555 );

  for (int32_t k = 0; k < 10; k++) {
    *a1 = k;
    *a2 = k + 1;
    *a3 = k + 2;
    *a4 = k + 3;
    *a5 = k + 4;
  }

  perf_marker( 1666 );

  printf("%d\n", *a1 + *a2 + *a3 + *a4 + *a5);
  
  return 0;
}