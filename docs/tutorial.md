<style>
r { color: Red }
o { color: Orange }
g { color: Green }
.hidden {
    display: none;
}
pre.sourceCode {
    max-height: 300px;
}
.myFigures {
    display: flex;
}
.myFigures figure {
    width: 50%;
}
.myGroup {
    display: flex;
    flex-direction: column;
    width: 100%;
}
.myError,
.myCode,
.myJSON {
    width: 100%;
    height: 300px;
}
.myError {
    color: red;
    height: 100px;
}
.sourceCode {
    overflow: auto;
}
</style>

<script src="keparser.max.js"></script>
<script>
        function process(ids=["model","output","error"]) {
            var PopJSON = require('PopJSON');
            var parser = new PopJSON.PopJSON();
            let text = document.getElementById(ids[0]).value;
            let result = parser.parse_json(text);
            document.getElementById(ids[1]).value = result.model;
            document.getElementById(ids[2]).value = result.error;
        }
</script>

# PopJSON

We propose a JSON representation of dynamically-structured matrix population models, which can accommodate multiple processes, such as survival, development, and egg laying. At present, PopJSON deals only with the `sPop` models of Erguler et al. \[<a href="https://f1000research.com/articles/7-1220/v3" target="_blank" rel="noreferrer">sPop</a>, <a href="https://www.nature.com/articles/s41598-022-15806-2" target="_blank" rel="noreferrer">sPop2</a>, <a href="https://github.com/kerguler/Population" target="_blank" rel="noreferrer">Population</a>\], but soon, and with your help, it will expand to cover more.

## Using the library

The parser takes a JSON string, or a file, and converts it to its raw ANSI C equivalent. You'll then need to compile this in your system and run using the <a href="https://github.com/kerguler/Population" target="_blank" rel="noreferrer">Population</a> package. The package includes a wrapper for `Python`, and soon, will have another one for `R`.

We are working on developing the <a href="https://VEClim.com" target="_blank" rel="noreferrer">VEClim</a> API to take PopJSON requests and run sample simulations. But this shall be used to preview models. We will not reproduce the functionality of advanced analysis tools.

## Model definition

We first define the **model** with its **type** and **parameters**. algorithm (`Population` for now) and its parameters for simulation. The **deterministic** option will declare the model as either deterministic or stochastic. To set the precision of the accumulative process indicator, **istep** is used. This effectively limits the maximum number of pseudo-stage classes.
```json
{
    "model": {
            "title": "Climate-sensitive population dynamics of Aedes albopictus",
            "type": "Population",
            "url": "https://github.com/kerguler/Population",
            "deterministic": true,
            "parameters": {
                "algorithm": "Population",
                "istep": 0.01
            }
        }
}
```

### Declaring a population (or a development stage)

```json
{
    "model": {
            "title": "Climate-sensitive population dynamics of Aedes albopictus",
            "type": "Population",
            "url": "https://github.com/kerguler/Population",
            "deterministic": true,
            "parameters": {
                "algorithm": "Population",
                "istep": 0.01
            }
        },
    "populations": [
        {
            "id": "larva",
            "name": "The larva stage",
            "processes": [
                {
                    "id": "larva_dev",
                    "name": "Larva development time",
                    "arbiter": "ACC_ERLANG",
                    "value": [10, 4]
                }
            ]
        }
    ]
}
```

```C
#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 0

#define larva_dev 0

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
    char arbiters[2];
    number key[2];
    number size_larva;
    number completed_larva[2];
    double par[2];

    arbiters[0] = ACC_ERLANG;
    key[0] = numZERO;
    arbiters[1] = STOP;
    key[1] = numZERO;
    larva = spop2_init(arbiters, DETERMINISTIC);
    if (y0[0]) { num.d = y0[0]; spop2_add(larva, key, num); }

    int TIME = 0;
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
    spop2_free(&larva);
}

int main(int argc, char *argv[]) {
    return 0;
}
```

<div class="myFigures">

![Erlang-distributed larva development time](figures/ex1a.png "Deterministic - Erlang-distributed")

![Three realisations of stochastic development in 100 larvae](figures/ex1b.png "Stochastic - Erlang-distributed")

</div>

### Declaring multiple processes

```json
{
    "model": {
            "title": "Climate-sensitive population dynamics of Aedes albopictus",
            "type": "Population",
            "url": "https://github.com/kerguler/Population",
            "deterministic": true,
            "parameters": {
                "algorithm": "Population",
                "istep": 0.01
            }
        },
    "populations": [
        {
            "id": "larva",
            "name": "The larva stage",
            "processes": [
                {
                    "id": "larva_mort",
                    "name": "Larva lifetime",
                    "arbiter": "ACC_ERLANG",
                    "value": [7, 2]
                },
                {
                    "id": "larva_dev",
                    "name": "Larva development time",
                    "arbiter": "ACC_ERLANG",
                    "value": [10, 4]
                }
            ]
        }
    ],
    "transformations": [
        {
            "id": "larva_death",
            "name": "Larva dying today",
            "value": ["larva_mort", "larva"]
        }, {
            "id": "larva_to_pupa",
            "name": "Larva developing into pupa",
            "value": ["larva_dev", "larva"]
        }
    ]
}
```

The following **transformations** tag is needed to bind **larva_mort** to **larva_death** and **larva_dev** to **larva_to_pupa** to make them visible. Otherwise, only the size of the **larva** population is monitored.

```json
{
    "transformations": [
        {
            "id": "larva_death",
            "name": "Larva dying today",
            "value": ["larva_mort", "larva"]
        }, {
            "id": "larva_to_pupa",
            "name": "Larva developing into pupa",
            "value": ["larva_dev", "larva"]
        }
    ]
}
```

```C
#define larva_mort 0
#define larva_dev 1

char temp[NumPop+NumPar+NumInt][256] = {
    "larva",
    "larva_death", "larva_to_pupa",
};

par[0] = 7;
par[1] = 2;
par[2] = 10;
par[3] = 4;
spop2_step(larva, par, &size_larva, completed_larva, 0);

larva_death = completed_larva[larva_mort].d;
larva_to_pupa = completed_larva[larva_dev].d;
```


![Erlang-distributed larva lifetime and development time](figures/ex2a.png "Deterministic - Erlang-distributed")

### Declaring cyclic development (gonotrophic cycle)

```json
{
    "model": {
            "title": "Climate-sensitive population dynamics of Aedes albopictus",
            "type": "Population",
            "url": "https://github.com/kerguler/Population",
            "deterministic": true,
            "parameters": {
                "algorithm": "Population",
                "istep": 0.01
            }
        },
    "populations": [
        {
            "id": "adult",
            "name": "Adult females",
            "processes": [
                {
                    "id": "adult_mort",
                    "name": "Adult lifetime",
                    "arbiter": "ACC_ERLANG",
                    "value": [20, 5]
                },
                {
                    "id": "adult_dev",
                    "name": "Egg development in adult females",
                    "arbiter": "ACC_ERLANG",
                    "value": [5, 1]
                }
            ]
        }
    ],
    "transfers": [
        {
            "id": "gonotrophic_cycle",
            "name": "Gonotrophic cycle",
            "from": "adult_dev",
            "to": "adult",
            "value": [["adult_mort", "adult"], "0"]
        }
    ],
    "transformations": [
        {
            "id": "adult_death",
            "name": "Adult females dying today",
            "value": ["adult_mort", "adult"]
        }, {
            "id": "num_gravid",
            "name": "Number of gravid females",
            "value": ["adult_dev", "adult"]
        },{
            "id": "egg_laying",
            "name": "Egg laying at the end of gonotrophic cycle",
            "value": ["*", "num_gravid", 10]
        }
    ]
}
```

```C
#include <math.h>
#include "population.h"

#define CHECK(x) (isnan(x) || isinf(x))

#define NumPar 0
#define NumPop 1
#define NumInt 3

#define adult_mort 0
#define adult_dev 1

double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

void fun_transfer_gonotrophic_cycle(number *key, number num, void *pop) {
    number q[3] = {
        {.d=key[adult_mort].d},
        {.d=0},
    };
    spop2_add(*(population *)pop, q, num);
}

void init(int *no, int *np, int *ni) {
    spop2_set_eps(0.01);

    *no = NumPop;
    *np = NumPar;
    *ni = NumInt;
}

void parnames(char **names, double *param, double *parmin, double *parmax) {
    char temp[NumPop+NumPar+NumInt][256] = {
        "adult",
        "adult_death", "num_gravid", "egg_laying",
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
    char arbiters[3];
    number key[3];
    number size_adult;
    number completed_adult[3];
    double adult_death = 0.0;
    double num_gravid = 0.0;
    double egg_laying = 0.0;
    double par[4];

    arbiters[0] = ACC_ERLANG;
    key[0] = numZERO;
    arbiters[1] = ACC_ERLANG;
    key[1] = numZERO;
    arbiters[2] = STOP;
    key[2] = numZERO;
    adult = spop2_init(arbiters, DETERMINISTIC);
    if (y0[0]) { num.d = y0[0]; spop2_add(adult, key, num); }

    population popdone_adult[3];
    popdone_adult[0] = spop2_init(arbiters, DETERMINISTIC);
    popdone_adult[1] = spop2_init(arbiters, DETERMINISTIC);
    popdone_adult[2] = spop2_init(arbiters, DETERMINISTIC);

    int TIME = 0;
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


                spop2_foreach(popdone_adult[adult_dev], fun_transfer_gonotrophic_cycle, (void *)(&adult));

                spop2_empty(&popdone_adult[0]);
                spop2_empty(&popdone_adult[1]);
                spop2_empty(&popdone_adult[2]);

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

    spop2_free(&adult);
    spop2_free(&(popdone_adult[0]));
    spop2_free(&(popdone_adult[1]));
    spop2_free(&(popdone_adult[2]));

}

int main(int argc, char *argv[]) {
    return 0;
}
```

![Erlang-distributed adult lifetime and gonotrophic cycle](figures/ex3a.png "Deterministic - Erlang-distributed")

```json
{
    "model": {
            "title": "Climate-sensitive population dynamics of Aedes albopictus",
            "type": "Population",
            "url": "https://github.com/kerguler/Population",
            "deterministic": false,
            "parameters": {
                "algorithm": "Population",
                "istep": 0.01
            }
        },
    "populations": [
        {
            "id": "adult",
            "name": "Adult females",
            "processes": [
                {
                    "id": "adult_mort",
                    "name": "Adult lifetime",
                    "arbiter": "ACC_ERLANG",
                    "value": [20, 5]
                },
                {
                    "id": "adult_dev",
                    "name": "Egg development in adult females",
                    "arbiter": "ACC_ERLANG",
                    "value": [5, 1]
                }
            ]
        }
    ],
    "transfers": [
        {
            "id": "gonotrophic_cycle",
            "name": "Gonotrophic cycle",
            "from": "adult_dev",
            "to": "adult",
            "value": [["adult_mort", "adult"], "0"]
        }
    ],
    "transformations": [
        {
            "id": "adult_death",
            "name": "Adult females dying today",
            "value": ["adult_mort", "adult"]
        }, {
            "id": "num_gravid",
            "name": "Number of gravid females",
            "value": ["adult_dev", "adult"]
        },{
            "id": "egg_laying",
            "name": "Egg laying at the end of gonotrophic cycle",
            "value": ["poisson", ["*", "num_gravid", 10]]
        }
    ]
}
```

![Erlang-distributed adult lifetime and gonotrophic cycle](figures/ex3b.png "Stochastic - Erlang-distributed")

### Linking rates to pseudo-states



# SandBox

<div class="myGroup">
<textarea id="model" class="myJSON">
</textarea>
<button onclick="process(['model','output','error'])">Parse</button>
<textarea id="output" class="myCode">
</textarea>
<textarea id="error" class="myError">
</textarea>
</div>

# Usage examples
