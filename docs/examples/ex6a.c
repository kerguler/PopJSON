#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 17
#define NumInt 0
#define NumEnv 0


#define adult_mort 0
#define adult_mort_3 1
#define adult_mort_4 2
#define adult_mort_5 3
#define adult_mort_6 4
#define adult_mort_7 5
#define adult_mort_8 6
#define adult_mort_9 7
#define adult_mort_10 8
#define adult_mort_1_3 0
#define adult_mort_1_4 0
#define adult_mort_1_5 0
#define adult_mort_1_6 0
#define adult_mort_1_7 0
#define adult_mort_1_8 0
#define adult_mort_1_9 0
#define adult_mort_1_10 0
#define adult_mort_2_3 0
#define adult_mort_2_4 0
#define adult_mort_2_5 0
#define adult_mort_2_6 0
#define adult_mort_2_7 0
#define adult_mort_2_8 0
#define adult_mort_2_9 0
#define adult_mort_2_10 0


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

int TIME;

double *model_param;



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
        "adult", "egg_1_3", "egg_1_4", "egg_1_5", "egg_1_6", "egg_1_7", "egg_1_8", "egg_1_9", "egg_1_10", "egg_2_3", "egg_2_4", "egg_2_5", "egg_2_6", "egg_2_7", "egg_2_8", "egg_2_9", "egg_2_10",
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
    population egg_1_3;
    population egg_1_4;
    population egg_1_5;
    population egg_1_6;
    population egg_1_7;
    population egg_1_8;
    population egg_1_9;
    population egg_1_10;
    population egg_2_3;
    population egg_2_4;
    population egg_2_5;
    population egg_2_6;
    population egg_2_7;
    population egg_2_8;
    population egg_2_9;
    population egg_2_10;

    number num = numZERO;
    char arbiters[10];
    number key[10];
    number size_adult;
    number size_egg_1_3;
    number size_egg_1_4;
    number size_egg_1_5;
    number size_egg_1_6;
    number size_egg_1_7;
    number size_egg_1_8;
    number size_egg_1_9;
    number size_egg_1_10;
    number size_egg_2_3;
    number size_egg_2_4;
    number size_egg_2_5;
    number size_egg_2_6;
    number size_egg_2_7;
    number size_egg_2_8;
    number size_egg_2_9;
    number size_egg_2_10;
    number completed_adult[10];
    number completed_egg_1_3[10];
    number completed_egg_1_4[10];
    number completed_egg_1_5[10];
    number completed_egg_1_6[10];
    number completed_egg_1_7[10];
    number completed_egg_1_8[10];
    number completed_egg_1_9[10];
    number completed_egg_1_10[10];
    number completed_egg_2_3[10];
    number completed_egg_2_4[10];
    number completed_egg_2_5[10];
    number completed_egg_2_6[10];
    number completed_egg_2_7[10];
    number completed_egg_2_8[10];
    number completed_egg_2_9[10];
    number completed_egg_2_10[10];
    double par[18];

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
        egg_1_3 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_4 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_5 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_6 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_7 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_8 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_9 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_1_10 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_3 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_4 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_5 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_6 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_7 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_8 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_9 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        egg_2_10 = spop2_loadstate(buff);


        free(buff);

    } else {
        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = ACC_ERLANG;
        key[1] = numZERO;
        arbiters[2] = ACC_ERLANG;
        key[2] = numZERO;
        arbiters[3] = ACC_ERLANG;
        key[3] = numZERO;
        arbiters[4] = ACC_ERLANG;
        key[4] = numZERO;
        arbiters[5] = ACC_ERLANG;
        key[5] = numZERO;
        arbiters[6] = ACC_ERLANG;
        key[6] = numZERO;
        arbiters[7] = ACC_ERLANG;
        key[7] = numZERO;
        arbiters[8] = ACC_ERLANG;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        adult = spop2_init(arbiters, DETERMINISTIC);
        if (y0[0]) { num.d = y0[0]; spop2_add(adult, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_3 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[1]) { num.d = y0[1]; spop2_add(egg_1_3, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_4 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[2]) { num.d = y0[2]; spop2_add(egg_1_4, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_5 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[3]) { num.d = y0[3]; spop2_add(egg_1_5, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_6 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[4]) { num.d = y0[4]; spop2_add(egg_1_6, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_7 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[5]) { num.d = y0[5]; spop2_add(egg_1_7, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_8 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[6]) { num.d = y0[6]; spop2_add(egg_1_8, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_9 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[7]) { num.d = y0[7]; spop2_add(egg_1_9, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_1_10 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[8]) { num.d = y0[8]; spop2_add(egg_1_10, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_3 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[9]) { num.d = y0[9]; spop2_add(egg_2_3, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_4 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[10]) { num.d = y0[10]; spop2_add(egg_2_4, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_5 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[11]) { num.d = y0[11]; spop2_add(egg_2_5, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_6 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[12]) { num.d = y0[12]; spop2_add(egg_2_6, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_7 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[13]) { num.d = y0[13]; spop2_add(egg_2_7, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_8 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[14]) { num.d = y0[14]; spop2_add(egg_2_8, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_9 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[15]) { num.d = y0[15]; spop2_add(egg_2_9, key, num); }


        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        arbiters[3] = STOP;
        key[3] = numZERO;
        arbiters[4] = STOP;
        key[4] = numZERO;
        arbiters[5] = STOP;
        key[5] = numZERO;
        arbiters[6] = STOP;
        key[6] = numZERO;
        arbiters[7] = STOP;
        key[7] = numZERO;
        arbiters[8] = STOP;
        key[8] = numZERO;
        arbiters[9] = STOP;
        key[9] = numZERO;
        egg_2_10 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[16]) { num.d = y0[16]; spop2_add(egg_2_10, key, num); }


    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

    size_adult = spop2_size(adult);
    size_egg_1_3 = spop2_size(egg_1_3);
    size_egg_1_4 = spop2_size(egg_1_4);
    size_egg_1_5 = spop2_size(egg_1_5);
    size_egg_1_6 = spop2_size(egg_1_6);
    size_egg_1_7 = spop2_size(egg_1_7);
    size_egg_1_8 = spop2_size(egg_1_8);
    size_egg_1_9 = spop2_size(egg_1_9);
    size_egg_1_10 = spop2_size(egg_1_10);
    size_egg_2_3 = spop2_size(egg_2_3);
    size_egg_2_4 = spop2_size(egg_2_4);
    size_egg_2_5 = spop2_size(egg_2_5);
    size_egg_2_6 = spop2_size(egg_2_6);
    size_egg_2_7 = spop2_size(egg_2_7);
    size_egg_2_8 = spop2_size(egg_2_8);
    size_egg_2_9 = spop2_size(egg_2_9);
    size_egg_2_10 = spop2_size(egg_2_10);

    ret[0] = size_adult.d;
    if (CHECK(ret[0])) {goto endall;};
    ret[1] = size_egg_1_3.d;
    if (CHECK(ret[1])) {goto endall;};
    ret[2] = size_egg_1_4.d;
    if (CHECK(ret[2])) {goto endall;};
    ret[3] = size_egg_1_5.d;
    if (CHECK(ret[3])) {goto endall;};
    ret[4] = size_egg_1_6.d;
    if (CHECK(ret[4])) {goto endall;};
    ret[5] = size_egg_1_7.d;
    if (CHECK(ret[5])) {goto endall;};
    ret[6] = size_egg_1_8.d;
    if (CHECK(ret[6])) {goto endall;};
    ret[7] = size_egg_1_9.d;
    if (CHECK(ret[7])) {goto endall;};
    ret[8] = size_egg_1_10.d;
    if (CHECK(ret[8])) {goto endall;};
    ret[9] = size_egg_2_3.d;
    if (CHECK(ret[9])) {goto endall;};
    ret[10] = size_egg_2_4.d;
    if (CHECK(ret[10])) {goto endall;};
    ret[11] = size_egg_2_5.d;
    if (CHECK(ret[11])) {goto endall;};
    ret[12] = size_egg_2_6.d;
    if (CHECK(ret[12])) {goto endall;};
    ret[13] = size_egg_2_7.d;
    if (CHECK(ret[13])) {goto endall;};
    ret[14] = size_egg_2_8.d;
    if (CHECK(ret[14])) {goto endall;};
    ret[15] = size_egg_2_9.d;
    if (CHECK(ret[15])) {goto endall;};
    ret[16] = size_egg_2_10.d;
    if (CHECK(ret[16])) {goto endall;};

    ret += 17;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 20;
                par[1] = 5;
                par[2] = 20;
                par[3] = 5;
                par[4] = 20;
                par[5] = 5;
                par[6] = 20;
                par[7] = 5;
                par[8] = 20;
                par[9] = 5;
                par[10] = 20;
                par[11] = 5;
                par[12] = 20;
                par[13] = 5;
                par[14] = 20;
                par[15] = 5;
                par[16] = 20;
                par[17] = 5;
                spop2_step(adult, par, &size_adult, completed_adult, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_3, par, &size_egg_1_3, completed_egg_1_3, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_4, par, &size_egg_1_4, completed_egg_1_4, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_5, par, &size_egg_1_5, completed_egg_1_5, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_6, par, &size_egg_1_6, completed_egg_1_6, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_7, par, &size_egg_1_7, completed_egg_1_7, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_8, par, &size_egg_1_8, completed_egg_1_8, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_9, par, &size_egg_1_9, completed_egg_1_9, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_1_10, par, &size_egg_1_10, completed_egg_1_10, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_3, par, &size_egg_2_3, completed_egg_2_3, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_4, par, &size_egg_2_4, completed_egg_2_4, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_5, par, &size_egg_2_5, completed_egg_2_5, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_6, par, &size_egg_2_6, completed_egg_2_6, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_7, par, &size_egg_2_7, completed_egg_2_7, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_8, par, &size_egg_2_8, completed_egg_2_8, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_9, par, &size_egg_2_9, completed_egg_2_9, 0);

                par[0] = 20;
                par[1] = 5;
                par[2] = 0.0;
                par[3] = 0.0;
                par[4] = 0.0;
                par[5] = 0.0;
                par[6] = 0.0;
                par[7] = 0.0;
                par[8] = 0.0;
                par[9] = 0.0;
                par[10] = 0.0;
                par[11] = 0.0;
                par[12] = 0.0;
                par[13] = 0.0;
                par[14] = 0.0;
                par[15] = 0.0;
                par[16] = 0.0;
                par[17] = 0.0;
                spop2_step(egg_2_10, par, &size_egg_2_10, completed_egg_2_10, 0);




        }

        ret[0] = size_adult.d;
        if (CHECK(ret[0])) {goto endall;};
        ret[1] = size_egg_1_3.d;
        if (CHECK(ret[1])) {goto endall;};
        ret[2] = size_egg_1_4.d;
        if (CHECK(ret[2])) {goto endall;};
        ret[3] = size_egg_1_5.d;
        if (CHECK(ret[3])) {goto endall;};
        ret[4] = size_egg_1_6.d;
        if (CHECK(ret[4])) {goto endall;};
        ret[5] = size_egg_1_7.d;
        if (CHECK(ret[5])) {goto endall;};
        ret[6] = size_egg_1_8.d;
        if (CHECK(ret[6])) {goto endall;};
        ret[7] = size_egg_1_9.d;
        if (CHECK(ret[7])) {goto endall;};
        ret[8] = size_egg_1_10.d;
        if (CHECK(ret[8])) {goto endall;};
        ret[9] = size_egg_2_3.d;
        if (CHECK(ret[9])) {goto endall;};
        ret[10] = size_egg_2_4.d;
        if (CHECK(ret[10])) {goto endall;};
        ret[11] = size_egg_2_5.d;
        if (CHECK(ret[11])) {goto endall;};
        ret[12] = size_egg_2_6.d;
        if (CHECK(ret[12])) {goto endall;};
        ret[13] = size_egg_2_7.d;
        if (CHECK(ret[13])) {goto endall;};
        ret[14] = size_egg_2_8.d;
        if (CHECK(ret[14])) {goto endall;};
        ret[15] = size_egg_2_9.d;
        if (CHECK(ret[15])) {goto endall;};
        ret[16] = size_egg_2_10.d;
        if (CHECK(ret[16])) {goto endall;};

        ret += 17;



        iret += 0;

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

            buffsz = spop2_buffsize(egg_1_3);
            buff = spop2_savestate(egg_1_3);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_4);
            buff = spop2_savestate(egg_1_4);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_5);
            buff = spop2_savestate(egg_1_5);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_6);
            buff = spop2_savestate(egg_1_6);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_7);
            buff = spop2_savestate(egg_1_7);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_8);
            buff = spop2_savestate(egg_1_8);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_9);
            buff = spop2_savestate(egg_1_9);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_1_10);
            buff = spop2_savestate(egg_1_10);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_3);
            buff = spop2_savestate(egg_2_3);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_4);
            buff = spop2_savestate(egg_2_4);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_5);
            buff = spop2_savestate(egg_2_5);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_6);
            buff = spop2_savestate(egg_2_6);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_7);
            buff = spop2_savestate(egg_2_7);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_8);
            buff = spop2_savestate(egg_2_8);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_9);
            buff = spop2_savestate(egg_2_9);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(egg_2_10);
            buff = spop2_savestate(egg_2_10);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            fclose(file);
        }
    }

    spop2_free(&adult);
    spop2_free(&egg_1_3);
    spop2_free(&egg_1_4);
    spop2_free(&egg_1_5);
    spop2_free(&egg_1_6);
    spop2_free(&egg_1_7);
    spop2_free(&egg_1_8);
    spop2_free(&egg_1_9);
    spop2_free(&egg_1_10);
    spop2_free(&egg_2_3);
    spop2_free(&egg_2_4);
    spop2_free(&egg_2_5);
    spop2_free(&egg_2_6);
    spop2_free(&egg_2_7);
    spop2_free(&egg_2_8);
    spop2_free(&egg_2_9);
    spop2_free(&egg_2_10);

}

int main(int argc, char *argv[]) {
    return 0;
}


