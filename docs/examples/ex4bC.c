#include <math.h>
#include "population.h"

extern gsl_rng *RANDOM;

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 3


#define adult_mort 0
#define adult_dev 1
#define adult_num_dev 2


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }


void fun_transfer_gonotrophic_cycle(number *key, number num, void *pop) {
    number q[4] = {
        {.d=key[adult_mort].d},
        {.d=0},
        {.i=(key[adult_num_dev].i + 1)},
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
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {

    int TIME = 0;


    population adult;

    number num = numZERO;
    char arbiters[4];
    number key[4];
    number size_adult;
    number completed_adult[4];
    population popdone_adult[4];
    unsigned int adult_death = 0;
    unsigned int num_gravid = 0;
    unsigned int egg_laying = 0;
    double par[5];

    FILE *file;
    number *buff = 0;
    unsigned int buffsz = 0;
    if (file0 && file0[0]!=' ') {
        file = fopen(file0,"rb");
        if (!file) {
            *success = 0;
            goto endall;
        }
        rewind(file);
    }

    if (file0 && file0[0]!=' ') {
        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult = spop2_loadstate(buff);

        popdone_adult[0] = spop2_loadstate_empty(buff);
        popdone_adult[1] = spop2_loadstate_empty(buff);
        popdone_adult[2] = spop2_loadstate_empty(buff);
        popdone_adult[3] = spop2_loadstate_empty(buff);

        free(buff);

    } else {
        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = ACC_ERLANG;
        key[1] = numZERO;
        arbiters[2] = AGE_CUSTOM;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        adult = spop2_init(arbiters, STOCHASTIC);
        adult->arbiters[2]->fun_step = 0;
        if (y0[0]) { num.i = y0[0]; spop2_add(adult, key, num); }

        popdone_adult[0] = spop2_init(arbiters, STOCHASTIC);
        popdone_adult[1] = spop2_init(arbiters, STOCHASTIC);
        popdone_adult[2] = spop2_init(arbiters, STOCHASTIC);
        popdone_adult[3] = spop2_init(arbiters, STOCHASTIC);
    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

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
                par[4] = 0;
                spop2_step(adult, par, &size_adult, completed_adult, popdone_adult);

                adult_death = completed_adult[adult_mort].i;
                num_gravid = completed_adult[adult_dev].i;
                egg_laying = (unsigned int)gsl_ran_poisson(RANDOM, (num_gravid * 10));


                spop2_foreach(popdone_adult[adult_dev], fun_transfer_gonotrophic_cycle, (void *)(&adult));

                spop2_empty(&popdone_adult[0]);
                spop2_empty(&popdone_adult[1]);
                spop2_empty(&popdone_adult[2]);
                spop2_empty(&popdone_adult[3]);

                size_adult = spop2_size(adult);

        }

        ret[0] = (double)(size_adult.i);
        if (CHECK(ret[0])) {goto endall;};

        printf("%u %g\n",TIME,ret[0]);

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

    if (file1 && file1[0]!=' ') {
        file = fopen(file1,"wb");
        if (!file) {
            *success = 0;
        } else {
            rewind(file);

            buffsz = spop2_buffsize(adult);
            buff = spop2_savestate(adult);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            fclose(file);
        }
    }

    spop2_printable(adult,0);

    spop2_free(&adult);
    spop2_free(&(popdone_adult[0]));
    spop2_free(&(popdone_adult[1]));
    spop2_free(&(popdone_adult[2]));
    spop2_free(&(popdone_adult[3]));

}

#define TF 20

int main(int argc, char *argv[]) {
    spop2_random_init();

    double y0 = 100.0;
    double ret[1*(TF+1)];
    double iret[3*(TF+1)];
    int success;
    sim(TF, 1, 0, 0, &y0, 0, 0, ret, iret, &success);
    return 0;
}
