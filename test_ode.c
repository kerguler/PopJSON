#include <stdio.h>
#include <stdlib.h>
#include "lsoda.h"

int fex(double t, double *y, double *ydot, void *data)
{
	ydot[0] = 1.0E4 * y[1] * y[2] - .04E0 * y[0];
	ydot[2] = 3.0E7 * y[1] * y[1];
	ydot[1] = -1.0 * (ydot[0] + ydot[2]);
	return(0);
}

int test(void)
{
	double          atol[3], rtol[3], t, tout, y[3];
	int             neq = 3;
	int             iout;

	y[0] = 1.0E0;
	y[1] = 0.0E0;
	y[2] = 0.0E0;
	t = 0.0E0;
	tout = 0.4E0;
	struct lsoda_opt_t opt = {0};
	opt.ixpr = 0;
	opt.rtol = rtol;
	opt.atol = atol;
	opt.itask = 1;
	
	rtol[0] = rtol[2] = 1.0E-4;
	rtol[1] = 1.0E-4;
	atol[0] = 1.0E-6;
	atol[1] = 1.0E-10;
	atol[2] = 1.0E-6;


	struct lsoda_context_t ctx = {
		.function = fex,
		.neq = neq,
		.data = NULL,
		.state = 1,
	};

	lsoda_prepare(&ctx, &opt);

	for (iout = 1; iout <= 12; iout++) {
		lsoda(&ctx, y, &t, tout);
		printf(" at t= %12.4e y= %14.6e %14.6e %14.6e\n", t, y[0], y[1], y[2]);
		if (ctx.state <= 0) {
			printf("error istate = %d\n", ctx.state);
			exit(0);
		}
		tout = tout * 10.0E0;
	}
	lsoda_free(&ctx);
	return(0);
}

int main(void) {
	int i;
	for(i = 0; i < 1; i++) {
		test();
	}
	return(0);
}
