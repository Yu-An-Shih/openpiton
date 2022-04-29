//========================================================================
// ubmark-cmlpx-mult
//========================================================================

//#include "ubmark.h"
//#include "ubmark-cmplx-mult.dat"

void uart_print(char* str){
  static char* uart_base = (char*)(0xfff0c2c000);
  unsigned i = 0;
  while(str[i] != 0){
    *uart_base = str[i];
    ++i;
  }
}

void uart_putchar(char c){
  static char* uart_base = (char*)(0xfff0c2c000);
  *uart_base = c;
}


//------------------------------------------------------------------------
// cmplx-mult-scalar
//------------------------------------------------------------------------

void gen( int dest[], int size )
{
  int i;
  for ( i = 0; i < size; i++ ) {
    // dest[i*2]   = (src0[i*2] * src1[i*2]) - (src0[i*2+1] * src1[i*2+1]);
    // dest[i*2+1] = (src0[i*2] * src1[i*2+1]) + (src0[i*2+1] * src1[i*2]);
    const unsigned a = (i + 7) * 2;
    const unsigned b = (i + 13) * 2;
    const unsigned c = (i + 17) * 2;
    const unsigned d = (i + 23) * 2;
    dest[i*2]   = (a * c) - (b * d);
    dest[i*2+1] = (a * d) - (b * c);
  }
}

//------------------------------------------------------------------------
// verify_results
//------------------------------------------------------------------------

void verify_results( int dest[], int size )
{
  int temp = 0;
  int i;
  for ( i = 0; i < size; i++ ) {
    // if ( !( ( dest[i*2] == ref[i*2] ) && ( dest[i*2+1] == ref[i*2+1] ) ) ) {
    //   //test_fail( temp );
    //   fail();
    // }
    const unsigned a = (i + 7) * 2;
    const unsigned b = (i + 13) * 2;
    const unsigned c = (i + 17) * 2;
    const unsigned d = (i + 23) * 2;
    unsigned res0 = (a * c) - (b * d);
    unsigned res1 = (a * d) - (b * c);
    if ( (res0 != dest[i*2]) || (res1 != dest[i*2+1]))
      fail();
  }
  //pass();
  //test_pass( temp );
}

//------------------------------------------------------------------------
// Test harness
//------------------------------------------------------------------------

static const unsigned kIntSize = 4;
static const unsigned kL1Size = 8*1024/4/2;
static const unsigned kL2Size = 64*1024/4/2;
static const unsigned kL3Size = 25*64*1024/4/2;
int dest_ [409600*2];

int main( int argc, char* argv[] )
{
    uart_print("Test ubmark-cmplx-mult starts\n");
    const int size = 100;
    //int dest[size*2];

    unsigned j = 0;
    // while (1) {
      // unsigned k = 0;
      // for (k = 0; k < 1000; ++k){
        unsigned i = 0;
  	    for ( i = 0; i < kL1Size*2; i++ )
  	      dest_[i] = 0;
  	    gen( dest_, kL1Size );
  	    verify_results( dest_, kL1Size );
      // }
      uart_print("Checked L1. ");

      // for (k = 0; k < 1000; ++k){
      //   unsigned i = 0;
      //   for ( i = 0; i < kL2Size*2; i++ )
      //     dest_[i] = 0;
      //   gen( dest_, kL2Size );
      //   verify_results( dest_, kL2Size );
      // }
      // uart_print("Checked L2. ");
      
      // for (k = 0; k < 1000; ++k){
      //   unsigned i = 0;
      //   for ( i = 0; i < kL3Size*2; i++ )
      //     dest_[i] = 0;
      //   gen( dest_, kL3Size );
      //   verify_results( dest_, kL3Size );
      // }
      // uart_print("Checked L3. ");

      // j += 1;
      // if (j % 10000 == 0)
      //    uart_print("...loop\r\n");
    // }

    pass();
    return 0;
}

