#include <math.h>
#include "population.h"

extern gsl_rng *RANDOM;

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 4
#define NumInt 1
#define NumEnv 0


#define adult_dev 0
#define harvest 0


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

int TIME;

double *model_param;

unsigned int adult_developing;


void fun_harvest_adult_mortality(number *key, number num, number *newkey, double *frac) {
    *frac = 0.1;
}

void fun_harvest_adult_harvest(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[adult_dev].d;
    *frac = 0.1;
}

void init(int *no, int *np, int *ni, int *ne, int *st) {
    spop2_random_init();

    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
    *ne = NumEnv;
    *st = 1;
}

void parnames(char **names, double *param, double *parmin, double *parmax) {
    char temp[NumPop+NumPar+NumInt+NumEnv][256] = {
        "adult", "adult_developed", "adult_dead", "adult_harvested",
        "adult_developing",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt+NumEnv); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
    spop2_random_destroy();
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {

    TIME = 0;

    model_param = pr;

    population adult;
    population adult_developed;
    population adult_dead;
    population adult_harvested;

    number num = numZERO;
    char arbiters[2];
    number key[2];
    number size_adult;
    number size_adult_developed;
    number size_adult_dead;
    number size_adult_harvested;
    number completed_adult[2];
    number completed_adult_harvested[2];
    adult_developing = 0;
    double par[2];

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


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_developed = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_dead = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_harvested = spop2_loadstate(buff);


        free(buff);

    } else {
        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult = spop2_init(arbiters, STOCHASTIC);
        if (y0[0]) { num.i = y0[0]; spop2_add(adult, key, num); }


        arbiters[0] = STOP;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_developed = spop2_init(arbiters, STOCHASTIC);
        if (y0[1]) { num.i = y0[1]; spop2_add(adult_developed, key, num); }


        arbiters[0] = STOP;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_dead = spop2_init(arbiters, STOCHASTIC);
        if (y0[2]) { num.i = y0[2]; spop2_add(adult_dead, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_harvested = spop2_init(arbiters, STOCHASTIC);
        if (y0[3]) { num.i = y0[3]; spop2_add(adult_harvested, key, num); }


    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

    size_adult = spop2_size(adult);
    size_adult_developed = spop2_size(adult_developed);
    size_adult_dead = spop2_size(adult_dead);
    size_adult_harvested = spop2_size(adult_harvested);

    ret[0] = (double)(size_adult.i);
    if (CHECK(ret[0])) {goto endall;};
    ret[1] = (double)(size_adult_developed.i);
    if (CHECK(ret[1])) {goto endall;};
    ret[2] = (double)(size_adult_dead.i);
    if (CHECK(ret[2])) {goto endall;};
    ret[3] = (double)(size_adult_harvested.i);
    if (CHECK(ret[3])) {goto endall;};

    ret += 4;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 5;
                par[1] = 1;
                spop2_step(adult, par, &size_adult, completed_adult, 0);

                par[0] = 0;
                par[1] = 0.0;
                spop2_step(adult_harvested, par, &size_adult_harvested, completed_adult_harvested, 0);

                adult_developing = (0.5 * completed_adult[adult_dev].i);

                key[0] = numZERO;
                key[1] = numZERO;
                num.i = adult_developing;
                spop2_add(adult, key, num);

                size_adult.i += adult_developing;

                spop2_harvest(adult, adult_dead, fun_harvest_adult_mortality);
                spop2_harvest(adult, adult_harvested, fun_harvest_adult_harvest);

                size_adult_dead = spop2_size(adult_dead);
                size_adult_harvested = spop2_size(adult_harvested);

        }

        ret[0] = (double)(size_adult.i);
        if (CHECK(ret[0])) {goto endall;};
        ret[1] = (double)(size_adult_developed.i);
        if (CHECK(ret[1])) {goto endall;};
        ret[2] = (double)(size_adult_dead.i);
        if (CHECK(ret[2])) {goto endall;};
        ret[3] = (double)(size_adult_harvested.i);
        if (CHECK(ret[3])) {goto endall;};

        ret += 4;


        iret[0] = adult_developing;
        if (CHECK(iret[0])) {goto endall;};

        iret += 1;

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

            buffsz = spop2_buffsize(adult_developed);
            buff = spop2_savestate(adult_developed);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_dead);
            buff = spop2_savestate(adult_dead);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_harvested);
            buff = spop2_savestate(adult_harvested);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            fclose(file);
        }
    }

    spop2_free(&adult);
    spop2_free(&adult_developed);
    spop2_free(&adult_dead);
    spop2_free(&adult_harvested);

}

int main(int argc, char *argv[]) {
    return 0;
}


