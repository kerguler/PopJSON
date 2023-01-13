#include <math.h>
#include "population.h"

#define NumPar 15
#define NumPop 3
#define NumInt 7

#define p_p23_1 0
#define p_p23_2 1
#define p_p23_3 2
#define p_p4_1 3
#define p_p4_2 4
#define p_p4_3 5
#define p_d23_1 6
#define p_d23_2 7
#define p_d23_3 8
#define a_F4_1 9
#define a_F4_2 10
#define a_F4_3 11
#define a_KK_water 12
#define a_KK_surv 13
#define sex_ratio 14

#define hatch 0
#define imsurv 0
#define imdev 1
#define surv 0

double dmin(double x, double y) { return(x<y ? x : y); }
double dmax(double x, double y) { return(x>y ? x : y); }
double funmort(double T, double Tm, double a, double c) { return (dmax(0.0, dmin(1.0, exp(a)*(pow((T)-(Tm),4.0)) + (c)))); }
double briere1C(double T, double T0, double T1, double a) { return ( (T) <= (T0) ? 1e13 : ( (T) >= (T1) ? 1e13 : dmin(1e13, dmax(1.0, 1.0/(exp(a)*(T)*((T)-(T0))*sqrt((T1)-(T))) ) ) )); }
double briere1(double T, double T0, double T1, double a) { return (briere1C(273.15+(T),273.15+(T0),273.15+(T1),(a))); }
double poly(double x, double a1, double a2, double a3) { return (max(0.0, (a1) + (a2)*(x) + (a3)*pow((x),2))); }

void init(int *no, int *np, int *ni) {
    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
}

void parnames(char **names, double *param) {
    char temp[NumPop+NumPar+NumInt][256] = {
        "egg", "immature", "adult",
        "p_p23_1", "p_p23_2", "p_p23_3", "p_p4_1", "p_p4_2", "p_p4_3", "p_d23_1", "p_d23_2", "p_d23_3", "a_F4_1", "a_F4_2", "a_F4_3", "a_KK_water", "a_KK_surv", "sex_ratio",
        "p4", "pd1", "d23", "KK", "F4", "p23dens", "p23",
    };

    int i;
    for (i=0; i<(NumPop+NumPar+NumInt); i++)
        names[i] = strdup(temp[i]);

    param[p_p23_1] = 27.1775;
    param[p_p23_2] = -15.4698;
    param[p_p23_3] = 0.00299033;
    param[p_p4_1] = 22.0939;
    param[p_p4_2] = -17.4307;
    param[p_p4_3] = 0.000737542;
    param[p_d23_1] = 11.7082;
    param[p_d23_2] = 45.4497;
    param[p_d23_3] = -13.617;
    param[a_F4_1] = -21.360902029092074;
    param[a_F4_2] = 2.337525403022666;
    param[a_F4_3] = -0.04019927418978105;
    param[a_KK_water] = 25;
    param[a_KK_surv] = 0.05;
    param[sex_ratio] = 0.5;
}

void destroy(void) {
}

void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *dret, double *iret, int *success) {
    double *atemp = envir + 0 * tf;
    double *stemp = envir + 1 * tf;
    double *water = envir + 2 * tf;

    population egg;
    population immature;
    population adult;

    number num = numZERO;
    char arbiters[3];
    number key[3];
    number size_egg;
    number size_immature;
    number size_adult;
    number completed_egg[3];
    number completed_immature[3];
    number completed_adult[3];
    double egg_to_larva = 0.0;
    double larva_to_adult = 0.0;
    double egg_laying = 0.0;
    double par[2];

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    arbiters[2] = STOP;
    key[2] = numZERO;
    egg = spop2_init(arbiters, DETERMINISTIC);
    if (y0[0]) { num.d = y0[0]; spop2_add(egg, key, num); }

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = ACC_FIXED;
    key[1] = numZERO;
    arbiters[2] = STOP;
    key[2] = numZERO;
    immature = spop2_init(arbiters, DETERMINISTIC);
    if (y0[1]) { num.d = y0[1]; spop2_add(immature, key, num); }

    arbiters[0] = NOAGE_CONST;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    arbiters[2] = STOP;
    key[2] = numZERO;
    adult = spop2_init(arbiters, DETERMINISTIC);
    if (y0[2]) { num.d = y0[2]; spop2_add(adult, key, num); }


    double p4 = 0.0;
    double pd1 = 0.0;
    double d23 = 0.0;
    double KK = 0.0;
    double F4 = 0.0;
    double p23dens = 0.0;
    double p23 = 0.0;

    int tm = 0;
    size_egg = spop2_size(egg);
    size_immature = spop2_size(immature);
    size_adult = spop2_size(adult);

    ret[0] = size_egg.d;
    ret[1] = size_immature.d;
    ret[2] = size_adult.d;

    dret[0] = 0.0;
    dret[1] = 0.0;
    dret[2] = 0.0;

    iret[0] = p4;
    iret[1] = pd1;
    iret[2] = d23;
    iret[3] = KK;
    iret[4] = F4;
    iret[5] = p23dens;
    iret[6] = p23;

    ret += 3;
    dret += 3;
    iret += 7;

    for (tm=1; tm<tf; tm++) {
        p4 = funmort(atemp[(tm - 1)], pr[p_p4_1], pr[p_p4_2], pr[p_p4_3]);
        pd1 = 1;
        d23 = briere1(atemp[(tm - 1)], pr[p_d23_1], pr[p_d23_2], pr[p_d23_3]);
        KK = (pr[a_KK_water] * water[(tm - 1)]);
        F4 = poly(atemp[(tm - 1)], pr[a_F4_1], pr[a_F4_2], pr[a_F4_3]);
        p23dens = (1 - exp((-1 * pr[a_KK_surv] * (1 + (size_immature.d / KK)))));
        p23 = (1 - ((1 - funmort(stemp[(tm - 1)], pr[p_p23_1], pr[p_p23_2], pr[p_p23_3])) * (1 - p23dens)));

        par[0] = pd1;
        par[1] = 0.0;
        spop2_step(egg, par, &size_egg, completed_egg, 0);

        par[0] = p23;
        par[1] = d23;
        spop2_step(immature, par, &size_immature, completed_immature, 0);

        par[0] = p4;
        par[1] = 0.0;
        spop2_step(adult, par, &size_adult, completed_adult, 0);

        egg_to_larva = completed_egg[hatch].d;
        larva_to_adult = (pr[sex_ratio] * completed_immature[imdev].d);
        egg_laying = (F4 * size_adult.d);

        num.d = egg_to_larva;
        spop2_add(immature, key, num);
        num.d = larva_to_adult;
        spop2_add(adult, key, num);
        num.d = egg_laying;
        spop2_add(egg, key, num);

        size_immature.d += egg_to_larva;
        size_adult.d += larva_to_adult;
        size_egg.d += egg_laying;

        ret[0] = size_egg.d;
        ret[1] = size_immature.d;
        ret[2] = size_adult.d;

        dret[0] = 0.0;
        dret[1] = 0.0;
        dret[2] = 0.0;

        iret[0] = p4;
        iret[1] = pd1;
        iret[2] = d23;
        iret[3] = KK;
        iret[4] = F4;
        iret[5] = p23dens;
        iret[6] = p23;

        ret += 3;
        dret += 3;
        iret += 7;

    }

    spop2_free(&egg);
    spop2_free(&immature);
    spop2_free(&adult);

    *success = 1;
}

int main(int argc, char *argv[]) {
    return 0;
}

