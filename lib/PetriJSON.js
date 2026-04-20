/* 
 *  odeJSON: ODE parser plugin for PopJSON.
 * 
 *  Copyright (C) 2026  Kamil Erguler <kerguler@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

'use strict';

const { PopJSON } = require('./index.js');

class PetriJSON extends PopJSON {

    write_model() {
        // Detect ODE + top-level processes (Petri-net style)
        if (this.json?.model?.type === "ODE" && 
            Array.isArray(this.json.processes) &&
            this.json?.model?.parameters?.algorithm === "rk8pd") {
            this.parse();
        } else {
            super.write_model();
        }
    }

    write_header() {
        const that = this;

        this.header += "#include <stdio.h>\n";
        this.header += "#include <stdlib.h>\n";
        this.header += "#include <math.h>\n";
        this.header += "#include <gsl/gsl_odeiv2.h>\n";
        this.header += "#include <gsl/gsl_errno.h>\n";

        this.header += "\n";

        this.header += `#define NumPop ${this.numpop}\n`;
        this.header += `#define NumEnv ${this.numenv}\n`;
        this.header += `#define NumPar ${this.numpar}\n`;
        this.header += `#define NumInt ${this.numint}\n`;

        this.header += "\n";

        this.json.parameters
            .filter(p => !p.constant)
            .forEach((p, i) => {
                that.header += `#define ${p.id} ${i}\n`;
            });

        this.header += "\n";

        // constants
        this.json.parameters
            .filter(p => p.constant)
            .forEach(p => {
                that.header += `double ${p.id} = ${p.value};\n`;
            });

        this.header += "\n";

        // env arrays
        this.json.environ?.forEach(env => {
            that.header += `double *envir_${env.id};\n`;
        });

        this.header += "\n";
        //
        this.header += "int TIME;\n";
        this.header += "int TIMEF;\n";
        this.header += "\n";

        this.header += "double *model_param;\n";

        this.header += "\n";

        this.header += "gsl_odeiv2_system ODE_SYSTEM;\n";
        this.header += "gsl_odeiv2_driver *ODE_DRIVER;\n";
        this.header += `double ODE_HSTART = ${this.json.model.parameters.hstart || 1e-3};\n`;
        this.header += `double ODE_EPS_ABS = ${this.json.model.parameters.eps_abs || 1e-8};\n`;
        this.header += `double ODE_EPS_REL = ${this.json.model.parameters.eps_rel || 1e-8};\n`;

        this.header += "\n";
    }

    write_init() {
        this.model += "void init(int *no, int *np, int *ni, int *ne, int *st) {\n";
        this.model += "    ODE_SYSTEM.function = ode_func;\n";
        this.model += "    ODE_SYSTEM.jacobian = 0;\n";
        this.model += "    ODE_SYSTEM.dimension = NumPop;\n";
        this.model += "    ODE_SYSTEM.params = NumPar;\n";

        this.model += "\n";
        
        this.model += "    ODE_DRIVER = gsl_odeiv2_driver_alloc_y_new(\n";
        this.model += "        &ODE_SYSTEM,\n";
        this.model += "        gsl_odeiv2_step_rk8pd,\n";
        this.model += "        ODE_HSTART,\n";
        this.model += "        ODE_EPS_ABS,\n";
        this.model += "        ODE_EPS_REL\n";
        this.model += "    );\n";

        this.model += "\n";

        this.model += "    *no = NumPop;\n";
        this.model += "    *np = NumPar;\n";
        this.model += "    *ni = NumInt;\n";
        this.model += "    *ne = NumEnv;\n";
        this.model += "    *st = 0;\n";
        this.model += "}\n";

        this.model += "\n";
    }

    write_RHS_functions() {
        const that = this;

        this.model += "int ode_func(double t, const double y[], double f[], void *params) {\n";
        this.model += "    int k = (int)floor(t);\n";
        this.model += "    if (k < 0) k = 0;\n";
        this.model += "    if (k >= TIMEF) k = TIMEF - 1;\n";

        this.model += "\n";

        // reset derivatives
        this.json.populations.forEach((pop, i) => {
            this.model += `    f[${i}] = 0.0;\n`;
        });

        this.model += "\n";

        this.json.processes.forEach(proc => {

            let rate = this.parse_value(proc.value);

            // adapt env indexing: ["prec", TIME_1] -> envir_prec[k]
            rate = rate.replace(/envir_([a-zA-Z0-9_]+)\[\(int\)\(TIME-1\)\]/g, "envir_$1[k]");

            this.model += `    double rate_${proc.id} = ${rate};\n`;

            // FROM (negative)
            if (proc.from) {
                Object.entries(proc.from).forEach(([pop, coeff]) => {
                    let idx = that.populations.indexOf(pop);
                    this.model += `    f[${idx}] -= ((double)${coeff}) * rate_${proc.id};\n`;
                });
            }

            // TO (positive)
            if (proc.to) {
                Object.entries(proc.to).forEach(([pop, coeff]) => {
                    let idx = that.populations.indexOf(pop);
                    this.model += `    f[${idx}] += ((double)${coeff}) * rate_${proc.id};\n`;
                });
            }

            this.model += "\n";
        });

        this.model += "    return GSL_SUCCESS;\n";
        this.model += "}\n\n";
    }

    write_sim() {
        const that = this;

        this.model += "void sim(int *tf, int *rep, double *envir, double *pr, double *y0, char **file_from, char **file_to, double *ret, double *iret, int *success) {\n";

        this.model += `    int dim = ${this.numpop};\n`;
        this.model += "    double y[NumPop];\n";

        // initial conditions
        this.json.populations.forEach((pop, i) => {
            this.model += `    y[${i}] = 0.0;\n`;
        });

        this.model += "\n";

        this.model += "    TIME = 0;\n";
        this.model += "    TIMEF = *tf;\n";
        this.model += "    double t = 0.0;\n";

        this.model += "    for (int i = 1; i <= 100; i++) {\n";
        this.model += "        double ti = i;\n";
        this.model += "        int status = gsl_odeiv2_driver_apply(d, &t, ti, y);\n";

        this.model += "        if (status != GSL_SUCCESS) {\n";
        this.model += "            printf(\"error\\n\");\n";
        this.model += "            break;\n";
        this.model += "        }\n";

        this.model += "        printf(\"%g\", t);\n";

        this.json.populations.forEach((pop, i) => {
            this.model += ` printf(\" %g\", y[${i}]);`;
        });

        this.model += " printf(\"\\n\");\n";
        this.model += "    }\n\n";

        this.model += "    gsl_odeiv2_driver_free(d);\n";
        this.model += "    return 0;\n";
        this.model += "}\n";
    }

    check_ids(id) {
        if (!/^[a-zA-Z][a-zA-Z0-9\_]+$/.test(id)) {
            this.error += "Only numeric and alphanumeric characters and \"_\" are allowed in IDs. Also, IDs should be more than one character and start with an alphanumeric.\nViolating ID is " + id + "\n";
            return "";
        }
        if (this.ids.includes(id)) {
            this.error += "IDs should be unique! Please check " + id + "\n";
            return "";
        }
        this.ids.push(id);
        return id;
    }

    parse() {
        this.json = super.handle_repeats(this.json);
        //
        const that = this;
        this.ids = [];
        //
        if (!('environ' in this.json)) this.json['environ'] = [];
        this.environs = this.json['environ'].map( (pr) => that.check_ids(pr['id']) );
        this.numenv = this.environs.length;
        //
        this.parametersv = this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => that.check_ids(pr['id']) );
        this.parametersc = this.json['parameters'].filter( (p) => p['constant'] ).map( (pr) => pr['id'] );
        if (!('functions' in this.json)) this.json['functions'] = [];
        this.numpar = this.json['parameters'].filter( (p) => !p['constant'] ).length;
        //
        this.functions = Object.keys(this.json['functions']);
        //
        this.numint = 0;
        //
        if (!('populations' in this.json)) {
            this.json['populations'] = [];
            this.error += "Couldn't find any populations!\n";
            return(super.results());
        }
        this.populations = this.json['populations'].map( (pr) => that.check_ids(pr['id']) );
        this.numpop = this.json['populations'].length;
        //
        this.processobj = {};
        this.intermediates = [];
        this.transformations = [];
        this.transfers = [];
        //
        this.deterministic = true;
        //
        this.operations = ["abs","min","max","round","poisson","binomial","define","?","&&","||",">=","<=",">","<","==","sqrt","pow","exp","log","log2","log10","indicator","index","size","count","*","+","-","/","%","!"];
        this.funparnames = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
                            "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
        //
        this.header = "";
        this.model = "";
        if (!this.error)
            this.write_ode_petrinet();
    }

    write_ode_petrinet() {
        const that = this;

        super.write_warranty();
        this.write_header();
        this.write_RHS_functions();
        this.write_init();
        this.write_sim();
        super.write_main();
    }
}

module.exports = { PetriJSON };