/* 
 *  This code was automatically generated by PopJSON (v.1.2.5)
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  See the GNU General Public License for more details:
 *  <https://www.gnu.org/licenses/>
 */

#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 9
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


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

int TIME;
int TIMEF;

double *model_param;
double *envir_tprob;


double tprob_adult_dispersion[81];


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

void fun_harvest_adult_dispersion_25(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[25];
}

void fun_harvest_adult_dispersion_26(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[26];
}

void fun_harvest_adult_dispersion_27(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[27];
}

void fun_harvest_adult_dispersion_28(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[28];
}

void fun_harvest_adult_dispersion_29(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[29];
}

void fun_harvest_adult_dispersion_30(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[30];
}

void fun_harvest_adult_dispersion_31(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[31];
}

void fun_harvest_adult_dispersion_32(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[32];
}

void fun_harvest_adult_dispersion_33(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[33];
}

void fun_harvest_adult_dispersion_34(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[34];
}

void fun_harvest_adult_dispersion_35(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[35];
}

void fun_harvest_adult_dispersion_36(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[36];
}

void fun_harvest_adult_dispersion_37(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[37];
}

void fun_harvest_adult_dispersion_38(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[38];
}

void fun_harvest_adult_dispersion_39(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[39];
}

void fun_harvest_adult_dispersion_40(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[40];
}

void fun_harvest_adult_dispersion_41(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[41];
}

void fun_harvest_adult_dispersion_42(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[42];
}

void fun_harvest_adult_dispersion_43(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[43];
}

void fun_harvest_adult_dispersion_44(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[44];
}

void fun_harvest_adult_dispersion_45(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[45];
}

void fun_harvest_adult_dispersion_46(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[46];
}

void fun_harvest_adult_dispersion_47(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[47];
}

void fun_harvest_adult_dispersion_48(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[48];
}

void fun_harvest_adult_dispersion_49(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[49];
}

void fun_harvest_adult_dispersion_50(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[50];
}

void fun_harvest_adult_dispersion_51(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[51];
}

void fun_harvest_adult_dispersion_52(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[52];
}

void fun_harvest_adult_dispersion_53(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[53];
}

void fun_harvest_adult_dispersion_54(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[54];
}

void fun_harvest_adult_dispersion_55(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[55];
}

void fun_harvest_adult_dispersion_56(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[56];
}

void fun_harvest_adult_dispersion_57(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[57];
}

void fun_harvest_adult_dispersion_58(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[58];
}

void fun_harvest_adult_dispersion_59(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[59];
}

void fun_harvest_adult_dispersion_60(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[60];
}

void fun_harvest_adult_dispersion_61(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[61];
}

void fun_harvest_adult_dispersion_62(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[62];
}

void fun_harvest_adult_dispersion_63(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[63];
}

void fun_harvest_adult_dispersion_64(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[64];
}

void fun_harvest_adult_dispersion_65(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[65];
}

void fun_harvest_adult_dispersion_66(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[66];
}

void fun_harvest_adult_dispersion_67(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[67];
}

void fun_harvest_adult_dispersion_68(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[68];
}

void fun_harvest_adult_dispersion_69(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[69];
}

void fun_harvest_adult_dispersion_70(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[70];
}

void fun_harvest_adult_dispersion_71(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[71];
}

void fun_harvest_adult_dispersion_72(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[72];
}

void fun_harvest_adult_dispersion_73(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[73];
}

void fun_harvest_adult_dispersion_74(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[74];
}

void fun_harvest_adult_dispersion_75(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[75];
}

void fun_harvest_adult_dispersion_76(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[76];
}

void fun_harvest_adult_dispersion_77(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[77];
}

void fun_harvest_adult_dispersion_78(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[78];
}

void fun_harvest_adult_dispersion_79(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[79];
}

void fun_harvest_adult_dispersion_80(number *key, number num, number *newkey, double *frac) {
    newkey[0].i=key[0].i;
    *frac = tprob_adult_dispersion[80];
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
        "adult_0", "adult_1", "adult_2", "adult_3", "adult_4", "adult_5", "adult_6", "adult_7", "adult_8",
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

    prepare_tprobs(9, envir_tprob, tprob_adult_dispersion);

    population adult_0;
    population adult_1;
    population adult_2;
    population adult_3;
    population adult_4;
    population adult_5;
    population adult_6;
    population adult_7;
    population adult_8;

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
    number completed_adult_0[2];
    number completed_adult_1[2];
    number completed_adult_2[2];
    number completed_adult_3[2];
    number completed_adult_4[2];
    number completed_adult_5[2];
    number completed_adult_6[2];
    number completed_adult_7[2];
    number completed_adult_8[2];
    population popdummy_adult_0;
    population popdummy_adult_1;
    population popdummy_adult_2;
    population popdummy_adult_3;
    population popdummy_adult_4;
    population popdummy_adult_5;
    population popdummy_adult_6;
    population popdummy_adult_7;
    population popdummy_adult_8;
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


    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_0 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_1 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_2 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_3 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_4 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_5 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_6 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_7 = spop2_init(arbiters, DETERMINISTIC);

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    popdummy_adult_8 = spop2_init(arbiters, DETERMINISTIC);


    size_adult_0 = spop2_size(adult_0);
    size_adult_1 = spop2_size(adult_1);
    size_adult_2 = spop2_size(adult_2);
    size_adult_3 = spop2_size(adult_3);
    size_adult_4 = spop2_size(adult_4);
    size_adult_5 = spop2_size(adult_5);
    size_adult_6 = spop2_size(adult_6);
    size_adult_7 = spop2_size(adult_7);
    size_adult_8 = spop2_size(adult_8);

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

    ret += 9;

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




                spop2_harvest(adult_0, popdummy_adult_0, fun_harvest_adult_dispersion_0);
                spop2_harvest(adult_0, popdummy_adult_1, fun_harvest_adult_dispersion_1);
                spop2_harvest(adult_0, popdummy_adult_2, fun_harvest_adult_dispersion_2);
                spop2_harvest(adult_0, popdummy_adult_3, fun_harvest_adult_dispersion_3);
                spop2_harvest(adult_0, popdummy_adult_4, fun_harvest_adult_dispersion_4);
                spop2_harvest(adult_0, popdummy_adult_5, fun_harvest_adult_dispersion_5);
                spop2_harvest(adult_0, popdummy_adult_6, fun_harvest_adult_dispersion_6);
                spop2_harvest(adult_0, popdummy_adult_7, fun_harvest_adult_dispersion_7);
                spop2_harvest(adult_0, popdummy_adult_8, fun_harvest_adult_dispersion_8);
                spop2_harvest(adult_1, popdummy_adult_0, fun_harvest_adult_dispersion_9);
                spop2_harvest(adult_1, popdummy_adult_1, fun_harvest_adult_dispersion_10);
                spop2_harvest(adult_1, popdummy_adult_2, fun_harvest_adult_dispersion_11);
                spop2_harvest(adult_1, popdummy_adult_3, fun_harvest_adult_dispersion_12);
                spop2_harvest(adult_1, popdummy_adult_4, fun_harvest_adult_dispersion_13);
                spop2_harvest(adult_1, popdummy_adult_5, fun_harvest_adult_dispersion_14);
                spop2_harvest(adult_1, popdummy_adult_6, fun_harvest_adult_dispersion_15);
                spop2_harvest(adult_1, popdummy_adult_7, fun_harvest_adult_dispersion_16);
                spop2_harvest(adult_1, popdummy_adult_8, fun_harvest_adult_dispersion_17);
                spop2_harvest(adult_2, popdummy_adult_0, fun_harvest_adult_dispersion_18);
                spop2_harvest(adult_2, popdummy_adult_1, fun_harvest_adult_dispersion_19);
                spop2_harvest(adult_2, popdummy_adult_2, fun_harvest_adult_dispersion_20);
                spop2_harvest(adult_2, popdummy_adult_3, fun_harvest_adult_dispersion_21);
                spop2_harvest(adult_2, popdummy_adult_4, fun_harvest_adult_dispersion_22);
                spop2_harvest(adult_2, popdummy_adult_5, fun_harvest_adult_dispersion_23);
                spop2_harvest(adult_2, popdummy_adult_6, fun_harvest_adult_dispersion_24);
                spop2_harvest(adult_2, popdummy_adult_7, fun_harvest_adult_dispersion_25);
                spop2_harvest(adult_2, popdummy_adult_8, fun_harvest_adult_dispersion_26);
                spop2_harvest(adult_3, popdummy_adult_0, fun_harvest_adult_dispersion_27);
                spop2_harvest(adult_3, popdummy_adult_1, fun_harvest_adult_dispersion_28);
                spop2_harvest(adult_3, popdummy_adult_2, fun_harvest_adult_dispersion_29);
                spop2_harvest(adult_3, popdummy_adult_3, fun_harvest_adult_dispersion_30);
                spop2_harvest(adult_3, popdummy_adult_4, fun_harvest_adult_dispersion_31);
                spop2_harvest(adult_3, popdummy_adult_5, fun_harvest_adult_dispersion_32);
                spop2_harvest(adult_3, popdummy_adult_6, fun_harvest_adult_dispersion_33);
                spop2_harvest(adult_3, popdummy_adult_7, fun_harvest_adult_dispersion_34);
                spop2_harvest(adult_3, popdummy_adult_8, fun_harvest_adult_dispersion_35);
                spop2_harvest(adult_4, popdummy_adult_0, fun_harvest_adult_dispersion_36);
                spop2_harvest(adult_4, popdummy_adult_1, fun_harvest_adult_dispersion_37);
                spop2_harvest(adult_4, popdummy_adult_2, fun_harvest_adult_dispersion_38);
                spop2_harvest(adult_4, popdummy_adult_3, fun_harvest_adult_dispersion_39);
                spop2_harvest(adult_4, popdummy_adult_4, fun_harvest_adult_dispersion_40);
                spop2_harvest(adult_4, popdummy_adult_5, fun_harvest_adult_dispersion_41);
                spop2_harvest(adult_4, popdummy_adult_6, fun_harvest_adult_dispersion_42);
                spop2_harvest(adult_4, popdummy_adult_7, fun_harvest_adult_dispersion_43);
                spop2_harvest(adult_4, popdummy_adult_8, fun_harvest_adult_dispersion_44);
                spop2_harvest(adult_5, popdummy_adult_0, fun_harvest_adult_dispersion_45);
                spop2_harvest(adult_5, popdummy_adult_1, fun_harvest_adult_dispersion_46);
                spop2_harvest(adult_5, popdummy_adult_2, fun_harvest_adult_dispersion_47);
                spop2_harvest(adult_5, popdummy_adult_3, fun_harvest_adult_dispersion_48);
                spop2_harvest(adult_5, popdummy_adult_4, fun_harvest_adult_dispersion_49);
                spop2_harvest(adult_5, popdummy_adult_5, fun_harvest_adult_dispersion_50);
                spop2_harvest(adult_5, popdummy_adult_6, fun_harvest_adult_dispersion_51);
                spop2_harvest(adult_5, popdummy_adult_7, fun_harvest_adult_dispersion_52);
                spop2_harvest(adult_5, popdummy_adult_8, fun_harvest_adult_dispersion_53);
                spop2_harvest(adult_6, popdummy_adult_0, fun_harvest_adult_dispersion_54);
                spop2_harvest(adult_6, popdummy_adult_1, fun_harvest_adult_dispersion_55);
                spop2_harvest(adult_6, popdummy_adult_2, fun_harvest_adult_dispersion_56);
                spop2_harvest(adult_6, popdummy_adult_3, fun_harvest_adult_dispersion_57);
                spop2_harvest(adult_6, popdummy_adult_4, fun_harvest_adult_dispersion_58);
                spop2_harvest(adult_6, popdummy_adult_5, fun_harvest_adult_dispersion_59);
                spop2_harvest(adult_6, popdummy_adult_6, fun_harvest_adult_dispersion_60);
                spop2_harvest(adult_6, popdummy_adult_7, fun_harvest_adult_dispersion_61);
                spop2_harvest(adult_6, popdummy_adult_8, fun_harvest_adult_dispersion_62);
                spop2_harvest(adult_7, popdummy_adult_0, fun_harvest_adult_dispersion_63);
                spop2_harvest(adult_7, popdummy_adult_1, fun_harvest_adult_dispersion_64);
                spop2_harvest(adult_7, popdummy_adult_2, fun_harvest_adult_dispersion_65);
                spop2_harvest(adult_7, popdummy_adult_3, fun_harvest_adult_dispersion_66);
                spop2_harvest(adult_7, popdummy_adult_4, fun_harvest_adult_dispersion_67);
                spop2_harvest(adult_7, popdummy_adult_5, fun_harvest_adult_dispersion_68);
                spop2_harvest(adult_7, popdummy_adult_6, fun_harvest_adult_dispersion_69);
                spop2_harvest(adult_7, popdummy_adult_7, fun_harvest_adult_dispersion_70);
                spop2_harvest(adult_7, popdummy_adult_8, fun_harvest_adult_dispersion_71);
                spop2_harvest(adult_8, popdummy_adult_0, fun_harvest_adult_dispersion_72);
                spop2_harvest(adult_8, popdummy_adult_1, fun_harvest_adult_dispersion_73);
                spop2_harvest(adult_8, popdummy_adult_2, fun_harvest_adult_dispersion_74);
                spop2_harvest(adult_8, popdummy_adult_3, fun_harvest_adult_dispersion_75);
                spop2_harvest(adult_8, popdummy_adult_4, fun_harvest_adult_dispersion_76);
                spop2_harvest(adult_8, popdummy_adult_5, fun_harvest_adult_dispersion_77);
                spop2_harvest(adult_8, popdummy_adult_6, fun_harvest_adult_dispersion_78);
                spop2_harvest(adult_8, popdummy_adult_7, fun_harvest_adult_dispersion_79);
                spop2_harvest(adult_8, popdummy_adult_8, fun_harvest_adult_dispersion_80);

                spop2_addpop(adult_0, popdummy_adult_0);
                spop2_addpop(adult_1, popdummy_adult_1);
                spop2_addpop(adult_2, popdummy_adult_2);
                spop2_addpop(adult_3, popdummy_adult_3);
                spop2_addpop(adult_4, popdummy_adult_4);
                spop2_addpop(adult_5, popdummy_adult_5);
                spop2_addpop(adult_6, popdummy_adult_6);
                spop2_addpop(adult_7, popdummy_adult_7);
                spop2_addpop(adult_8, popdummy_adult_8);

                size_adult_0 = spop2_size(adult_0);
                size_adult_1 = spop2_size(adult_1);
                size_adult_2 = spop2_size(adult_2);
                size_adult_3 = spop2_size(adult_3);
                size_adult_4 = spop2_size(adult_4);
                size_adult_5 = spop2_size(adult_5);
                size_adult_6 = spop2_size(adult_6);
                size_adult_7 = spop2_size(adult_7);
                size_adult_8 = spop2_size(adult_8);


                spop2_empty(&popdummy_adult_0);
                spop2_empty(&popdummy_adult_1);
                spop2_empty(&popdummy_adult_2);
                spop2_empty(&popdummy_adult_3);
                spop2_empty(&popdummy_adult_4);
                spop2_empty(&popdummy_adult_5);
                spop2_empty(&popdummy_adult_6);
                spop2_empty(&popdummy_adult_7);
                spop2_empty(&popdummy_adult_8);

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

        ret += 9;



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
    spop2_free(&(popdummy_adult_0));
    spop2_free(&(popdummy_adult_1));
    spop2_free(&(popdummy_adult_2));
    spop2_free(&(popdummy_adult_3));
    spop2_free(&(popdummy_adult_4));
    spop2_free(&(popdummy_adult_5));
    spop2_free(&(popdummy_adult_6));
    spop2_free(&(popdummy_adult_7));
    spop2_free(&(popdummy_adult_8));

}

int main(int argc, char *argv[]) {
    return 0;
}


