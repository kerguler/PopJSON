<style>
r { color: Red }
o { color: Orange }
g { color: Green }
.hidden {
    display: none;
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

JSON representation of the dynamically-structured matrix population models with multiple processes.

## Using the library

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
            "id": "adult",
            "name": "Adult females",
            "processes": [
                {
                    "id": "adult_lifetime",
                    "name": "Adult female lifetime",
                    "arbiter": "ACC_ERLANG",
                    "value": [10, 4]
                }
            ]
        }
    ]
}
```

![Erlang-distributed adult female mosquito lifetime](figures/ex1a.png "Deterministic - Erlang-distributed")
![Erlang-distributed adult female mosquito lifetime](figures/ex1b.png "Stochastic - Erlang-distributed")

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
