#include <math.h>
#include "population.h"

extern gsl_rng *RANDOM;

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 0


#define larva_dev 0


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }


void init(int *no, int *np, int *ni) {
    spop2_random_init();

    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
}

void parnames(char **names, double *param, double *parmin, double *parmax) {
    char temp[NumPop+NumPar+NumInt][256] = {
        "larva",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
    spop2_random_destroy();
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *iret, int *success) {

    population larva;

    number num = numZERO;
    char arbiters[2];
    number key[2];
    number size_larva;
    number completed_larva[2];
    double par[2];

    arbiters[0] = ACC_ERLANG;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    larva = spop2_init(arbiters, STOCHASTIC);
    if (y0[0]) { num.i = y0[0]; spop2_add(larva, key, num); }



    int TIME = 0;
    size_larva = spop2_size(larva);

    ret[0] = (double)(size_larva.i);
    if (CHECK(ret[0])) {goto endall;};

    ret += 1;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 10;
                par[1] = 4;
                spop2_step(larva, par, &size_larva, completed_larva, 0);





        }

        ret[0] = (double)(size_larva.i);
        if (CHECK(ret[0])) {goto endall;};

        ret += 1;



        iret += 0;

    }

  endall:

    *success = TIME;

    spop2_free(&larva);

}

int main(int argc, char *argv[]) {
    return 0;
}