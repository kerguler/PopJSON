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

We propose a JSON representation for dynamically-structured multi-process matrix population models. PopJSON, as we call it, deals with the `sPop` models of Erguler et al. \[<a href="https://f1000research.com/articles/7-1220/v3" target="_blank" rel="noreferrer">sPop</a>, <a href="https://www.nature.com/articles/s41598-022-15806-2" target="_blank" rel="noreferrer">sPop2</a>, <a href="https://github.com/kerguler/Population" target="_blank" rel="noreferrer">Population</a>\], for the time being, but soon, and with your help, it will cover more.

Here, we describe a PopJSON parser, which takes a JSON string, or a file, and converts it to its raw ANSI C equivalent. You'll then need to compile this code in your system and run it using the <a href="https://github.com/kerguler/Population" target="_blank" rel="noreferrer">Population</a> package. In this repository, we included a <a href="../population.py" target="_blank" rel="noreferrer">wrapper</a> to read and simulate the translated models in `Python`. Soon, we will write another one for `R`.

In parallel, we are working on developing the <a href="https://VEClim.com" target="_blank" rel="noreferrer">VEClim</a> API to take PopJSON requests and run simulations. But this shall take some time to complete.

# Instructions

PopJSON is a standard JavaScript Object Notation (JSON) ornamented with custom tags to describe the essentials of a model. Overall, we use the curly brackets \{\} to group related tags and square brackets \[\] to define processess with a strict order. We hope all will be clearer as you read along.

Please note that we used <a href="./plot.py" target="_blank" rel="noreferrer">this</a> script to simulate and plot all the examples on this page.

## Model definition

We first define a model by using the **model** tag. Here, the key tags are **type** and **parameters**. In this version of PopJSON, we covered the **Population** model, but we are working on including more canonical ODE, DDE, etc. models.

The dynamics can either be deterministic or stochastic, which is determined using the boolean tag **deterministic**. If needed, we can set the precision of the accumulative process indicator (this is specific to the Population package) with the **istep** tag.  This effectively limits the maximum number of pseudo-stage classes.

Before following the steps below, we recommend having a look at the <a href="https://kerguler.github.io/Population/" target="_blank" rel="noreferrer">Population</a> package description.

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

## Declaring a population (or a development stage)

A population, by definition, is a structured collection of similar individuals. There exists classes in a population that are invisible to the end user, but they distinguish intividuals into sub-groups. For example, an age-structured population of larvae is composed of indivudals all at their larva stage of development, however, some have been there for long but some have just started. The time they turn into pupae depends on how long they have been in the stage. 

```json
{
    "populations": [
        {
            "id": "larva",
            "name": "The larva stage",
            "processes": [
                {
                    "id": "larva_dev",
                    "name": "Larva development time",
                    "arbiter": "AGE_GAMMA",
                    "value": [10, 4]
                }
            ]
        }
    ]
}
```
See [ex1a.json](./examples/ex1a.json) and [ex1a.c](./examples/ex1a.c) for the full PopJSON representation and the C translation.

The above example represents a larva population with a development time of 10 days (with a standard deviation of 4 days). The population is age-structured and the development time is gamma-distributed.

<div class="myFigures">

![Gamma-distributed larva development time](figures/ex1a.png "Deterministic - Gamma-distributed")

![Three realisations of stochastic development in 100 larvae](figures/ex1b.png "Stochastic - Gamma-distributed")

</div>

## Dependence on environmental variables

Development is strongly dependent on temperature in many insect species. We have demonstrated that accumulative processes are suitable representations for development under variable environments (<a href="https://www.nature.com/articles/s41598-022-15806-2" target="_blank" rel="noreferrer">Erguler et al. 2022</a>). So, we will switch to **ACC_ERLANG** as the accumulative equivalent of the gamma-distributed age-structured development.

Next, we define an environmental variable, temperature, to feed into our model.
```json
{    
    "environ": [
        {
            "id": "temp",
            "name": "Temperature of the breeding pool (in °C)",
            "url": ""
        }
    ]
}
```

What we need now is a function to transform temperature into the mean and standard deviation of development (to replace the 10 and 4 in the previous example).
```json
{
    "functions": {
        "briere1": ["define", ["T","B","E","a"], ["?",["<=","T","B"],"365.0", ["?",[">=","T","E"],"365.0", ["min","365.0",["max","1.0", ["/","1.0",["exp", ["+","a",["log","T"],["log",["-","T","B"]],["*","0.5",["log",["-","E","T"]]]]]]]]]]],
    }
}
```

```C
double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

#define briere1(T,a,L,R) ((((T) <= (L))) ? (365.0) : ((((T) >= (R))) ? (365.0) : (dmin(365.0, dmax(1.0, (1.0 / exp(((a) + log((T)) + log(((T) - (L))) + (0.5 * log(((R) - (T))))))))))))
```

<p style="text-align: center; font-size: 1.5rem;"> 
$$ briere_1(T,a,L,R) = e^{-\left\{a + \ln(T) + \ln(T-L) + 0.5\ln(R-T)\right\}} $$
</p>

Outside **L** and **R**, the function returns a large value (365 days of development) and stays within 1 and 365 for all **T**. Please note that the accumulative development algorithm in `Population` currently cannot handle mean process time smaller than a unit.


```json
{
    "intermediates": [
        {
            "id": "d2m",
            "value": ["briere1", ["index","temp","TIME_1"], "d2m_a", "d2m_b", "d2m_c"]
        }, {
            "id": "d2s",
            "value": ["*", "d2s_c", "d2m"]
        }
    ]
}
```

```json
{
    "parameters": [
        {
            "id": "d2m_a",
            "constant": false,
            "name": "Larva development mean (a)",
            "value": -15
        }, {
            "id": "d2m_b",
            "constant": false,
            "name": "Larva development mean (b)",
            "value": 10
        }, {
            "id": "d2m_c",
            "constant": false,
            "name": "Larva development mean (c)",
            "value": 35
        }, {
            "id": "d2s_c",
            "constant": false,
            "name": "Larva development stdev (c)",
            "value": 0.2
        }
    ]
}
```

```json
{
    "populations": [
        {
            "id": "larva",
            "name": "The larva stage",
            "processes": [
                {
                    "id": "larva_dev",
                    "name": "Larva development time",
                    "arbiter": "ACC_ERLANG",
                    "value": ["d2m", "d2s"]
                }
            ]
        }
    ]
}
```
See [ex1E.json](./examples/ex1E.json) and [ex1E.c](./examples/ex1E.c) for the full PopJSON representation and the C translation.

<div class="myFigures">

![Larva development time](figures/ex1Ebr.png "Briere1 function")

![Larva development](figures/ex1E.png "Deterministic - Erlang-distributed - variable temperature")

</div>


## Declaring multiple processes

```json
{
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
See [ex2a.json](./examples/ex2a.json) and [ex2a.c](./examples/ex2a.c) for the full PopJSON representation and the C translation.

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

![Erlang-distributed larva lifetime and development time](figures/ex2a.png "Deterministic - Erlang-distributed")

# Advanced usage

## Declaring cyclic development (gonotrophic cycle)

```json
{
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
See [ex3a.json](./examples/ex3a.json) and [ex3a.c](./examples/ex3a.c) for the full PopJSON representation and the C translation.


![Erlang-distributed adult lifetime and gonotrophic cycle](figures/ex3a.png "Deterministic - Erlang-distributed")

```json
{
    "model": {
            "deterministic": false,
        },
    "transformations": [
        {
            "id": "egg_laying",
            "name": "Egg laying at the end of gonotrophic cycle",
            "value": ["poisson", ["*", "num_gravid", 10]]
        }
    ]
}
```
See [ex3b.json](./examples/ex3b.json) and [ex3b.c](./examples/ex3b.c) for the full PopJSON representation and the C translation.

![Erlang-distributed adult lifetime and gonotrophic cycle](figures/ex3b.png "Stochastic - Erlang-distributed")

## Linking rates with pseudo-states

```json
{
    "populations": [
        {
            "id": "adult_num_dev",
            "name": "Number of gonotrophic cycles in adult females",
            "arbiter": "AGE_CUSTOM",
            "stepper": "NO_STEPPER",
            "hazard": ["NOAGE_CONST", ["?",[">","total_eggs",150],1,0], 0],
            "value": []
        }
    ]
}
```

```json
{
    "transfers": [
        {
            "id": "gonotrophic_cycle",
            "name": "Gonotrophic cycle",
            "from": "adult_dev",
            "to": "adult",
            "value": [["adult_mort", "adult"], 0, ["+", ["adult_num_dev", "adult"], 1]]
        }
    ]
}
```

```json
{
    "transformations": [
        {
            "id": "egg_laying",
            "name": "Egg laying at the end of gonotrophic cycle",
            "value": ["*", "num_gravid", 1]
        },{
            "id": "total_eggs",
            "name": "Total number of eggs laid",
            "value": ["+", "total_eggs", "egg_laying"]
        }
    ]
}
```

See [ex4a.json](./examples/ex4a.json) and [ex4a.c](./examples/ex4a.c) for the full PopJSON representation and the C translation.

![Limited number of gonotrophic cycles](figures/ex4a.png "Deterministic - Erlang-distributed")

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

