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
.myError
.myCode,
.myJSON {
    width: 100%;
    height: 300px;
}
.myError {
    color: red;
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

To define the algorithm and its parameters for simulation. For now, only <code>Population</code> is defined (<code>ODE</code> is pending), which could be either deterministic or stochastic. To set the precision of the accumulative process indicator, <code>istep</code> is used. This effectively limits the maximum number of pseudo-stage classes.
```json
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
```
<div class="myGroup">
<textarea id="model0" class="hidden">
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
            "name": "Larva",
            "processes": []
        }
    ]
}
</textarea>
<textarea id="output0" class="myCode">
</textarea>
<textarea id="error0" class="myError">
</textarea>
<script>process(['model0','output0','error0']);</script>
</div>

# SandBox

<div class="myGroup">
<textarea id="model" class="myJSON">
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
        "environ": [
            {
                "id": "atemp",
                "name": "Mean air temperature (°C)",
                "url": ""
            }
        ],
        "populations": [
            {
                "id": "larva",
                "name": "Larva",
                "processes": [
                    {
                        "id": "larva_mort",
                        "name": "Larva mortality",
                        "arbiter": "NOAGE_CONST",
                        "value": "pr_larva_mort"
                    }, {
                        "id": "larva_dev",
                        "name": "Larva development",
                        "arbiter": "ACC_FIXED",
                        "value": "pr_larva_dev"
                    }
                ]
            },{
                "id": "pupa",
                "name": "Pupa",
                "processes": [
                ]
            }
        ],
        "intermediates": [
            {
                "id": "pr_larva_dev_raw",
                "value": ["round", ["dmax", "0.0", ["+", "pr_d2_dev_0", ["*", "pr_d2_dev_1", ["index", "stemp", ["-", "TIME", "1"]]], ["*", "pr_d2_dev_2", ["pow", ["index", "stemp", ["-", "TIME", "1"]], "2"]]]]]
            }, {
                "id": "pr_larva_dev",
                "value": ["*", "pr_dens_imp", "pr_larva_dev_raw"]
            }, {
                "id": "pr_larva_surv_perc",
                "value": ["dmax", "0.0", ["dmin", "1.0", ["*", "0.01", ["+", "pr_p2_pcent_0", ["*", "pr_p2_pcent_1", ["index", "stemp", ["-", "TIME", "1"]]], ["*", "pr_p2_pcent_2", ["pow", ["index", "stemp", ["-", "TIME", "1"]], "2"]]]]]]
            }, {
                "id": "pr_larva_surv",
                "value": ["-", "1.0", ["pow", "pr_larva_surv_perc", ["/","1.0","pr_larva_dev"]]]
            }, {
                "id": "pr_larva_mort",
                "value": ["-", "1.0", ["/", ["-", "1.0", "pr_larva_surv"], ["pow", "pr_dens_imp", "2.0"]]]
            }
        ],
        "transformations": [
            {
                "id": "larva_to_pupa",
                "name": "Larva to pupa",
                "to": "pupa",
                "value": ["larva_dev", "larva"]
            }
        ],
        "transfers": [
            {
                "id": "gonotrophic_cycle",
                "name": "Gonotrophic cycle",
                "from": "adult_dev",
                "to": "adult",
                "value": [["adult_life", "adult"], "0"]
            },{
                "id": "juvenile_to_adult",
                "name": "Juveniles becoming adults",
                "from": "juvenile_dev",
                "to": "adult",
                "value": [["juvenile_life", "juvenile"], "0"]
            }
        ],
        "parameters": [
            {
                "id": "sex_ratio",
                "constant": true,
                "name": "Sex ratio",
                "value": 0.5
            },{
                "id": "pr_stdev",
                "constant": true,
                "name": "Standard deviation of mean lifetime for adults",
                "value": 0.5
            },{
                "id": "pr_CPP_0",
                "constant": false,
                "name": "Critical photoperiod vs latitude no. 0",
                "value": 12.0
            },{
                "id": "pr_CPP_1",
                "constant": false,
                "name": "Critical photoperiod vs latitude no. 1",
                "value": 2.0
            },{
                "id": "pr_d1_dev_0",
                "constant": false,
                "name": "Temperature-driven egg development no.0",
                "value": 0.0
            },{
                "id": "pr_d1_dev_1",
                "constant": false,
                "name": "Temperature-driven egg development no.1",
                "value": 0.0
            },{
                "id": "pr_d1_dev_2",
                "constant": false,
                "name": "Temperature-driven egg development no.2",
                "value": 0.0
            },{
                "id": "pr_d2_dev_0",
                "constant": false,
                "name": "Temperature-driven larva development no.0",
                "value": 0.0
            },{
                "id": "pr_d2_dev_1",
                "constant": false,
                "name": "Temperature-driven larva development no.1",
                "value": 0.0
            },{
                "id": "pr_d2_dev_2",
                "constant": false,
                "name": "Temperature-driven larva development no.2",
                "value": 0.0
             },{
                "id": "pr_d3_dev_0",
                "constant": false,
                "name": "Temperature-driven pupa development no.0",
                "value": 0.0
            },{
                "id": "pr_d3_dev_1",
                "constant": false,
                "name": "Temperature-driven pupa development no.1",
                "value": 0.0
            },{
                "id": "pr_d3_dev_2",
                "constant": false,
                "name": "Temperature-driven pupa development no.2",
                "value": 0.0
             },{
                "id": "pr_d4j_dev_0",
                "constant": false,
                "name": "Temperature-driven juvenile development no.0",
                "value": 0.0
            },{
                "id": "pr_d4j_dev_1",
                "constant": false,
                "name": "Temperature-driven juvenile development no.1",
                "value": 0.0
            },{
                "id": "pr_d4j_dev_2",
                "constant": false,
                "name": "Temperature-driven juvenile development no.2",
                "value": 0.0
             },{
                "id": "pr_d4gc_dev_0",
                "constant": false,
                "name": "Temperature-driven gonotrophic cycle length no.0",
                "value": 0.0
            },{
                "id": "pr_d4gc_dev_1",
                "constant": false,
                "name": "Temperature-driven gonotrophic cycle length no.1",
                "value": 0.0
            },{
                "id": "pr_d4gc_dev_2",
                "constant": false,
                "name": "Temperature-driven gonotrophic cycle length no.2",
                "value": 0.0
             },{
                "id": "pr_d4_dev_0",
                "constant": false,
                "name": "Temperature-driven adult (female) life expectancy no.0",
                "value": 0.0
            },{
                "id": "pr_d4_dev_1",
                "constant": false,
                "name": "Temperature-driven adult (female) life expectancy no.1",
                "value": 0.0
            },{
                "id": "pr_d4_dev_2",
                "constant": false,
                "name": "Temperature-driven adult (male) life expectancy no.2",
                "value": 0.0
             },{
                "id": "pr_d4m_ratio",
                "constant": false,
                "name": "Adult male life expectancy compared to adult female life expectancy",
                "value": 0.0
            },{
                "id": "pr_f4_gc_0",
                "constant": false,
                "name": "Temperature-driven number of eggs per female no.0",
                "value": 0.0
            },{
                "id": "pr_f4_gc_1",
                "constant": false,
                "name": "Temperature-driven number of eggs per female no.1",
                "value": 0.0
            },{
                "id": "pr_f4_gc_2",
                "constant": false,
                "name": "Temperature-driven number of eggs per female no.2",
                "value": 0.0
            },{
                "id": "pr_p0_0",
                "constant": false,
                "name": "Temperature-driven diapausing egg survival no.0",
                "value": 0.0
            },{
                "id": "pr_p0_1",
                "constant": false,
                "name": "Temperature-driven diapausing egg survival no.1",
                "value": 0.0
            },{
                "id": "pr_p1_pcent_0",
                "constant": false,
                "name": "Temperature-driven egg survival no.0",
                "value": 0.0
            },{
                "id": "pr_p1_pcent_1",
                "constant": false,
                "name": "Temperature-driven egg survival no.1",
                "value": 0.0
            },{
                "id": "pr_p1_pcent_2",
                "constant": false,
                "name": "Temperature-driven egg survival no.2",
                "value": 0.0
            },{
                "id": "pr_p2_pcent_0",
                "constant": false,
                "name": "Temperature-driven larva survival no.0",
                "value": 0.0
            },{
                "id": "pr_p2_pcent_1",
                "constant": false,
                "name": "Temperature-driven larva survival no.1",
                "value": 0.0
            },{
                "id": "pr_p2_pcent_2",
                "constant": false,
                "name": "Temperature-driven larva survival no.2",
                "value": 0.0
             },{
                "id": "pr_p3_pcent_0",
                "constant": false,
                "name": "Temperature-driven pupa survival no.0",
                "value": 0.0
            },{
                "id": "pr_p3_pcent_1",
                "constant": false,
                "name": "Temperature-driven pupa survival no.1",
                "value": 0.0
            },{
                "id": "pr_p3_pcent_2",
                "constant": false,
                "name": "Temperature-driven pupa survival no.2",
                "value": 0.0
            },{
                "id": "pr_dens_thr",
                "constant": false,
                "name": "Impact of larva density (threshold)",
                "value": 0.0
            },{
                "id": "pr_dens_str",
                "constant": false,
                "name": "Impact of larva density (steepness)",
                "value": 0.0
            },{
                "id": "pr_dens_scl",
                "constant": false,
                "name": "Impact of larva density (KCap scale - log)",
                "value": 0.0
            },{
                "id": "pr_hatch_temp",
                "constant": false,
                "name": "Temperature threshold for egg hatching",
                "value": 0.0
            }
       ]
    }
</textarea>
<button onclick="process(['model','output','error'])">Parse</button>
<textarea id="output" class="myCode">
</textarea>
<textarea id="error" class="myError">
</textarea>
</div>

# Usage examples
