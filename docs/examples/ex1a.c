#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 0


#define larva_dev 0


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

double *model_param;



void init(int *no, int *np, int *ni) {
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
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {

    int TIME = 0;

    model_param = pr;

    population larva;

    number num = numZERO;
    char arbiters[2];
    number key[2];
    number size_larva;
    number completed_larva[2];
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
        larva = spop2_loadstate(buff);


        free(buff);

    } else {
        arbiters[0] = ACC_ERLANG;
        key[0] = numZERO;
        arbiters[1] = STOP;
        key[1] = numZERO;
        larva = spop2_init(arbiters, DETERMINISTIC);
        if (y0[0]) { num.d = y0[0]; spop2_add(larva, key, num); }

    }

    if (file0 && file0[0]!=' ') {
        fclose(file);
    }

    size_larva = spop2_size(larva);

    ret[0] = size_larva.d;
    if (CHECK(ret[0])) {goto endall;};

    ret += 1;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 10;
                par[1] = 4;
                spop2_step(larva, par, &size_larva, completed_larva, 0);





        }

        ret[0] = size_larva.d;
        if (CHECK(ret[0])) {goto endall;};

        ret += 1;



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

            buffsz = spop2_buffsize(larva);
            buff = spop2_savestate(larva);
            fwrite(&buffsz, sizeof(unsigned int), 1, file);
            fwrite(buff, buffsz, 1, file);
            free(buff);

            fclose(file);
        }
    }

    spop2_free(&larva);

}

int main(int argc, char *argv[]) {
    return 0;
}

