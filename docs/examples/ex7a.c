#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 25
#define NumInt 0
#define NumEnv 1


#define adult_mort_0 0
#define adult_mort_1 0
#define adult_mort_2 0
#define adult_mort_3 0
#define adult_mort_4 0
#define adult_mort_5 0
#define adult_mort_6 0
#define adult_mort_7 0
#define adult_mort_8 0
#define adult_mort_9 0
#define adult_mort_10 0
#define adult_mort_11 0
#define adult_mort_12 0
#define adult_mort_13 0
#define adult_mort_14 0
#define adult_mort_15 0
#define adult_mort_16 0
#define adult_mort_17 0
#define adult_mort_18 0
#define adult_mort_19 0
#define adult_mort_20 0
#define adult_mort_21 0
#define adult_mort_22 0
#define adult_mort_23 0
#define adult_mort_24 0


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

int TIME;
int TIMEF;

double *model_param;
double *envir_tprob;


double tprob_adult_dispersion[25];


void fun_harvest_adult_dispersion_0(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[0];
}

void fun_harvest_adult_dispersion_1(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[1];
}

void fun_harvest_adult_dispersion_2(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[2];
}

void fun_harvest_adult_dispersion_3(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[3];
}

void fun_harvest_adult_dispersion_4(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[4];
}

void fun_harvest_adult_dispersion_5(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[5];
}

void fun_harvest_adult_dispersion_6(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[6];
}

void fun_harvest_adult_dispersion_7(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[7];
}

void fun_harvest_adult_dispersion_8(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[8];
}

void fun_harvest_adult_dispersion_9(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[9];
}

void fun_harvest_adult_dispersion_10(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[10];
}

void fun_harvest_adult_dispersion_11(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[11];
}

void fun_harvest_adult_dispersion_12(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[12];
}

void fun_harvest_adult_dispersion_13(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[13];
}

void fun_harvest_adult_dispersion_14(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[14];
}

void fun_harvest_adult_dispersion_15(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[15];
}

void fun_harvest_adult_dispersion_16(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[16];
}

void fun_harvest_adult_dispersion_17(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[17];
}

void fun_harvest_adult_dispersion_18(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[18];
}

void fun_harvest_adult_dispersion_19(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[19];
}

void fun_harvest_adult_dispersion_20(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[20];
}

void fun_harvest_adult_dispersion_21(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[21];
}

void fun_harvest_adult_dispersion_22(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[22];
}

void fun_harvest_adult_dispersion_23(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[23];
}

void fun_harvest_adult_dispersion_24(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[24];
}

void prepare_tprobs(int numcol, double *ttprobs, double *tprobs) {
    int rA, rB, i = 0;
    double sum;
    for (i=0, rB=0; rB<numcol; rB++) {
        sum = 1.0;
        for (rA=0; rA<numcol; rA++, i++) {
            tprobs[i] = sum <= 0.0 ? 1.0 : ttprobs[i] / sum;
            sum -= ttprobs[i];
        }
    }
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
        "adult_0", "adult_1", "adult_2", "adult_3", "adult_4", "adult_5", "adult_6", "adult_7", "adult_8", "adult_9", "adult_10", "adult_11", "adult_12", "adult_13", "adult_14", "adult_15", "adult_16", "adult_17", "adult_18", "adult_19", "adult_20", "adult_21", "adult_22", "adult_23", "adult_24",
        "tprob",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt+NumEnv); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {

    TIME = 0;
    TIMEF = tf;

    model_param = pr;

    envir_tprob = envir + 1; envir += (int)round(*envir) + 1;

    prepare_tprobs(5, envir_tprob, tprob_adult_dispersion);

    population adult_0;
    population adult_1;
    population adult_2;
    population adult_3;
    population adult_4;
    population adult_5;
    population adult_6;
    population adult_7;
    population adult_8;
    population adult_9;
    population adult_10;
    population adult_11;
    population adult_12;
    population adult_13;
    population adult_14;
    population adult_15;
    population adult_16;
    population adult_17;
    population adult_18;
    population adult_19;
    population adult_20;
    population adult_21;
    population adult_22;
    population adult_23;
    population adult_24;

    number num = numZERO;
    char arbiters[2];
    number key[2];
    number size_adult_0;
    number size_adult_1;
    number size_adult_2;
    number size_adult_3;
    number size_adult_4;
    number size_adult_5;
    number size_adult_6;
    number size_adult_7;
    number size_adult_8;
    number size_adult_9;
    number size_adult_10;
    number size_adult_11;
    number size_adult_12;
    number size_adult_13;
    number size_adult_14;
    number size_adult_15;
    number size_adult_16;
    number size_adult_17;
    number size_adult_18;
    number size_adult_19;
    number size_adult_20;
    number size_adult_21;
    number size_adult_22;
    number size_adult_23;
    number size_adult_24;
    number completed_adult_0[2];
    number completed_adult_1[2];
    number completed_adult_2[2];
    number completed_adult_3[2];
    number completed_adult_4[2];
    number completed_adult_5[2];
    number completed_adult_6[2];
    number completed_adult_7[2];
    number completed_adult_8[2];
    number completed_adult_9[2];
    number completed_adult_10[2];
    number completed_adult_11[2];
    number completed_adult_12[2];
    number completed_adult_13[2];
    number completed_adult_14[2];
    number completed_adult_15[2];
    number completed_adult_16[2];
    number completed_adult_17[2];
    number completed_adult_18[2];
    number completed_adult_19[2];
    number completed_adult_20[2];
    number completed_adult_21[2];
    number completed_adult_22[2];
    number completed_adult_23[2];
    number completed_adult_24[2];
    population popdummy_adult_0 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_1 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_2 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_3 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_4 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_5 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_6 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_7 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_8 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_9 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_10 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_11 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_12 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_13 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_14 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_15 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_16 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_17 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_18 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_19 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_20 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_21 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_22 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_23 = spop2_init(arbiters, DETERMINISTIC);
    population popdummy_adult_24 = spop2_init(arbiters, DETERMINISTIC);
    double par[1];

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
        adult_0 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_1 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_2 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_3 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_4 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_5 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_6 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_7 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_8 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_9 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_10 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_11 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_12 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_13 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_14 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_15 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_16 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_17 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_18 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_19 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_20 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_21 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_22 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_23 = spop2_loadstate(buff);


        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult_24 = spop2_loadstate(buff);


        free(buff);

    } else {
        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_0 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[0]) { num.d = y0[0]; spop2_add(adult_0, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_1 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[1]) { num.d = y0[1]; spop2_add(adult_1, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_2 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[2]) { num.d = y0[2]; spop2_add(adult_2, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_3 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[3]) { num.d = y0[3]; spop2_add(adult_3, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_4 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[4]) { num.d = y0[4]; spop2_add(adult_4, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_5 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[5]) { num.d = y0[5]; spop2_add(adult_5, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_6 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[6]) { num.d = y0[6]; spop2_add(adult_6, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_7 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[7]) { num.d = y0[7]; spop2_add(adult_7, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_8 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[8]) { num.d = y0[8]; spop2_add(adult_8, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_9 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[9]) { num.d = y0[9]; spop2_add(adult_9, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_10 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[10]) { num.d = y0[10]; spop2_add(adult_10, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_11 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[11]) { num.d = y0[11]; spop2_add(adult_11, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_12 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[12]) { num.d = y0[12]; spop2_add(adult_12, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_13 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[13]) { num.d = y0[13]; spop2_add(adult_13, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_14 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[14]) { num.d = y0[14]; spop2_add(adult_14, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_15 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[15]) { num.d = y0[15]; spop2_add(adult_15, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_16 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[16]) { num.d = y0[16]; spop2_add(adult_16, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_17 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[17]) { num.d = y0[17]; spop2_add(adult_17, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_18 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[18]) { num.d = y0[18]; spop2_add(adult_18, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_19 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[19]) { num.d = y0[19]; spop2_add(adult_19, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_20 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[20]) { num.d = y0[20]; spop2_add(adult_20, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_21 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[21]) { num.d = y0[21]; spop2_add(adult_21, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_22 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[22]) { num.d = y0[22]; spop2_add(adult_22, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_23 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[23]) { num.d = y0[23]; spop2_add(adult_23, key, num); }


        arbiters[0] = NOAGE_CONST;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        adult_24 = spop2_init(arbiters, DETERMINISTIC);
        if (y0[24]) { num.d = y0[24]; spop2_add(adult_24, key, num); }


    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

    size_adult_0 = spop2_size(adult_0);
    size_adult_1 = spop2_size(adult_1);
    size_adult_2 = spop2_size(adult_2);
    size_adult_3 = spop2_size(adult_3);
    size_adult_4 = spop2_size(adult_4);
    size_adult_5 = spop2_size(adult_5);
    size_adult_6 = spop2_size(adult_6);
    size_adult_7 = spop2_size(adult_7);
    size_adult_8 = spop2_size(adult_8);
    size_adult_9 = spop2_size(adult_9);
    size_adult_10 = spop2_size(adult_10);
    size_adult_11 = spop2_size(adult_11);
    size_adult_12 = spop2_size(adult_12);
    size_adult_13 = spop2_size(adult_13);
    size_adult_14 = spop2_size(adult_14);
    size_adult_15 = spop2_size(adult_15);
    size_adult_16 = spop2_size(adult_16);
    size_adult_17 = spop2_size(adult_17);
    size_adult_18 = spop2_size(adult_18);
    size_adult_19 = spop2_size(adult_19);
    size_adult_20 = spop2_size(adult_20);
    size_adult_21 = spop2_size(adult_21);
    size_adult_22 = spop2_size(adult_22);
    size_adult_23 = spop2_size(adult_23);
    size_adult_24 = spop2_size(adult_24);

    ret[0] = size_adult_0.d;
    if (CHECK(ret[0])) {goto endall;};
    ret[1] = size_adult_1.d;
    if (CHECK(ret[1])) {goto endall;};
    ret[2] = size_adult_2.d;
    if (CHECK(ret[2])) {goto endall;};
    ret[3] = size_adult_3.d;
    if (CHECK(ret[3])) {goto endall;};
    ret[4] = size_adult_4.d;
    if (CHECK(ret[4])) {goto endall;};
    ret[5] = size_adult_5.d;
    if (CHECK(ret[5])) {goto endall;};
    ret[6] = size_adult_6.d;
    if (CHECK(ret[6])) {goto endall;};
    ret[7] = size_adult_7.d;
    if (CHECK(ret[7])) {goto endall;};
    ret[8] = size_adult_8.d;
    if (CHECK(ret[8])) {goto endall;};
    ret[9] = size_adult_9.d;
    if (CHECK(ret[9])) {goto endall;};
    ret[10] = size_adult_10.d;
    if (CHECK(ret[10])) {goto endall;};
    ret[11] = size_adult_11.d;
    if (CHECK(ret[11])) {goto endall;};
    ret[12] = size_adult_12.d;
    if (CHECK(ret[12])) {goto endall;};
    ret[13] = size_adult_13.d;
    if (CHECK(ret[13])) {goto endall;};
    ret[14] = size_adult_14.d;
    if (CHECK(ret[14])) {goto endall;};
    ret[15] = size_adult_15.d;
    if (CHECK(ret[15])) {goto endall;};
    ret[16] = size_adult_16.d;
    if (CHECK(ret[16])) {goto endall;};
    ret[17] = size_adult_17.d;
    if (CHECK(ret[17])) {goto endall;};
    ret[18] = size_adult_18.d;
    if (CHECK(ret[18])) {goto endall;};
    ret[19] = size_adult_19.d;
    if (CHECK(ret[19])) {goto endall;};
    ret[20] = size_adult_20.d;
    if (CHECK(ret[20])) {goto endall;};
    ret[21] = size_adult_21.d;
    if (CHECK(ret[21])) {goto endall;};
    ret[22] = size_adult_22.d;
    if (CHECK(ret[22])) {goto endall;};
    ret[23] = size_adult_23.d;
    if (CHECK(ret[23])) {goto endall;};
    ret[24] = size_adult_24.d;
    if (CHECK(ret[24])) {goto endall;};

    ret += 25;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 0.0;
                spop2_step(adult_0, par, &size_adult_0, completed_adult_0, 0);

                par[0] = 0.0;
                spop2_step(adult_1, par, &size_adult_1, completed_adult_1, 0);

                par[0] = 0.0;
                spop2_step(adult_2, par, &size_adult_2, completed_adult_2, 0);

                par[0] = 0.0;
                spop2_step(adult_3, par, &size_adult_3, completed_adult_3, 0);

                par[0] = 0.0;
                spop2_step(adult_4, par, &size_adult_4, completed_adult_4, 0);

                par[0] = 0.0;
                spop2_step(adult_5, par, &size_adult_5, completed_adult_5, 0);

                par[0] = 0.0;
                spop2_step(adult_6, par, &size_adult_6, completed_adult_6, 0);

                par[0] = 0.0;
                spop2_step(adult_7, par, &size_adult_7, completed_adult_7, 0);

                par[0] = 0.0;
                spop2_step(adult_8, par, &size_adult_8, completed_adult_8, 0);

                par[0] = 0.0;
                spop2_step(adult_9, par, &size_adult_9, completed_adult_9, 0);

                par[0] = 0.0;
                spop2_step(adult_10, par, &size_adult_10, completed_adult_10, 0);

                par[0] = 0.0;
                spop2_step(adult_11, par, &size_adult_11, completed_adult_11, 0);

                par[0] = 0.0;
                spop2_step(adult_12, par, &size_adult_12, completed_adult_12, 0);

                par[0] = 0.0;
                spop2_step(adult_13, par, &size_adult_13, completed_adult_13, 0);

                par[0] = 0.0;
                spop2_step(adult_14, par, &size_adult_14, completed_adult_14, 0);

                par[0] = 0.0;
                spop2_step(adult_15, par, &size_adult_15, completed_adult_15, 0);

                par[0] = 0.0;
                spop2_step(adult_16, par, &size_adult_16, completed_adult_16, 0);

                par[0] = 0.0;
                spop2_step(adult_17, par, &size_adult_17, completed_adult_17, 0);

                par[0] = 0.0;
                spop2_step(adult_18, par, &size_adult_18, completed_adult_18, 0);

                par[0] = 0.0;
                spop2_step(adult_19, par, &size_adult_19, completed_adult_19, 0);

                par[0] = 0.0;
                spop2_step(adult_20, par, &size_adult_20, completed_adult_20, 0);

                par[0] = 0.0;
                spop2_step(adult_21, par, &size_adult_21, completed_adult_21, 0);

                par[0] = 0.0;
                spop2_step(adult_22, par, &size_adult_22, completed_adult_22, 0);

                par[0] = 0.0;
                spop2_step(adult_23, par, &size_adult_23, completed_adult_23, 0);

                par[0] = 0.0;
                spop2_step(adult_24, par, &size_adult_24, completed_adult_24, 0);




        }

        ret[0] = size_adult_0.d;
        if (CHECK(ret[0])) {goto endall;};
        ret[1] = size_adult_1.d;
        if (CHECK(ret[1])) {goto endall;};
        ret[2] = size_adult_2.d;
        if (CHECK(ret[2])) {goto endall;};
        ret[3] = size_adult_3.d;
        if (CHECK(ret[3])) {goto endall;};
        ret[4] = size_adult_4.d;
        if (CHECK(ret[4])) {goto endall;};
        ret[5] = size_adult_5.d;
        if (CHECK(ret[5])) {goto endall;};
        ret[6] = size_adult_6.d;
        if (CHECK(ret[6])) {goto endall;};
        ret[7] = size_adult_7.d;
        if (CHECK(ret[7])) {goto endall;};
        ret[8] = size_adult_8.d;
        if (CHECK(ret[8])) {goto endall;};
        ret[9] = size_adult_9.d;
        if (CHECK(ret[9])) {goto endall;};
        ret[10] = size_adult_10.d;
        if (CHECK(ret[10])) {goto endall;};
        ret[11] = size_adult_11.d;
        if (CHECK(ret[11])) {goto endall;};
        ret[12] = size_adult_12.d;
        if (CHECK(ret[12])) {goto endall;};
        ret[13] = size_adult_13.d;
        if (CHECK(ret[13])) {goto endall;};
        ret[14] = size_adult_14.d;
        if (CHECK(ret[14])) {goto endall;};
        ret[15] = size_adult_15.d;
        if (CHECK(ret[15])) {goto endall;};
        ret[16] = size_adult_16.d;
        if (CHECK(ret[16])) {goto endall;};
        ret[17] = size_adult_17.d;
        if (CHECK(ret[17])) {goto endall;};
        ret[18] = size_adult_18.d;
        if (CHECK(ret[18])) {goto endall;};
        ret[19] = size_adult_19.d;
        if (CHECK(ret[19])) {goto endall;};
        ret[20] = size_adult_20.d;
        if (CHECK(ret[20])) {goto endall;};
        ret[21] = size_adult_21.d;
        if (CHECK(ret[21])) {goto endall;};
        ret[22] = size_adult_22.d;
        if (CHECK(ret[22])) {goto endall;};
        ret[23] = size_adult_23.d;
        if (CHECK(ret[23])) {goto endall;};
        ret[24] = size_adult_24.d;
        if (CHECK(ret[24])) {goto endall;};

        ret += 25;



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

            buffsz = spop2_buffsize(adult_0);
            buff = spop2_savestate(adult_0);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_1);
            buff = spop2_savestate(adult_1);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_2);
            buff = spop2_savestate(adult_2);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_3);
            buff = spop2_savestate(adult_3);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_4);
            buff = spop2_savestate(adult_4);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_5);
            buff = spop2_savestate(adult_5);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_6);
            buff = spop2_savestate(adult_6);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_7);
            buff = spop2_savestate(adult_7);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_8);
            buff = spop2_savestate(adult_8);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_9);
            buff = spop2_savestate(adult_9);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_10);
            buff = spop2_savestate(adult_10);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_11);
            buff = spop2_savestate(adult_11);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_12);
            buff = spop2_savestate(adult_12);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_13);
            buff = spop2_savestate(adult_13);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_14);
            buff = spop2_savestate(adult_14);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_15);
            buff = spop2_savestate(adult_15);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_16);
            buff = spop2_savestate(adult_16);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_17);
            buff = spop2_savestate(adult_17);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_18);
            buff = spop2_savestate(adult_18);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_19);
            buff = spop2_savestate(adult_19);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_20);
            buff = spop2_savestate(adult_20);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_21);
            buff = spop2_savestate(adult_21);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_22);
            buff = spop2_savestate(adult_22);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_23);
            buff = spop2_savestate(adult_23);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult_24);
            buff = spop2_savestate(adult_24);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            fclose(file);
        }
    }

    spop2_free(&adult_0);
    spop2_free(&adult_1);
    spop2_free(&adult_2);
    spop2_free(&adult_3);
    spop2_free(&adult_4);
    spop2_free(&adult_5);
    spop2_free(&adult_6);
    spop2_free(&adult_7);
    spop2_free(&adult_8);
    spop2_free(&adult_9);
    spop2_free(&adult_10);
    spop2_free(&adult_11);
    spop2_free(&adult_12);
    spop2_free(&adult_13);
    spop2_free(&adult_14);
    spop2_free(&adult_15);
    spop2_free(&adult_16);
    spop2_free(&adult_17);
    spop2_free(&adult_18);
    spop2_free(&adult_19);
    spop2_free(&adult_20);
    spop2_free(&adult_21);
    spop2_free(&adult_22);
    spop2_free(&adult_23);
    spop2_free(&adult_24);
    spop2_free(&(popdummy_adult_0));
    spop2_free(&(popdummy_adult_1));
    spop2_free(&(popdummy_adult_2));
    spop2_free(&(popdummy_adult_3));
    spop2_free(&(popdummy_adult_4));
    spop2_free(&(popdummy_adult_5));
    spop2_free(&(popdummy_adult_6));
    spop2_free(&(popdummy_adult_7));
    spop2_free(&(popdummy_adult_8));
    spop2_free(&(popdummy_adult_9));
    spop2_free(&(popdummy_adult_10));
    spop2_free(&(popdummy_adult_11));
    spop2_free(&(popdummy_adult_12));
    spop2_free(&(popdummy_adult_13));
    spop2_free(&(popdummy_adult_14));
    spop2_free(&(popdummy_adult_15));
    spop2_free(&(popdummy_adult_16));
    spop2_free(&(popdummy_adult_17));
    spop2_free(&(popdummy_adult_18));
    spop2_free(&(popdummy_adult_19));
    spop2_free(&(popdummy_adult_20));
    spop2_free(&(popdummy_adult_21));
    spop2_free(&(popdummy_adult_22));
    spop2_free(&(popdummy_adult_23));
    spop2_free(&(popdummy_adult_24));

}

int main(int argc, char *argv[]) {
    return 0;
}


