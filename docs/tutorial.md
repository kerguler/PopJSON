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

We propose a JSON representation of dynamically-structured matrix population models, which can accommodate multiple processes, such as survival, development, and egg laying. At present, PopJSON deals only with the <code>sPop</code> models of Erguler et al. \[<a href="https://f1000research.com/articles/7-1220/v3" target="_blank" rel="noreferrer">sPop</a>, <a href="https://www.nature.com/articles/s41598-022-15806-2" target="_blank" rel="noreferrer">sPop2</a>, <a href="https://github.com/kerguler/Population" target="_blank" rel="noreferrer">Population</a>\], but soon, and with your help, it will expand to cover more.

## Using the library

The parser takes a JSON string, or a file, and converts it to its raw ANSI C equivalent. You'll then need to compile this in your system and run using the <a href="https://github.com/kerguler/Population" target="_blank" rel="noreferrer">Population</a> package. The package includes a wrapper for <code>Python</code>, and soon, will have another one for <code>R</code>.

We are working on developing the <a href="https://VEClim.com" target="_blank" rel="noreferrer">VEClim</a> API to take PopJSON requests and run sample simulations. But this shall be used to preview models. We will not reproduce the functionality of advanced analysis tools.

### Model definition

To define the algorithm and its parameters for simulation. For now, only <code>Population</code> is defined (<code>ODE</code> is pending), which could be either deterministic or stochastic. To set the precision of the accumulative process indicator, <code>istep</code> is used. This effectively limits the maximum number of pseudo-stage classes.
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

```json
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
```


![Erlang-distributed larva lifetime and development time](figures/ex2a.png "Deterministic - Erlang-distributed")

### Declaring cyclic development (gonotrophic cycle)



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
