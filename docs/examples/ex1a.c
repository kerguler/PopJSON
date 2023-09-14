#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 0


#define adult_lifetime 0


double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }


void init(int *no, int *np, int *ni) {
    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
}

void parnames(char **names, double *param, double *parmin, double *parmax) {
    char temp[NumPop+NumPar+NumInt][256] = {
        "adult",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *iret, int *success) {

    population adult;

    number num = numZERO;
    char arbiters[2];
    number key[2];
    number size_adult;
    number completed_adult[2];
    double par[2];

    arbiters[0] = ACC_ERLANG;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    adult = spop2_init(arbiters, DETERMINISTIC);
    if (y0[0]) { num.d = y0[0]; spop2_add(adult, key, num); }



    int TIME = 0;
    size_adult = spop2_size(adult);

    ret[0] = size_adult.d;
    if (CHECK(ret[0])) {goto endall;};

    ret += 1;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 10;
                par[1] = 4;
                spop2_step(adult, par, &size_adult, completed_adult, 0);





        }

        ret[0] = size_adult.d;
        if (CHECK(ret[0])) {goto endall;};

        ret += 1;



        iret += 0;

    }

  endall:

    *success = TIME;

    spop2_free(&adult);

}

int main(int argc, char *argv[]) {
    return 0;
}
