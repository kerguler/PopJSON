'use strict';

const fs = require('fs');
const util = require('util');

class PopJSON {
    constructor(filename) {
      this.filename = filename;
      let data = fs.readFileSync(this.filename);
      this.json = JSON.parse(data);
      this.deterministic = this.json['modelTypes'][this.json['model']['type']]['deterministic']
      this.model = "";
      this.write_model();
    }
    write_model() {
        this.model = "";
        this.write_header();
        this.write_functions();
        this.write_init();
        this.write_parnames();
        this.write_destroy();
        // this.write_inter();
        this.write_sim();
        this.write_main();
    }
    write_header() {
        let that = this;
        if (this.json['model']['type'] == "Population") {
            this.model += "#include <math.h>\n";
            this.model += "#include \"population.h\"\n";
            this.model += "\n";
            if (!this.deterministic) {
                this.model += "extern gsl_rng *RANDOM;\n";
                this.model += "\n";
            } else if (this.json['model']['type'] == "ODE") {
                this.model += "#include <stdlib.h>\n";
                this.model += "#include \"lsoda.h\"\n";
                this.model += "\n";
            }
        }
        this.model += "#define NumPar " + util.format(Object.keys(this.json['parameters']).length) + "\n";
        this.model += "#define NumPop " + util.format(Object.keys(this.json['populations']).length) + "\n";
        this.model += "\n";
        this.json['parameters'].forEach( (pr, i) => {
            that.model += "#define " + pr['id'] + " " + util.format(i) + "\n";
        } );
        this.model += "\n";
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc, i) => {
                spc['processes'].forEach( (prc, j) => {
                    that.model += "#define " + prc['id'] + " " + util.format(j) + "\n";
                } );
            } );
            this.model += "\n";
        }
    }
    write_functions() {
        let that = this;
        if ('functions' in this.json) {
           Object.entries(this.json['functions']).forEach( ([key, value]) => {
                that.model += value + "\n";
           });
           this.model += "\n";
        }
    }
    write_init() {
        this.model += "void init(int *np, int *nm) {\n";
        if (this.json['model']['type'] == "Population") {
            if (!this.deterministic) {
                this.model += "    spop2_random_init();\n";
                this.model += "\n";
            }
            if ('istep' in this.json['modelTypes'][this.json['model']['type']]['parameters']) {
                this.model += "    spop2_set_eps(" + util.format(this.json['modelTypes'][this.json['model']['type']]['parameters']['istep']) + ");\n";
                this.model += "\n";
            }
        }
        this.model += "    *np = NumPar;\n";
        this.model += "    *nm = NumPop;\n";
        this.model += "}\n";
        this.model += "\n";
    }
    write_parnames() {
        let that = this;
        this.model += "void parnames(char **names, double *param) {\n";
        this.model += "    char temp[NumPop+NumPar][256] = {\n";
        this.model += "        \"" + this.json['populations'].map( (s) => s['id'] ).join("\", \"") + "\",\n";
        this.model += "        \"" + this.json['parameters'].map( (pr) => pr['id'] ).join("\", \"") + "\"\n";
        this.model += "    };\n";
        this.model += "\n";
        this.model += "    int i;\n";
        this.model += "    for (i=0; i<(NumPop+NumPar); i++)\n";
        this.model += "        names[i] = strdup(temp[i]);\n";
        this.model += "\n";
        this.json['parameters'].forEach( (pr, i) => {
            that.model += "    param[" + pr['id'] + "] = " + util.format('value' in pr ? pr['value'] : 0.0) + ";\n";
        } );
        this.model += "}\n";
        this.model += "\n";
    }
    write_destroy() {
        this.model += "void destroy(void) {\n";
        if (this.json['model']['type'] == "Population" && !this.deterministic) {
                this.model += "    spop2_random_destroy();\n";
        }
        this.model += "}\n";
        this.model += "\n";
    }
    write_inter() {
        if (!('intermediates' in this.json)) {
            return;
        }
        let that = this;
        this.model += "void get_intermediates(double *envir, double *pr, double *ret) {\n"
        if ('environ' in this.json) {
            this.json['environ'].forEach( (elm, i) => {
                 that.model += "    double *" + elm['id'] + " = envir + " + util.format(i) + " * tf;\n";
            });
        }
        this.model += "\n";
        this.json['intermediates'].forEach( (elm, i) => {
            this.model += "    ret[" + util.format(i) + "] = " + elm['value'] + ";\n";
        } );
        this.model += "}\n";
        this.model += "\n";
    }
    write_out(tab) {
        let that = this;
        if (this.deterministic) {
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    ".repeat(tab) + "ret[" + util.format(i) + "] = size_" + spc['id'] + ".d;\n";
            } );
        } else {
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    ".repeat(tab) + "ret[" + util.format(i) + "] = (double)(size_" + spc['id'] + ".i);\n";
            } );
        }
        this.model += "\n";
        this.json['populations'].forEach( (spc, i) => {
            that.model += "    ".repeat(tab) + "dret[" + util.format(i) + "] = 0.0;\n";
        } );
        this.model += "\n";
        this.model += "    ".repeat(tab) + "ret += " + (util.format(this.json['populations'].length)) + ";\n";
        this.model += "    ".repeat(tab) + "dret += " + (util.format(this.json['populations'].length)) + ";\n";
        this.model += "\n";
    }
    write_sim() {
        let i, j;
        let that = this;
        let numpop = null;
        let numproc = null;
        let numprocpar = null;
        let det = this.deterministic ? 'DETERMINISTIC' : 'STOCHASTIC';
        this.model += "void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *dret, int *success) {\n";
        if ('environ' in this.json) {
           this.json['environ'].forEach( (elm, i) => {
                that.model += "    double *" + elm['id'] + " = envir + " + util.format(i) + " * tf;\n";
           });
           this.model += "\n";
        }
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "    population " + util.format(spc['id']) + ";\n";
            });
            this.model += "\n";
        }
        if (this.json['model']['type'] == "Population") {
            numpop = this.json['populations'].length;
            numproc = 1 + Math.max(...this.json['populations'].map( (s) => s['processes'].length ));
            numprocpar = Math.max(...this.json['populations'].map( (spc, i) => {
                let pars = 0;
                spc['processes'].forEach( (s) => {
                    if (Array.isArray(s['value'])) {
                        pars += s['value'].length;
                    } else {
                        pars += 1;
                    }
                } );
                return pars;
            } ));
            this.model += "    number num = numZERO;\n";
            this.model += "    number harvest;\n";
            this.model += "    char arbiters[" + util.format(numproc) + "];\n";
            this.model += "    number key[" + util.format(numproc) + "];\n";
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    number size_" + spc['id'] + ";\n";
            } );
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    number completed_" + spc['id'] + "[" + util.format(numproc) + "];\n";
            } );
            this.model += "    double par[" + util.format(numprocpar) + "];\n";
            this.model += "\n";
            this.json['populations'].forEach( (spc, i) => {
                for (j=0; j<numproc; j++) {
                    that.model += "    arbiters[" + util.format(j) + "] = " + (j < spc['processes'].length ? spc['processes'][j]['arbiter'] : "STOP") + ";\n";
                    that.model += "    key[" + util.format(j) + "] = numZERO;\n";
                }
                that.model += "    " + spc['id'] + " = spop2_init(arbiters, " + det + ");\n";
                that.model += "    if (y0[" + util.format(i) + "]) { num." + ("i",that.deterministic ? "d" : "i") + " = y0[" + util.format(i) + "]; spop2_add(" + spc['id'] + ", key, num); }\n";
                that.model += "\n";
            });
            this.model += "\n";
        }
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm, i) => {
                // that.model += "    double " + elm['id'] + " = y0[" + util.format(this.json['populations'].length + i) + "];\n";
                that.model += "    double " + elm['id'] + " = 0.0;\n";
            });
            this.model += "\n";
        }
        //
        this.model += "    int tm = 0;\n";
        this.json['populations'].forEach( (spc, i) => {
            that.model += "    size_" + spc['id'] + " = spop2_size(" + spc['id'] + ");\n";
        } );
        this.model += "\n";
        this.write_out(1);
        this.model += "    for (tm=1; tm<tf; tm++) {\n";
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm) => {
                this.model += "        " + elm['id'] + " = " + elm['value'] + ";\n";
            });
            this.model += "\n";
        }
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc, i) => {
                let pars = [];
                spc['processes'].forEach( (s) => {
                    if (Array.isArray(s['value'])) {
                        s['value'].forEach( (ss) => pars.push(ss) );
                    } else {
                        pars.push(s['value']);
                    }
                } );
                for (j = 0; j < numprocpar; j++) {
                    that.model += "        par[" + util.format(j) + "] = " + (pars.length ? pars.shift() : "0.0") + ";\n";
                }
                that.model += "        spop2_step(" + spc['id'] + ", par, &size_" + spc['id'] + ", completed_" + spc['id'] + ", 0);\n";
                that.model += "\n";
            });
            //
            if ('transformations' in this.json) {
                this.json['transformations'].forEach( (trx) => {
                    that.model += "        harvest = " + (trx['harvest'] == "size" ? "size_" + trx['from'] : "completed_" + trx['from'] + "[" + trx['harvest'] + "]") + ";\n";
                    that.model += "        spop2_add(" + trx['to'] + ", key, harvest);\n";
                    if (that.deterministic) {
                        that.model += "        size_" + trx['to'] + ".d += harvest.d;\n";
                    } else {
                        that.model += "        size_" + trx['to'] + ".i += harvest.i;\n";
                    }
                    that.model += "\n";
                } );
            }
            //
            this.write_out(2)
        }
        //
        this.model += "    }\n";
        this.model += "\n";
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "    spop2_free(&" + spc['id'] + ");\n";
            });
            this.model += "\n";
        }
        this.model += "    *success = 1;\n";
        this.model += "}\n";
        this.model += "\n";
    }
    write_main() {
        this.model += "int main(int argc, char *argv[]) {\n";
        this.model += "    return 0;\n";
        this.model += "}";
        this.model += "\n";
    }
}

exports.parse = PopJSON;
