#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 3
#define NumEnv 0


#define adult_mort 0
#define adult_dev 1


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

int TIME;

double *model_param;

double adult_death;
double num_gravid;
double egg_laying;


void fun_harvest_gonotrophic_cycle(number *key, number num, number *newkey, double *frac) {
    newkey[0].d=key[adult_mort].d;
    newkey[1].d=0;
    *frac = 1.0;
}

void init(int *no, int *np, int *ni, int *ne, int *st) {
    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
    *ne = NumEnv;
    *st = 0;
}

void parnames(char **names, double *param, double *parmin, double *parmax) {
    char temp[NumPop+NumPar+NumInt+NumEnv][256] = {
        "adult",
        "adult_death", "num_gravid", "egg_laying",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt+NumEnv); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {

    TIME = 0;

    model_param = pr;

    population adult;

    number num = numZERO;
    char arbiters[3];
    number key[3];
    number size_adult;
    number completed_adult[3];
    population popdone_adult[3];
    adult_death = 0.0;
    num_gravid = 0.0;
    egg_laying = 0.0;
    double par[4];

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

        free(buff);

    } else {
        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = ACC_ERLANG;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        adult = spop2_init(arbiters, DETERMINISTIC);
        if (y0[0]) { num.d = y0[0]; spop2_add(adult, key, num); }

        popdone_adult[0] = spop2_init(arbiters, DETERMINISTIC);
        popdone_adult[1] = spop2_init(arbiters, DETERMINISTIC);
        popdone_adult[2] = spop2_init(arbiters, DETERMINISTIC);

    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

    size_adult = spop2_size(adult);

    ret[0] = size_adult.d;
    if (CHECK(ret[0])) {goto endall;};

    ret += 1;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 20;
                par[1] = 5;
                par[2] = 5;
                par[3] = 1;
                spop2_step(adult, par, &size_adult, completed_adult, popdone_adult);

                adult_death = completed_adult[adult_mort].d;

                num_gravid = completed_adult[adult_dev].d;

                egg_laying = (num_gravid * 10);


                spop2_harvest(popdone_adult[adult_dev], adult, fun_harvest_gonotrophic_cycle);

                size_adult = spop2_size(adult);

        }

        ret[0] = size_adult.d;
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

    spop2_free(&adult);
    spop2_free(&(popdone_adult[0]));
    spop2_free(&(popdone_adult[1]));
    spop2_free(&(popdone_adult[2]));

}

int main(int argc, char *argv[]) {
    return 0;
}


