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
#define FFT_SIZE SIZE
#define TYPE float

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

#define twoPI 6.28318530717959
#define CONST_PI 3.14159265358979
#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

// https://austinhenley.com/blog/cosine.html
TYPE cos_taylor_literal_6terms(TYPE x)
{
    x = modd(x, twoPI);
    char sign = 1;
    if (x > CONST_PI)
    {
        x -= CONST_PI;
        sign = -1;
    }
    TYPE xx = x * x;

    return sign * (1 - ((xx) / (2)) + ((xx * xx) / (24)) - ((xx * xx * xx) / (720)) + ((xx * xx * xx * xx) / (40320)) - ((xx * xx * xx * xx * xx) / (3628800)) + ((xx * xx * xx * xx * xx * xx) / (479001600)));
}

TYPE sin_taylor_literal_5terms(TYPE x)
{
    x = modd(x, twoPI);
    char sign = 1;
    if (x > CONST_PI)
    {
        x -= CONST_PI;
        sign = -1;
    }
    TYPE xx = x * x;
    TYPE xxx = x * x * x;

    return sign * (x - ((xxx) / (6)) + ((xxx * xx) / (120)) - ((xxx * xx * xx) / (5040)) + ((xxx * xx * xx * xx) / (362880)) - ((xxx * xx * xx * xx * xx) / (39916800)));
}

void fft(TYPE real[FFT_SIZE], TYPE img[FFT_SIZE], TYPE real_twid[FFT_SIZE/2], TYPE img_twid[FFT_SIZE/2]){
    int even, odd, span, log, rootindex;
    TYPE temp;
    log = 0;

    outer:for(span=FFT_SIZE>>1; span; span>>=1, log++){
        inner:for(odd=span; odd<FFT_SIZE; odd++){
            odd |= span;
            even = odd ^ span;

            temp = real[even] + real[odd];
            real[odd] = real[even] - real[odd];
            real[even] = temp;

            temp = img[even] + img[odd];
            img[odd] = img[even] - img[odd];
            img[even] = temp;

            rootindex = (even<<log) & (FFT_SIZE - 1);
            if(rootindex){
                temp = real_twid[rootindex] * real[odd] -
                    img_twid[rootindex]  * img[odd];
                img[odd] = real_twid[rootindex]*img[odd] +
                    img_twid[rootindex]*real[odd];
                real[odd] = temp;
            }
        }
    }
}

int main(int argc, char** argv) {
    TYPE data_x[SIZE];
    TYPE data_y[SIZE];
    TYPE img[SIZE];
    TYPE real[SIZE];
    int i;

    //set up twiddles...
    TYPE typed;
    int n, N;
    N = SIZE;

    //Pre-calc twiddles
    for(n=0; n<(N>>1); n++){
        typed = (TYPE)(twoPI*n/N);
        real[n] = cos_taylor_literal_6terms(typed);
        img[n] = (-1.0)*sin_taylor_literal_5terms(typed);
    }

    //Init data
    for(i=0; i < SIZE; i++) {
        data_x[i] = (TYPE)(i);
        data_y[i] = (TYPE)(i);
    }

    printf("fft start\n");

    perf_marker( 1555 );
    fft(data_x, data_y, real, img);
    perf_marker( 1666 );


    printf("fft done\n");

    return 0;
}