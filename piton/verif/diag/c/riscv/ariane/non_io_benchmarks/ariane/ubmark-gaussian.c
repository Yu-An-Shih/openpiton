//========================================================================
// ubmark-gaussian.c
//========================================================================

//#include "ubmark.h"
#include <stdlib.h>
//#include <math.h>
 
#define mat_elem(a, y, x, n) (a + ((y) * (n) + (x)))
 
void swap_row(double *a, double *b, int r1, int r2, int n)
{
	double tmp, *p1, *p2;
	int i;
 
	if (r1 == r2) return;
	for (i = 0; i < n; i++) {
		p1 = mat_elem(a, r1, i, n);
		p2 = mat_elem(a, r2, i, n);
		tmp = *p1, *p1 = *p2, *p2 = tmp;
	}
	tmp = b[r1], b[r1] = b[r2], b[r2] = tmp;
}
 
void gauss_eliminate(double *a, double *b, double *x, int n)
{
#define A(y, x) (*mat_elem(a, y, x, n))
	int i, j, col, row, max_row;
	double max, tmp;
 
	for (row = 0; row < n; row++) {
		max_row = row, max = A(row, row);
 
		for (i = row + 1; i < n; i++)
			if ((tmp = fabs(A(i, row))) > max)
				max_row = i, max = tmp;
 
		swap_row(a, b, row, max_row, n);
 
		for (i = row + 1; i < n; i++) {
			tmp = A(i, row) / A(row, row);
			for (col = row+1; col < n; col++)
				A(i, col) -= tmp * A(row, col);
			A(i, row) = 0;
			b[i] -= tmp * b[row];
		}
	}
	for (row = n - 1; row >= 0; row--) {
		tmp = b[row];
		for (j = n - 1; j > row; j--)
			tmp -= x[j] * A(row, j);
		x[row] = tmp / A(row, row);
	}
#undef A
}
 
int main(void)
{
	//test_stats_on( 0 );

	double a[] = {
		1.00, 0.00, 0.00,  0.00,  0.00, 0.00,
		1.00, 0.63, 0.39,  0.25,  0.16, 0.10,
		1.00, 1.26, 1.58,  1.98,  2.49, 3.13,
		1.00, 1.88, 3.55,  6.70, 12.62, 23.80,
		1.00, 2.51, 6.32, 15.88, 39.90, 100.28,
		1.00, 3.14, 9.87, 31.01, 97.41, 306.02
	};
	double b[] = { -0.01, 0.61, 0.91, 0.99, 0.60, 0.02 };
	double x[6];
 
	gauss_eliminate(a, b, x, 6);
	//test_stats_off( 0 );

 	
 	//test_pass(0); 
    pass();
	return 0;
}
