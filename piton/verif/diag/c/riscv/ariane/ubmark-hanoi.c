 //========================================================================
 // towers of hanoi
 //======================================================================== 

//#include "ubmark.h"

__attribute__ ((noinline))
void move(int n, int from, int to, int via)
{
  if (n > 0) {
    move(n - 1, from, via, to);
    move(n - 1, via, to, from);
  }
}

int main()
{
  //test_stats_on( 0 );
  move(7,1,2,3);
  //test_stats_off( 0 );

  //test_pass(0);
  pass();
  //return 0;
}
