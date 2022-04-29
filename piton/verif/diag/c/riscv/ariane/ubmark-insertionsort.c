//========================================================================
// ubmark-insertionsort
//========================================================================

//#include "ubmark.h"
#include <stddef.h>
#include "ubmark-quicksort.dat"

//------------------------------------------------------------------------
// insertion_sort
//------------------------------------------------------------------------

__attribute__ ((noinline))
static void insertion_sort(int *a, const size_t n) {
    size_t i, j;
    int value;
    for (i = 1; i < n; i++) {
        value = a[i];
        for (j = i; j > 0 && value < a[j - 1]; j--) {
            a[j] = a[j - 1];
        }
        a[j] = value;
    }
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

  //test_stats_on( temp );
  insertion_sort( data, data_size );
  //test_stats_off( temp );

  verify_results( data, ref, data_size );

  return 0;

}
