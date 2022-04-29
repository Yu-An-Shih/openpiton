//========================================================================
// ubmark-euler.c
//========================================================================
//#include "ubmark.h"
//#include <math.h>
 
typedef double (*deriv_f)(double, double);
 
 __attribute__ ((noinline))
double ivp_euler(deriv_f f, double y, int step, int end_t)
{
	int t = 0;
 
	do {
		y += step * f(t, y);
	} while ((t += step) <= end_t);

	return y;
}

double cooling(double t, double temp)
{
	return -0.07 * (temp - 20);
}
 
int main()
{
	double result[3];

	//test_stats_on( 0 );
	// result[0] = ivp_euler(cooling, 100, 2, 100);
	// result[1] = ivp_euler(cooling, 100, 5, 100);
	result[2] = ivp_euler(cooling, 100, 10, 100);
	//test_stats_off( 0 );
 
 	//test_pass( 0 );
    pass();
	return 0;
}
