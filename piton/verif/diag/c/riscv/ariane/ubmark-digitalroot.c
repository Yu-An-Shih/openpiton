//========================================================================
// ubmark-digitalroot.c
//========================================================================

//#include "ubmark.h"

int droot(long long int x, int base, int *pers)
{
	int d = 0;
	if (pers)
		for (*pers = 0; x >= base; x = d, (*pers)++)
			for (d = 0; x; d += x % base, x /= base);
	else if (x && !(d = x % (base - 1)))
			d = base - 1;
 
	return d;
}
 
int main(void)
{

	//test_stats_on( 0 );
	int i, d, pers;
	long long x[] = {627615, 39390, 588225, 393900588225LL};
 
	for (i = 0; i < 4; i++) {
		d = droot(x[i], 10, &pers);
	}

	//test_stats_off( 0 );

    //test_pass( 0 );
    pass();
    return(0); 
}
