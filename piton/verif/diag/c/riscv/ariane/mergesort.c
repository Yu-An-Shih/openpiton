// Copyright 2018 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdint.h>
#include <stdio.h>
#include "util.h"

#define SIZE 2048
#define TYPE int32_t

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

int main(int argc, char** argv) {
    TYPE args[SIZE] = {};

    int i;
    TYPE lfsr = 0xAB3019E1;
    for (i = 0; i < SIZE; i++) {
        lfsr ^= lfsr << 13;
        lfsr ^= lfsr >> 17;
        lfsr ^= lfsr << 5;

        args[i] = (*((TYPE *) &lfsr)) & 0x7FFFFFFF;
    }
    printf("sort start\n");

    perf_marker( 1555 );
    ms_mergesort(args);
    perf_marker( 1666 );


    printf("sort done\n");

    return 0;
}

void merge(TYPE a[SIZE], int start, int m, int stop){
    TYPE temp[SIZE];
    int i, j, k;

    merge_label1 : for(i=start; i<=m; i++){
        temp[i] = a[i];
    }

    merge_label2 : for(j=m+1; j<=stop; j++){
        temp[m+1+stop-j] = a[j];
    }

    i = start;
    j = stop;

    merge_label3 : for(k=start; k<=stop; k++){
        TYPE tmp_j = temp[j];
        TYPE tmp_i = temp[i];
        if(tmp_j < tmp_i) {
            a[k] = tmp_j;
            j--;
        } else {
            a[k] = tmp_i;
            i++;
        }
    }
}

void ms_mergesort(TYPE a[SIZE]) {
    int start, stop;
    int i, m, from, mid, to;

    start = 0;
    stop = SIZE;

    mergesort_label1 : for(m=1; m<stop-start; m+=m) {
        mergesort_label2 : for(i=start; i<stop; i+=m+m) {
            from = i;
            mid = i+m-1;
            to = i+m+m-1;
            if(to < stop){
                merge(a, from, mid, to);
            }
            else{
                merge(a, from, mid, stop);
            }
        }
    }
}