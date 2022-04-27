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

  int32_t tmp = 0;
  
  perf_marker( 1555 );

  *a1 = 1;
  *a2 = 2;
  *a3 = 3;
  *a4 = 4;

  for (int32_t k = 0; k < 10; k++) {
    //*a1 = k;
    //*a2 = k;
    //*a3 = k;
    //*a4 = k;
    //*a5 = k;
    
    tmp += *a1;
    tmp += *a2;
    tmp += *a3;
    tmp += *a4;
  }

  perf_marker( 1666 );

  printf("%d\n", tmp);

  printf("Done!\n");
  
  return 0;
}