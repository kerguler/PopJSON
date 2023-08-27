'use strict';

const fs = require('fs');
const { emitWarning } = require('process');
const util = require('util');

const arbiter = {
    'ACC_FIXED': 'd',
    'ACC_ERLANG': 'd',
    'ACC_PASCAL': 'd',
    'AGE_FIXED': 'i',
    'AGE_CONST': 'i',
    'AGE_GAMMA': 'i',
    'AGE_NBINOM': 'i',
    'AGE_CUSTOM': 'i',
    'NOAGE_CONST': 'i'
};

class PopJSON {
    constructor(filename) {
        let that = this;
        this.filename = filename;
        let data = fs.readFileSync(this.filename);
        this.json = JSON.parse(data);
        this.deterministic = this.json['modelTypes'][this.json['model']['type']]['deterministic']
        this.environs = 'environ' in this.json ? this.json['environ'].map( (pr) => pr['id'] ) : [];
        this.populations = this.json['populations'].map( (pr) => pr['id'] );
        this.processes = []; this.json['populations'].forEach( (pop) => 'processes' in pop ? pop['processes'].forEach( (pr) => { that.processes.push(pr['id']); } ) : [] );
        this.processobj = {}; this.json['populations'].forEach( (pop) => 'processes' in pop ? pop['processes'].forEach( (pr) => { that.processobj[pr['id']] = pr; that.processobj[pr['id']]['parent_id'] = pop['id']; } ) : {} );
        this.parametersv = this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => pr['id'] );
        this.parametersc = this.json['parameters'].filter( (p) => p['constant'] ).map( (pr) => pr['id'] );
        this.functions = 'functions' in this.json ? Object.keys(this.json['functions']) : [];
        this.intermediates = this.json['intermediates'].map( (pr) => pr['id'] );
        this.transformations = this.json['transformations'].map( (pr) => pr['id'] );
        this.transfers = 'transfers' in this.json ? Array.from(new Set(this.json['transfers'].map( (pr) => that.processobj[pr['from']]['parent_id'] ))) : [];
        this.operations = ["min","max","round","poisson","binomial","define","?","&&","||",">=","<=",">","<","==","sqrt","pow","exp","log","log2","log10","indicator","index","size","*","+","-","/"];
        this.funparnames = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
                            "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
        //
        this.model = "";
        this.write_model();
    }
    write_model() {
        this.model = "";
        this.write_header();
        this.write_functions();
        this.write_transfer();
        this.write_init();
        this.write_parnames();
        this.write_destroy();
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
        //
        this.model += "#define CHECK(x) (isnan(x) || isinf(x))\n"
        this.model += "\n";
        //
        this.numpar = this.json['parameters'].filter( (p) => !p['constant'] ).length;
        this.numpop = this.json['populations'].length;
        this.numint_int = this.json['intermediates'].length;
        this.numint_trans = this.json['transformations'].length;
        this.numint = this.numint_int + this.numint_trans;
        //
        if (this.json['model']['type'] == "Population") {
            this.numproc = 1 + Math.max(...this.json['populations'].map( (s) => s['processes'].length ));
            this.numprocpar = Math.max(...this.json['populations'].map( (spc, i) => {
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
        }
        //
        this.popart = {};
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (pop) => {
                if (!(pop['id'] in that.popart)) { that.popart[pop['id']] = {}; }
                pop['processes'].forEach( (proc) => {
                    that.popart[pop['id']][proc['arbiter']] = arbiter[proc['arbiter']];
                } );
            } );
        }
        //
        this.model += "#define NumPar " + util.format(this.numpar) + "\n";
        this.model += "#define NumPop " + util.format(this.numpop) + "\n";
        this.model += "#define NumInt " + util.format(this.numint) + "\n";
        this.model += "\n";
        //
        this.json['parameters'].filter( (p) => !p['constant'] ).forEach( (pr, i) => {
            that.model += "#define " + pr['id'] + " " + util.format(i) + "\n";
        } );
        this.model += "\n";
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc, i) => {
                spc['processes'].forEach( (prc, j) => {
                    that.model += "#define " + prc['id'] + " " + util.format(j) + "\n";
                } );
            } );
            this.model += "\n";
        }
        //
        this.json['parameters'].filter( (p) => p['constant'] ).forEach( (p) => {
            that.model += "double " + p['id'] + " = " + util.format(p['value']) + ";\n";
        } );
        this.model += "\n";
        //
        this.model += "double dmin(double a, double b) { return a < b ? a : b; }\n";
        this.model += "double dmax(double a, double b) { return a > b ? a : b; }\n";
        this.model += "\n";
    }
    write_functions() {
        let that = this;
        if ('functions' in this.json) {
           Object.entries(this.json['functions']).forEach( ([key, value]) => {
                that.model += "#define " + key + that.parse_value(value) + "\n";
           });
           this.model += "\n";
        }
    }
    write_transfer() {
        if (!('transfers' in this.json)) return;
        //
        let that = this;
        var di, pop;
        this.json['transfers'].forEach( (trn) => {
            that.model += "void fun_transfer_" + trn['id'] + "(number *key, number num, void *pop) {\n";
            that.model += "    number q[" + util.format(that.numproc) + "] = {\n";
            pop = that.json['populations'].filter( (tmp) => tmp['id'] == that.processobj[trn['from']]['parent_id'] )[0];
            pop['processes'].forEach( (proc, j) => {
                di = that.popart[pop['id']][proc['arbiter']];
                that.model += "        {." + di + "=" + that.parse_value(trn['value'][j], true) + "},\n";
            } );
            this.model += "    };\n";
            this.model += "    spop2_add(*(population *)pop, q, num);\n";
            this.model += "}\n";
            this.model += "\n";
        } );
    }
    write_init() {
        this.model += "void init(int *no, int *np, int *ni) {\n";
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
        this.model += "    *no = NumPop;\n";
        this.model += "    *np = NumPar;\n";
        this.model += "    *ni = NumInt;\n";
        this.model += "}\n";
        this.model += "\n";
    }
    write_parnames() {
        let that = this;
        this.model += "void parnames(char **names, double *param, double *parmin, double *parmax) {\n";
        this.model += "    char temp[NumPop+NumPar+NumInt][256] = {\n";
        if (this.numpop > 0) {
            this.model += "        \"" + this.json['populations'].map( (s) => s['id'] ).join("\", \"") + "\",\n";
        }
        if (this.numpar > 0) {
            this.model += "        \"" + this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => pr['id'] ).join("\", \"") + "\",\n";
        }
        if (this.numint_int > 0) {
            this.model += "        \"" + this.json['intermediates'].map( (pr) => pr['id'] ).join("\", \"") + "\",\n";
        }
        if (this.numint_trans > 0) {
            this.model += "        \"" + this.json['transformations'].map( (pr) => pr['id'] ).join("\", \"") + "\",\n";
        }
        this.model += "    };\n";
        this.model += "\n";
        this.model += "    int i;\n";
        this.model += "    for (i=0; i<(NumPop+NumPar+NumInt); i++)\n";
        this.model += "        names[i] = strdup(temp[i]);\n";
        this.model += "\n";
        this.json['parameters'].filter( (p) => !p['constant'] ).forEach( (pr, i) => {
            that.model += "    param[" + pr['id'] + "] = " + util.format('value' in pr ? pr['value'] : 0.0) + ";\n";
            that.model += "    parmin[" + pr['id'] + "] = " + util.format('min' in pr ? pr['min'] : ('value' in pr ? pr['value'] : 0.0)) + ";\n";
            that.model += "    parmax[" + pr['id'] + "] = " + util.format('max' in pr ? pr['max'] : ('value' in pr ? pr['value'] : 0.0)) + ";\n";
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
    write_out(tab, iret=true) {
        let that = this;
        if (this.deterministic) {
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    ".repeat(tab) + "ret[" + util.format(i) + "] = size_" + spc['id'] + ".d;\n" + "    ".repeat(tab) + "if (CHECK(ret[" + util.format(i) + "])) {goto endall;};\n";
            } );
        } else {
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    ".repeat(tab) + "ret[" + util.format(i) + "] = (double)(size_" + spc['id'] + ".i);\n" + "    ".repeat(tab) + "if (CHECK(ret[" + util.format(i) + "])) {goto endall;};\n";
            } );
        }
        this.model += "\n";
        this.model += "    ".repeat(tab) + "ret += " + (util.format(this.json['populations'].length)) + ";\n";
        this.model += "\n";
        if (iret) {
            if ('intermediates' in this.json) {
                this.json['intermediates'].forEach( (spc, i) => {
                    that.model += "    ".repeat(tab) + "iret[" + util.format(i) + "] = " + spc['id'] + ";\n" + "    ".repeat(tab) + "if (CHECK(iret[" + util.format(i) + "])) {goto endall;};\n";
                } );
                this.model += "\n";
            }
            if ('transformations' in this.json) {
                this.json['transformations'].forEach( (spc, i) => {
                    that.model += "    ".repeat(tab) + "iret[" + util.format(this.json['intermediates'].length + i) + "] = " + spc['id'] + ";\n" + "    ".repeat(tab) + "if (CHECK(iret[" + util.format(i) + "])) {goto endall;};\n";
                } );
                this.model += "\n";
            }
            if ('intermediates' in this.json) {
                this.model += "    ".repeat(tab) + "iret += " + (util.format(this.json['intermediates'].length + this.json['transformations'].length)) + ";\n";
            }
            this.model += "\n";
        }
    }
    write_sim() {
        let i, j;
        let that = this;
        let det = this.deterministic ? 'DETERMINISTIC' : 'STOCHASTIC';
        this.model += "void sim(int tf, int rep, double *envir, double *pr, double *y0, double *ret, double *iret, int *success) {\n";
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
            this.model += "    number num = numZERO;\n";
            this.model += "    char arbiters[" + util.format(this.numproc) + "];\n";
            this.model += "    number key[" + util.format(this.numproc) + "];\n";
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    number size_" + spc['id'] + ";\n";
            } );
            this.json['populations'].forEach( (spc, i) => {
                if (spc['processes'].length == 0) return;
                //
                that.model += "    number completed_" + spc['id'] + "[" + util.format(that.numproc) + "];\n";
            } );
            if ('transformations' in this.json) {
                if (this.deterministic) {
                    this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                        that.model += "    double " + id + " = 0.0;\n";
                    } );
                } else {
                    this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                        that.model += "    unsigned int " + id + " = 0;\n";
                    } );
                }
            }
            this.model += "    double par[" + util.format(this.numprocpar) + "];\n";
            this.model += "\n";
            this.json['populations'].forEach( (spc, i) => {
                for (j=0; j<that.numproc; j++) {
                    that.model += "    arbiters[" + util.format(j) + "] = " + (j < spc['processes'].length ? spc['processes'][j]['arbiter'] : "STOP") + ";\n";
                    that.model += "    key[" + util.format(j) + "] = numZERO;\n";
                }
                that.model += "    " + spc['id'] + " = spop2_init(arbiters, " + det + ");\n";
                that.model += "    if (y0[" + util.format(i) + "]) { num." + ("i",that.deterministic ? "d" : "i") + " = y0[" + util.format(i) + "]; spop2_add(" + spc['id'] + ", key, num); }\n";
                that.model += "\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    that.model += "    population popdone_" + spc['id'] + "[" + util.format(that.numproc) + "];\n";
                    for (j=0; j<that.numproc; j++) {
                        that.model += "    popdone_" + spc['id'] + "[" + util.format(j) + "] = spop2_init(arbiters, " + det + ");\n";
                    }
                }
            });
            this.model += "\n";
        }
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm, i) => {
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
        this.write_out(1, false);
        this.model += "    for (tm=1; tm<tf; tm++) {\n";
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm) => {
                this.model += "        " + elm['id'] + " = " + that.parse_value(elm['value']) + ";\n";
            });
            this.model += "\n";
        }
        //
        that.model += "        if (rep >= 0) {\n";
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc, i) => {
                if (spc['processes'].length == 0) return;
                //
                let pars = [];
                spc['processes'].forEach( (s) => {
                    if (Array.isArray(s['value'])) {
                        s['value'].forEach( (ss) => pars.push(ss) );
                    } else {
                        pars.push(s['value']);
                    }
                } );
                for (j = 0; j < that.numprocpar; j++) {
                    that.model += "                par[" + util.format(j) + "] = " + (pars.length ? that.parse_value(pars.shift()) : "0.0") + ";\n";
                }
                that.model += "                spop2_step(" + spc['id'] + ", par, &size_" + spc['id'] + ", completed_" + spc['id'] + ", " + (that.transfers.includes(spc['id']) ? "popdone_" + spc['id'] : "0") + ");\n";
                that.model += "\n";
            } );
            //
            /*
                TO DO!
                THERE SHOULD BE AN ORDER TO THESE PROCESSES:
                    INTERMEDIATES
                    TRANSFORMATIONS
                    TRANSFERS
            */
            if ('transformations' in this.json) {
                this.json['transformations'].forEach( (trx) => {
                    that.model += "                " + trx['id'] + " = " + that.parse_value(trx['value']) + ";\n";
                } );
                that.model += "\n";
                //
                this.json['transformations'].filter( (trx) => 'to' in trx ).forEach( (trx) => {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "                key[" + util.format(j) + "] = numZERO;\n";
                    }
                    that.model += "                num" + (that.deterministic ? ".d" : ".i") + " = " + trx['id'] + ";\n";
                    that.model += "                spop2_add(" + trx['to'] + ", key, num);\n";
                    that.model += "\n";
                } );
                //
                this.json['transformations'].filter( (trx) => 'to' in trx ).forEach( (trx) => {
                    if (that.deterministic) {
                        that.model += "                size_" + trx['to'] + ".d += " + trx['id'] + ";\n";
                    } else {
                        that.model += "                size_" + trx['to'] + ".i += " + trx['id'] + ";\n";
                    }
                } );
                that.model += "\n";
            }
            //
            if ('transfers' in this.json) {
                this.json['transfers'].forEach( (trn) => {
                    that.model += "                spop2_foreach(popdone_" + that.processobj[trn['from']]['parent_id'] + "[" + trn['from'] + "], fun_transfer_" + trn['id'] + ", (void *)(&" + trn['to'] + "));\n";
                } );
                that.model += "\n";
                this.transfers.forEach( (trn) => {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "                spop2_empty(&popdone_" + trn + "[" + util.format(j) + "]);\n"
                    }
                    that.model += "\n";
                } );
                Array.from(new Set(this.json['transfers'].map( (trn) => trn['to'] ))).forEach( (trn) => {
                    that.model += "                size_" + trn + " = spop2_size(" + trn + ");\n";
                } );
                that.model += "\n";
            }
            //
            that.model += "        }\n";
            that.model += "\n";
            //
            this.write_out(2, true);
        }
        //
        this.model += "    }\n";
        this.model += "\n";
        this.model += "  endall:\n";
        this.model += "\n";
        this.model += "    *success = tm;\n";
        this.model += "\n";
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "    spop2_free(&" + spc['id'] + ");\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "    spop2_free(&(popdone_" + spc['id'] + "[" + util.format(j) + "]));\n";
                    }
                }
            });
            this.model += "\n";
        }
        this.model += "}\n";
        this.model += "\n";
    }
    write_main() {
        this.model += "int main(int argc, char *argv[]) {\n";
        this.model += "    return 0;\n";
        this.model += "}";
        this.model += "\n";
    }
    parse_value(value, transfers=false) {
        let that = this;
        if (Array.isArray(value)) { // Function
            let fun = this.parse_value(value[0]);
            if (fun == "define") {
                if (!(value[1].every( (v) => that.funparnames.includes(v) ))) {
                    console.log(this.model + "\nError in function definition\nYou are allowed to use these as parameter names:\n" + that.funparnames + "\nERROR: " + fun + " : " + value);
                    process.exit(1);
                }
                let def = this.parse_value(value[2]);
                return "(" + value[1].join(",") + ") (" + def + ")";
            } else {
                let prm = value.slice(1).map( (v) => that.parse_value(v) );
                if (this.processes.includes(fun)) {
                    if (transfers) {
                        return "key[" + fun + "]." + this.popart[prm[0]][this.processobj[fun]['arbiter']];
                    }
                    return "completed_" + prm[0] + "[" + fun + "]" + (this.deterministic ? ".d" : ".i");
                } else if (fun == "size") {
                    return "size_" + prm[0] + (this.deterministic ? ".d" : ".i");
                } else if (fun == "index") {
                    return prm[0] + "[" + prm[1] + "]";
                } else if (fun == "round") {
                    return fun + "(" + prm.join(", ") + ")";
                } else if (fun == "min") {
                    return "dmin(" + prm.join(", ") + ")";
                } else if (fun == "max") {
                    return "dmax(" + prm.join(", ") + ")";
                } else if (this.functions.includes(fun) || fun == "exp" || fun == "log" || fun == "log2" || fun == "log10" || fun == "pow" || fun == "sqrt") {
                    return fun + "(" + prm.join(", ") + ")";
                } else if (fun == "binomial") {
                    return "(unsigned int)gsl_ran_binomial(RANDOM, " + prm[0] + ", " + prm[1] + ")";
                } else if (fun == "poisson") {
                    return "(unsigned int)gsl_ran_poisson(RANDOM, " + prm[0] + ")";
                } else if (fun == "*") {
                    return "(" + prm.join(" * ") + ")";
                } else if (fun == "+") {
                    return "(" + prm.join(" + ") + ")";
                } else if (fun == "-") {
                    return "(" + prm.join(" - ") + ")";
                } else if (fun == "/") {
                    return "(" + prm.join(" / ") + ")";
                } else if (fun == "?") {
                    return "(" + prm[0] + ") ? (" + prm[1] + ") : (" + prm[2] + ")";
                } else if (fun == "&&") {
                    return "(" + prm.join(" && ") + ")";
                } else if (fun == "||") {
                    return "(" + prm.join(" || ") + ")";
                } else if (fun == "indicator") {
                    return this.deterministic ? "(" + prm[0] + " ? 1.0 : 0.0)" : "(" + prm[0] + " ? 1 : 0)";
                } else if (fun == "==") {
                    return "(" + prm[0] + " == " + prm[1] + ")";
                } else if (fun == ">") {
                    return "(" + prm[0] + " > " + prm[1] + ")";
                } else if (fun == "<") {
                    return "(" + prm[0] + " < " + prm[1] + ")";
                } else if (fun == ">=") {
                    return "(" + prm[0] + " >= " + prm[1] + ")";
                } else if (fun == "<=") {
                    return "(" + prm[0] + " <= " + prm[1] + ")";
                } else {
                    console.log(this.model + "\nUnknown keyword in equation\nERROR: " + fun + " : " + value);
                    process.exit(1);
                }
            }
        } else { // Parameter
            if (typeof value === 'string' || value instanceof String) { // String
                if (this.environs.includes(value)) {
                    return value;
                } else if (this.populations.includes(value)) {
                    return value;
                } else if (this.processes.includes(value)) {
                    return value;
                } else if (this.parametersv.includes(value)) {
                    return "pr[" + value + "]";
                } else if (this.parametersc.includes(value)) {
                    return value;
                } else if (this.functions.includes(value)) {
                    return value;
                } else if (this.intermediates.includes(value)) {
                    return value;
                } else if (this.transformations.includes(value)) {
                    return value;
                } else if (this.operations.includes(value)) {
                    return value;
                } else if (this.funparnames.includes(value)) {
                    return "(" + value + ")";
                } else if (value == "tm") {
                    return value;
                } else if (!isNaN(parseFloat(value))) {
                    return value;
                } else if (!isNaN(parseInt(value))) {
                    return value;
                } else {
                    console.log(this.model + "\nUnknown reference to a user defined keyword\nERROR: " + value);
                    process.exit(1);
                }
            } else { // Number
                console.log(this.model + "\nERROR: Numbers should be provided as strings");
                console.log([fun,value])
                process.exit(1);
        }
        }
    }
}

exports.parse = PopJSON;
