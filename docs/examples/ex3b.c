#include <math.h>
#include "population.h"

extern gsl_rng *RANDOM;

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 3


#define adult_mort 0
#define adult_dev 1


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }


void fun_transfer_gonotrophic_cycle(number *key, number num, void *pop) {
    number q[3] = {
        {.d=key[adult_mort].d},
        {.d=0},
    };
    spop2_add(*(population *)pop, q, num);
}

void init(int *no, int *np, int *ni) {
    spop2_random_init();

    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
}

void parnames(char **names, double *param, double *parmin, double *parmax) {
    char temp[NumPop+NumPar+NumInt][256] = {
        "adult",
        "adult_death", "num_gravid", "egg_laying",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
    spop2_random_destroy();
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *iret, int *success) {

    population adult;

    number num = numZERO;
    char arbiters[3];
    number key[3];
    number size_adult;
    number completed_adult[3];
    unsigned int adult_death = 0;
    unsigned int num_gravid = 0;
    unsigned int egg_laying = 0;
    double par[4];

    arbiters[0] = ACC_ERLANG;
    key[0] = numZERO;
    arbiters[1] = ACC_ERLANG;
    key[1] = numZERO;
    arbiters[2] = STOP;
    key[2] = numZERO;
    adult = spop2_init(arbiters, STOCHASTIC);
    if (y0[0]) { num.i = y0[0]; spop2_add(adult, key, num); }

    population popdone_adult[3];
    popdone_adult[0] = spop2_init(arbiters, STOCHASTIC);
    popdone_adult[1] = spop2_init(arbiters, STOCHASTIC);
    popdone_adult[2] = spop2_init(arbiters, STOCHASTIC);


    int TIME = 0;
    size_adult = spop2_size(adult);

    ret[0] = (double)(size_adult.i);
    if (CHECK(ret[0])) {goto endall;};

    ret += 1;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 20;
                par[1] = 5;
                par[2] = 5;
                par[3] = 1;
                spop2_step(adult, par, &size_adult, completed_adult, popdone_adult);

                adult_death = completed_adult[adult_mort].i;
                num_gravid = completed_adult[adult_dev].i;
                egg_laying = (unsigned int)gsl_ran_poisson(RANDOM, (num_gravid * 10));


                spop2_foreach(popdone_adult[adult_dev], fun_transfer_gonotrophic_cycle, (void *)(&adult));

                spop2_empty(&popdone_adult[0]);
                spop2_empty(&popdone_adult[1]);
                spop2_empty(&popdone_adult[2]);

                size_adult = spop2_size(adult);

        }

        ret[0] = (double)(size_adult.i);
        if (CHECK(ret[0])) {goto endall;};

        ret += 1;


        iret[0] = adult_death;
        if (CHECK(iret[0])) {goto endall;};
        iret[1] = num_gravid;
        if (CHECK(iret[1])) {goto endall;};
        iret[2] = egg_laying;
        if (CHECK(iret[2])) {goto endall;};

        iret += 3;

    }

  endall:

    *success = TIME;

    spop2_free(&adult);
    spop2_free(&(popdone_adult[0]));
    spop2_free(&(popdone_adult[1]));
    spop2_free(&(popdone_adult[2]));

}

int main(int argc, char *argv[]) {
    return 0;
}
