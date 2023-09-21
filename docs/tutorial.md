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
Please note that we declared the mean and standard deviation of development in terms of two variables, **d2m** and **d2s**, which we will define shortly.

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

What we need now is a function to transform temperature into the mean and standard deviation of development (to define **d2m** and **d2s**).
```json
{
    "functions": {
        "briere1": ["define", ["T","B","E","a"], ["?",["<=","T","B"],"365.0", ["?",[">=","T","E"],"365.0", ["min","365.0",["max","1.0", ["/","1.0",["exp", ["+","a",["log","T"],["log",["-","T","B"]],["*","0.5",["log",["-","E","T"]]]]]]]]]]],
    }
}
```

Please note that the function definition begins with **define**, which is a key word (an operator). The second in the list is the list of parameters (only a single letter for each), and the last is the equation. The mathematical equation notation were inspired from the <a href="https://cortexjs.io/math-json/" target="_blank" rel="noreferrer">MathJSON</a> representation of CortexJS. The list of all operators can be found <a href="#operators-for-equations">here</a>.

This is the reciprocal of the <a href="https://doi.org/10.1093/ee/28.1.22" target="_blank" rel="noreferrer">Briere-1</a> function commonly used in ecological modelling.
<p style="text-align: center; font-size: 1.5rem;"> 
$$ briere_1(T,a,L,R) = e^{-\left\{a + \ln(T) + \ln(T-L) + 0.5\ln(R-T)\right\}} $$
</p>
The function returns a large value (365 days of development) outside of **L** and **R** and stays within 1 and 365 for all **T**. Please note that the accumulative development algorithm in `Population` currently cannot handle mean process time smaller than a unit.

The Briere-1 function translates into the following C code:
```C
double dmin(double a, double b) { return a < b ? a : b; }
double dmax(double a, double b) { return a > b ? a : b; }

#define briere1(T,a,L,R) ((((T) <= (L))) ? (365.0) : ((((T) >= (R))) ? (365.0) : (dmin(365.0, dmax(1.0, (1.0 / exp(((a) + log((T)) + log(((T) - (L))) + (0.5 * log(((R) - (T))))))))))))
```

Next, we connect temperature with **d2m** and **d2s** using the **intermediates** tag. A key feature of this tag is that it computes first at the beginning of each iteration.

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

Here, **index** is an operator extracting the **TIME_1**<sup>th</sup> element in the array of **temp** (temperature). **TIME_1** refers to the previous time point (we used yesterday's conditions to estimate today's population).

Lastly, we define the **parameters** of the model to complete the temperature dependency of development time. For each, we declare an **id**, a **value**, and weather the parameter is fixed (**constant:true**) or user-defined (**constant:false**). Minimum an maximum values can also be defined using **min** and **max** tags, which could be useful when performing optimisation.


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
See [ex1E.json](./examples/ex1E.json) and [ex1E.c](./examples/ex1E.c) for the full PopJSON representation and the C translation.

<div class="myFigures">

![Larva development time](figures/ex1Ebr.png "Briere1 function")

![Larva development](figures/ex1E.png "Deterministic - Erlang-distributed - variable temperature")

</div>

## Declaring multiple processes

The `Population` algorithm enables declaring multiple processes on a population. For instance, we could define lifetime and development time together having independent Erlang-distributed durations. Strinctly, they would not be completely independent as the processes take place in the order they are defined. This means that if we define development before larva mortality, pupa could be produced under conditions not suitable for larva survival. We do not what that I guess.

Here, we define the two processes for larva (in the plausible order) and follow the dynamics.

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

The following **transformations** tag is needed to bind **larva_mort** to **larva_death** and **larva_dev** to **larva_to_pupa** to make them visible (remember that internal population structure is hidden by default). Otherwise, only the size of the **larva** population is monitored.

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

The resulting larva population size and the number of larvae completing each process (mortality or pupa production) is given below.

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

# Operators for equations

| Operator    | Parameters    | Definition   |
| :---        | :---          | :---         |
| min         | a,b           | Minimum of two numbers |
| min         | a,b           | Maximum of two numbers |
| round       | a             | Rounds to the nearest integer |
| sqrt        | a             | Square root |
| pow         | a,b           | Power of a to b |
| exp         | a             | Exponential function |
| log         | a             | Natural logarithm |
| log2        | a             | Logarithm of a to base 2 |
| log10       | a             | Logarithm of a to base 10 |
| indicator   | a             | Indicator function (boolean to integer) |
| index       | a,b           | Value of an array at position b (counts from 0) |
| *           | ...           | Multiplication |
| +           | ...           | Addition |
| -           | ...           | Subtraction (in the given order) |
| /           | ...           | Division (in the given order) |
| size        | a             | Total size of a population |
| poisson     | a             | Generates a Poisson random number with lambda=a (works only when **deterministic:false**) |
| binomial    | a,b           | Generates a Binomial random number with n=a and p=b (works only when **deterministic:false**) |
| define      | a,b           | Function definition with parameters a and equation b |
| ?           | a,b,c         | Condition expression (if a is true, return b, else, return c) |
| &&          | ...           | Logical AND |
| \|\|        | ...           | Logical OR |
| >           | a,b           | Logical greater than |
| <           | a,b           | Logical smaller than |
| >=          | a,b           | Logical greater than or equal |
| <=          | a,b           | Logical smaller than or equal |
| ==          | a,b           | Logical equal to |

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

