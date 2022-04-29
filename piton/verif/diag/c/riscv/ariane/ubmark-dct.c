//#include "ubmark.h"
#include "math.h"

#define PI 3.14159265358979323846

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

static double c[8][8]; /* transform coefficients */

void init_dct()
{
  int i, j;
  double s;

  for (i=0; i<8; i++) {
    s = (i==0) ? sqrt(0.125) : 0.5;
    
    for (j=0; j<8; j++)
      c[i][j] = s * cos((PI/8.0)*i*(j+0.5));
  }
}

void dct(int* block)
{
  int i, j, k;
  double s;
  double tmp[64];
  
  for (i=0; i<8; i++)
    for (j=0; j<8; j++) {
      s = 0.0;
	
      for (k=0; k<8; k++)
        s += c[j][k] * block[8*i+k];
	
      tmp[8*i+j] = s;
    }
  
  for (j=0; j<8; j++)
    for (i=0; i<8; i++) {
      s = 0.0;
	
      for (k=0; k<8; k++)
        s += c[i][k] * tmp[8*k+j];
	
      block[8*i+j] = (int)floor(s+0.499999);
    }
}

int main(int argc, char* argv[])
{
    int k, m;
    int b[8][8];
    int* p = &(b[0][0]);

    //test_stats_on( 0 );

    init_dct();

    for (k=0; k<64; k++) {
        if (k<32) {
            m = 2 * k;
        }
        else {
            m = -k+64;
        }
        
	p[k] = m;
    }
    
    dct(p);
    //test_stats_off( 0 );

    //test_pass( 0 );
    pass();
    return(0); 
 
}
