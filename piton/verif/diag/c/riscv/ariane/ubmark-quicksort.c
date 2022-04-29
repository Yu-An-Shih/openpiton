//========================================================================
// ubmark-quicksort
//========================================================================

//#include "ubmark.h"
//#include "ubmark-quicksort.dat"

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

//------------------------------------------------------------------------
// quick_sort
//------------------------------------------------------------------------

__attribute__ ((noinline))
void quick_sort (int *a, int n) {
  if (n < 2)
    return;
  int p = a[n / 2];
  int *l = a;
  int *r = a + n - 1;
  while (l <= r) {
    if (*l < p) {
      l++;
      continue;
    }
    if (*r > p) {
      r--;
      continue; // we need to check the condition (l <= r) every time we change the value of l or r
    }
    int t = *l;
    *l++ = *r;
    *r-- = t;
  }
  quick_sort(a, r - a + 1);
  quick_sort(l, a + n - l);
}

//------------------------------------------------------------------------
// verify_results
//------------------------------------------------------------------------

void verify_results( int values[], int ref[], int size )
{
  int temp = 0;
  int i;
  for ( i = 0; i < size; i++ ) {
    if ( !( values[i] == ref[i] ) ) {
      //test_fail( i );
      fail();
    }
  }
  //test_pass( temp );
  pass();
}

//------------------------------------------------------------------------
// Test harness
//------------------------------------------------------------------------

int main( int argc, char* argv[] )
{
  int temp = 0;
  
  int data_size = 100;
  int data[] = {
    10003,
    -47842,
    7151,
    25321,
    45642,
    49208,
    -7432,
    -14910,
    -63332,
    -20676,
    -64018,
    99492,
    96287,
    57101,
    -49928,
    37541,
    1246,
    -19342,
    -80119,
    -42066,
    77981,
    -31378,
    59695,
    -19630,
    46490,
    6607,
    47268,
    -77432,
    -18045,
    81516,
    68456,
    67744,
    -61669,
    -18109,
    80180,
    -98587,
    72519,
    96815,
    55094,
    -43336,
    83384,
    -89137,
    -16750,
    66015,
    -96067,
    61237,
    8954,
    2906,
    33830,
    16011,
    -28502,
    78676,
    7100,
    -92804,
    -9755,
    29881,
    -88915,
    -41716,
    97161,
    59771,
    -57515,
    -55909,
    -91380,
    -55797,
    85036,
    17103,
    69606,
    57779,
    27672,
    4868,
    3736,
    -85954,
    -82154,
    42016,
    63573,
    -81551,
    48191,
    94253,
    33437,
    50412,
    71694,
    40169,
    68295,
    -97724,
    96827,
    -52169,
    -29122,
    -91643,
    -3368,
    -88585,
    -15615,
    29544,
    44751,
    -68300,
    39512,
    -19841,
    76279,
    72500,
    77321,
    89078,
  };
  int ref[] = {
    -98587,
    -97724,
    -96067,
    -92804,
    -91643,
    -91380,
    -89137,
    -88915,
    -88585,
    -85954,
    -82154,
    -81551,
    -80119,
    -77432,
    -68300,
    -64018,
    -63332,
    -61669,
    -57515,
    -55909,
    -55797,
    -52169,
    -49928,
    -47842,
    -43336,
    -42066,
    -41716,
    -31378,
    -29122,
    -28502,
    -20676,
    -19841,
    -19630,
    -19342,
    -18109,
    -18045,
    -16750,
    -15615,
    -14910,
    -9755,
    -7432,
    -3368,
    1246,
    2906,
    3736,
    4868,
    6607,
    7100,
    7151,
    8954,
    10003,
    16011,
    17103,
    25321,
    27672,
    29544,
    29881,
    33437,
    33830,
    37541,
    39512,
    40169,
    42016,
    44751,
    45642,
    46490,
    47268,
    48191,
    49208,
    50412,
    55094,
    57101,
    57779,
    59695,
    59771,
    61237,
    63573,
    66015,
    67744,
    68295,
    68456,
    69606,
    71694,
    72500,
    72519,
    76279,
    77321,
    77981,
    78676,
    80180,
    81516,
    83384,
    85036,
    89078,
    94253,
    96287,
    96815,
    96827,
    97161,
    99492,
  };
  
  perf_marker( 1555 );
  
  //test_stats_on( temp );
  quick_sort( data, data_size );
  //test_stats_off( temp );

  perf_marker( 1666 );
  
  verify_results( data, ref, data_size );

  return 0;

}

