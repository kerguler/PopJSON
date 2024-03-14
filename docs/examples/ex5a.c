#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 4
#define NumPop 2
#define NumInt 4
#define NumEnv 1

#define d2m_a 0
#define d2m_b 1
#define d2m_c 2
#define d2s_c 3

#define immat_mort 0
#define immat_dev 1
#define adult_mort 0
#define history 1


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

int TIME;

double *model_param;
double *envir_temp;

double immat_death;
double immat_to_adult;
double d2m;
double d2s;

#define briere1(T,a,L,R) (((((T) <= (L))) ? (365.0) : (((((T) >= (R))) ? (365.0) : (dmin(365.0, dmax(1.0, (1.0 / exp(((a) + log((T)) + log(((T) - (L))) + (0.5 * log(((R) - (T))))))))))))))

void fun_harvest_maturation(number *key, number num, number *newkey, double *frac) {
    newkey[0].d=0;
    newkey[1].i=round((100.0 * key[immat_mort].d));
    *frac = 1.0;
}

void fun_hazpar_adult_mort_adult(const number *key, const number num, double *par) {
    par[0] = (((key[history].i > 50)) ? (80) : (40));
    par[1] = 5;
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
        "immat", "adult",
        "d2m_a", "d2m_b", "d2m_c", "d2s_c",
        "d2m", "d2s",
        "immat_death", "immat_to_adult",
        "temp",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt+NumEnv); i++)
        names[i] = strdup(temp[i]);

    param[d2m_a] = -12;
    parmin[d2m_a] = -12;
    parmax[d2m_a] = -12;
    param[d2m_b] = 283.15;
    parmin[d2m_b] = 283.15;
    parmax[d2m_b] = 283.15;
    param[d2m_c] = 308.15;
    parmin[d2m_c] = 308.15;
    parmax[d2m_c] = 308.15;
    param[d2s_c] = 0.2;
    parmin[d2s_c] = 0.2;
    parmax[d2s_c] = 0.2;
}

void destroy(void) {
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {

    TIME = 0;

    model_param = pr;

    envir_temp = envir + 1; envir += (int)round(*envir) + 1;

    population immat;
    population adult;

    number num = numZERO;
    char arbiters[3];
    number key[3];
    number size_immat;
    number size_adult;
    number completed_immat[3];
    number completed_adult[3];
    population popdone_immat[3];
    immat_death = 0.0;
    immat_to_adult = 0.0;
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
        immat = spop2_loadstate(buff);

        popdone_immat[0] = spop2_loadstate_empty(buff);
        popdone_immat[1] = spop2_loadstate_empty(buff);
        popdone_immat[2] = spop2_loadstate_empty(buff);

        free(buff);

        fread(&buffsz, sizeof(unsigned int), 1, file);
        buff = (number *)malloc(buffsz);
        fread(buff, buffsz, 1, file);
        adult = spop2_loadstate(buff);


        free(buff);

    } else {
        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = ACC_ERLANG;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        immat = spop2_init(arbiters, DETERMINISTIC);
        if (y0[0]) { num.d = y0[0]; spop2_add(immat, key, num); }

        popdone_immat[0] = spop2_init(arbiters, DETERMINISTIC);
        popdone_immat[1] = spop2_init(arbiters, DETERMINISTIC);
        popdone_immat[2] = spop2_init(arbiters, DETERMINISTIC);

        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = NOAGE_CONST;
        key[1] = numZERO;
        arbiters[2] = STOP;
        key[2] = numZERO;
        adult = spop2_init(arbiters, DETERMINISTIC);
        adult->arbiters[0]->fun_q_par = fun_hazpar_adult_mort_adult;
        if (y0[1]) { num.d = y0[1]; spop2_add(adult, key, num); }


    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }
    d2m = 0.0;
    d2s = 0.0;

    size_immat = spop2_size(immat);
    size_adult = spop2_size(adult);

    ret[0] = size_immat.d;
    if (CHECK(ret[0])) {goto endall;};
    ret[1] = size_adult.d;
    if (CHECK(ret[1])) {goto endall;};

    ret += 2;

    for (TIME=1; TIME<tf; TIME++) {
        d2m = briere1(envir_temp[(TIME-1)], model_param[d2m_a], model_param[d2m_b], model_param[d2m_c]);
        d2s = (model_param[d2s_c] * d2m);

        if (rep >= 0) {
                par[0] = 40;
                par[1] = 5;
                par[2] = d2m;
                par[3] = d2s;
                spop2_step(immat, par, &size_immat, completed_immat, popdone_immat);

                par[0] = 0.0;
                par[1] = 0.0;
                par[2] = 0;
                par[3] = 0.0;
                spop2_step(adult, par, &size_adult, completed_adult, 0);

                immat_death = completed_immat[immat_mort].d;

                immat_to_adult = completed_immat[immat_dev].d;


                spop2_harvest(popdone_immat[immat_dev], adult, fun_harvest_maturation);

                spop2_empty(&popdone_immat[0]);
                spop2_empty(&popdone_immat[1]);
                spop2_empty(&popdone_immat[2]);

                size_adult = spop2_size(adult);

        }

        ret[0] = size_immat.d;
        if (CHECK(ret[0])) {goto endall;};
        ret[1] = size_adult.d;
        if (CHECK(ret[1])) {goto endall;};

        ret += 2;

        iret[0] = d2m;
        if (CHECK(iret[0])) {goto endall;};
        iret[1] = d2s;
        if (CHECK(iret[1])) {goto endall;};

        iret[2] = immat_death;
        if (CHECK(iret[0])) {goto endall;};
        iret[3] = immat_to_adult;
        if (CHECK(iret[1])) {goto endall;};

        iret += 4;

    }

  endall:

    *success = TIME;

    if (file1 && file1[0]!=' ') {
        file = fopen(file1,"wb");
        if (!file) {
            *success = 0;
        } else {
            rewind(file);

            buffsz = spop2_buffsize(immat);
            buff = spop2_savestate(immat);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            buffsz = spop2_buffsize(adult);
            buff = spop2_savestate(adult);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            fclose(file);
        }
    }

    spop2_free(&immat);
    spop2_free(&(popdone_immat[0]));
    spop2_free(&(popdone_immat[1]));
    spop2_free(&(popdone_immat[2]));
    spop2_free(&adult);

}

int main(int argc, char *argv[]) {
    return 0;
}


