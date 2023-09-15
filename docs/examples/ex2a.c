#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 2


#define larva_mort 0
#define larva_dev 1


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
        "larva",
        "larva_death", "larva_to_pupa",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt); i++)
        names[i] = strdup(temp[i]);

}

void destroy(void) {
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *iret, int *success) {

    population larva;

    number num = numZERO;
    char arbiters[3];
    number key[3];
    number size_larva;
    number completed_larva[3];
    double larva_death = 0.0;
    double larva_to_pupa = 0.0;
    double par[4];

    arbiters[0] = ACC_ERLANG;
    key[0] = numZERO;
    arbiters[1] = ACC_ERLANG;
    key[1] = numZERO;
    arbiters[2] = STOP;
    key[2] = numZERO;
    larva = spop2_init(arbiters, DETERMINISTIC);
    if (y0[0]) { num.d = y0[0]; spop2_add(larva, key, num); }



    int TIME = 0;
    size_larva = spop2_size(larva);

    ret[0] = size_larva.d;
    if (CHECK(ret[0])) {goto endall;};

    ret += 1;

    for (TIME=1; TIME<tf; TIME++) {

        if (rep >= 0) {
                par[0] = 7;
                par[1] = 2;
                par[2] = 10;
                par[3] = 4;
                spop2_step(larva, par, &size_larva, completed_larva, 0);

                larva_death = completed_larva[larva_mort].d;
                larva_to_pupa = completed_larva[larva_dev].d;




        }

        ret[0] = size_larva.d;
        if (CHECK(ret[0])) {goto endall;};

        ret += 1;


        iret[0] = larva_death;
        if (CHECK(iret[0])) {goto endall;};
        iret[1] = larva_to_pupa;
        if (CHECK(iret[1])) {goto endall;};

        iret += 2;

    }

  endall:

    *success = TIME;

    spop2_free(&larva);

}

int main(int argc, char *argv[]) {
    return 0;
}
