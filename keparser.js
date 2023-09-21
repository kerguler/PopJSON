'use strict';

const fs = require('fs');
// const { emitWarning } = require('process');
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

const stepper = {
    'NO_STEPPER': '0',
    'AGE_STEPPER': 'age_stepper',
    'ACC_STEPPER': 'acc_stepper'
};

const hazard = {
    'AGE_FIXED': ['age_fixed_pars', 'age_hazard_calc', 'age_fixed_haz'],
    'AGE_CONST': ['age_const_pars', 'age_const_calc', 'age_const_haz'],
    'AGE_GAMMA': ['age_gamma_pars','age_hazard_calc','age_gamma_haz'],
    'AGE_NBINOM': ['age_nbinom_pars', 'age_hazard_calc', 'age_nbinom_haz'],
    'NOAGE_CONST': ['age_const_pars', 'age_const_calc', 'age_const_haz']
};

class PopJSON {
    constructor() {
        this.model = "";
        this.error = "";
        this.json = {};
    }
    check_ids(id) {
        if (!/^[a-zA-Z][a-zA-Z0-9\_]+$/.test(id))
            this.error += "Only numeric and alphanumeric characters and \"_\" are allowed in IDs. Also, IDs should not start with a number.\n";
        return id;
    }
    results() {
        return({
            "model": this.model,
            "error": this.error
        });
    }
    parse_file(filename) {
        this.filename = filename;
        let data = fs.readFileSync(this.filename);
        if (!data) {
            this.error += "File not found!\n";
            return(this.results());
        }
        try {
            this.json = JSON.parse(data);
        } catch (e) {
            this.error += "JSON parse error!\n";
            return(this.results());
        }
        //
        this.parse();
        return(this.results());
    }
    parse_json(text) {
        try {
            this.json = JSON.parse(text);
        } catch (e) {
            this.error += "JSON parse error!\n";
            return(this.results());
        }
        //
        this.parse();
        return(this.results());
    }
    parse() {
        let that = this;
        this.deterministic = this.json['model']['deterministic'];
        if (!('environ' in this.json)) this.json['environ'] = [];
        this.environs = this.json['environ'].map( (pr) => that.check_ids(pr['id']) );
        if (!('populations' in this.json)) {
            this.json['populations'] = [];
            this.error += "Couldn't find any populations!\n";
            return(this.results());
        }
        this.populations = this.json['populations'].map( (pr) => that.check_ids(pr['id']) );
        this.processes = []; this.json['populations'].forEach( (pop) => 'processes' in pop ? pop['processes'].forEach( (pr) => { that.processes.push(that.check_ids(pr['id'])); } ) : [] );
        this.processobj = {}; this.json['populations'].forEach( (pop) => 'processes' in pop ? pop['processes'].forEach( (pr) => { that.processobj[that.check_ids(pr['id'])] = pr; that.processobj[that.check_ids(pr['id'])]['parent_id'] = that.check_ids(pop['id']); } ) : {} );
        if (!('parameters' in this.json)) this.json['parameters'] = [];
        this.parametersv = this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => that.check_ids(pr['id']) );
        this.parametersc = this.json['parameters'].filter( (p) => p['constant'] ).map( (pr) => that.check_ids(pr['id']) );
        if (!('functions' in this.json)) this.json['functions'] = [];
        this.functions = Object.keys(this.json['functions']);
        if (!('intermediates' in this.json)) this.json['intermediates'] = [];
        this.intermediates = this.json['intermediates'].map( (pr) => that.check_ids(pr['id']) );
        if (!('transformations' in this.json)) this.json['transformations'] = [];
        this.transformations = this.json['transformations'].map( (pr) => that.check_ids(pr['id']) );
        if (!('transfers' in this.json)) this.json['transfers'] = [];
        this.transfers = Array.from(new Set(this.json['transfers'].map( (pr) => that.processobj[pr['from']]['parent_id'] )));
        this.operations = ["min","max","round","poisson","binomial","define","?","&&","||",">=","<=",">","<","==","sqrt","pow","exp","log","log2","log10","indicator","index","size","*","+","-","/"];
        this.funparnames = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
                            "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
        //
        this.model = "";
        if (!this.error)
            this.write_model();
    }
    write_model() {
        this.model = "";
        this.write_header();
        this.write_functions();
        this.write_transfer();
        this.write_custom();
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
            if (this.json['populations'].length == 0) {
                this.numproc = 0;
                this.numprocpar = 0;
            } else {
                this.numproc = 1 + Math.max(...this.json['populations'].map( (s) => 'processes' in s ? s['processes'].length : 0 ));
                this.numprocpar = Math.max(...this.json['populations'].map( (spc, i) => {
                    let pars = 0;
                    if (!('processes' in spc)) return pars;
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
        //
        this.model += "double *model_param;\n";
        if ('environ' in this.json) {
            this.json['environ'].forEach( (elm, i) => {
                 that.model += "double *envir_" + elm['id'] + ";\n";
            });
            this.model += "\n";
        }
        //
        if ('transformations' in this.json) {
            if (this.deterministic) {
                this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                    that.model += "double " + id + ";\n";
                } );
            } else {
                this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                    that.model += "unsigned int " + id + ";\n";
                } );
            }
        }
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm, i) => {
                that.model += "double " + elm['id'] + ";\n";
            });
            this.model += "\n";
        }
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
            that.model += "    };\n";
            that.model += "    spop2_add(*(population *)pop, q, num);\n";
            that.model += "}\n";
            that.model += "\n";
        } );
    }
    write_custom() {
        let that = this;
        this.json['populations'].forEach( (pop) => {
            if (!('processes' in pop) || (pop['processes'].length == 0)) return;
            pop['processes'].forEach( (proc) => {
                if (proc['arbiter'] == "AGE_CUSTOM") {
                    if (!('hazard' in proc)) return;
                    that.model += "double fun_custom_" + proc['id'] + "_" + pop['id'] + "(hazard hfun, unsigned int d, number q, number k, double theta, const number *key) {\n";
                    that.model += "    double devmn = " + that.parse_value(proc['hazard'][1], true) + ";\n";
                    that.model += "    double devsd = " + that.parse_value(proc['hazard'][2], true) + ";\n";
                    that.model += "    hazpar hz = " + hazard[proc['hazard'][0]][0] + "(devmn, devsd);\n";
                    that.model += "    double a = " + hazard[proc['hazard'][0]][1] + "(" + hazard[proc['hazard'][0]][2] + ", 0, key[" + proc['id'] + "], hz.k, hz.theta, key);\n";
                    that.model += "    return a;\n";
                    that.model += "}\n";
                    that.model += "\n";
                }
            } );
        } );        
    }
    write_init() {
        this.model += "void init(int *no, int *np, int *ni) {\n";
        if (this.json['model']['type'] == "Population") {
            if (!this.deterministic) {
                this.model += "    spop2_random_init();\n";
                this.model += "\n";
            }
            if ('istep' in this.json['model']['parameters']) {
                this.model += "    spop2_set_eps(" + util.format(this.json['model']['parameters']['istep']) + ");\n";
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
            that.model += "    param[" + pr['id'] + "] = " + util.format('value' in pr ? this.parse_value(pr['value']) : 0.0) + ";\n";
            that.model += "    parmin[" + pr['id'] + "] = " + util.format('min' in pr ? this.parse_value(pr['min']) : ('value' in pr ? this.parse_value(pr['value']) : 0.0)) + ";\n";
            that.model += "    parmax[" + pr['id'] + "] = " + util.format('max' in pr ? this.parse_value(pr['max']) : ('value' in pr ? this.parse_value(pr['value']) : 0.0)) + ";\n";
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
        this.model += "void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {\n";
        this.model += "\n";
        this.model += "    int TIME = 0;\n";
        this.model += "\n";
        this.model += "    model_param = pr;\n";
        if ('environ' in this.json) {
           this.json['environ'].forEach( (elm, i) => {
                that.model += "    envir_" + elm['id'] + " = envir + " + util.format(i) + " * tf;\n";
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
            this.json['populations'].forEach( (spc, i) => {
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    that.model += "    population popdone_" + spc['id'] + "[" + util.format(that.numproc) + "];\n";
                }
            } );
            if ('transformations' in this.json) {
                if (this.deterministic) {
                    this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                        that.model += "    " + id + " = 0.0;\n";
                    } );
                } else {
                    this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                        that.model += "    " + id + " = 0;\n";
                    } );
                }
            }
            this.model += "    double par[" + util.format(this.numprocpar) + "];\n";
            this.model += "\n";
            //
            this.model += "    FILE *file;\n";
            this.model += "    number *buff = 0;\n";
            this.model += "    unsigned int buffsz = 0;\n";
            this.model += "    if (file0 && file0[0]!=' ') {\n";
            this.model += "        file = fopen(file0,\"rb\");\n";
            this.model += "        if (!file) {\n";
            this.model += "            *success = 0;\n";
            this.model += "            goto endall;\n";
            this.model += "        }\n";
            this.model += "        rewind(file);\n";
            this.model += "    }\n";
            this.model += "\n";
            this.model += "    if (file0 && file0[0]!=' ') {\n";
            this.json['populations'].forEach( (spc, i) => {
                that.model += "        fread(&buffsz, sizeof(unsigned int), 1, file);\n";
                that.model += "        buff = (number *)malloc(buffsz);\n";
                that.model += "        fread(buff, buffsz, 1, file);\n";
                that.model += "        " + spc['id'] + " = spop2_loadstate(buff);\n";
                that.model += "\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "        popdone_" + spc['id'] + "[" + util.format(j) + "] = spop2_loadstate_empty(buff);\n";
                    }
                }
                that.model += "\n";
                that.model += "        free(buff);\n";
                that.model += "\n";
            } );
            this.model += "    } else {\n";
            this.json['populations'].forEach( (spc, i) => {
                for (j=0; j<that.numproc; j++) {
                    that.model += "        arbiters[" + util.format(j) + "] = " + (j < spc['processes'].length ? spc['processes'][j]['arbiter'] : "STOP") + ";\n";
                    that.model += "        key[" + util.format(j) + "] = numZERO;\n";
                }
                that.model += "        " + spc['id'] + " = spop2_init(arbiters, " + det + ");\n";
                for (j=0; j<that.numproc; j++) {
                    if ((j < spc['processes'].length) && ('stepper' in spc['processes'][j])) {
                        if (!(spc['processes'][j]['stepper'] in stepper)) {
                            that.error += "Stepper not defined. Please choose one of these: " + Object.keys(stepper).join(", ") + "\n";
                            break;
                        }
                        that.model += "        " + spc['id'] + "->arbiters[" + util.format(j) + "]->fun_step = " + stepper[spc['processes'][j]['stepper']] + ";\n";
                    }
                    if ((j < spc['processes'].length) && ('hazard' in spc['processes'][j])) {
                        if (!(spc['processes'][j]['hazard'][0] in hazard)) {
                            that.error += "Hazard not defined. Please choose one of these: " + Object.keys(hazard).join(", ") + "\n";
                            break;
                        }
                        that.model += "        " + spc['id'] + "->arbiters[" + util.format(j) + "]->fun_calc = fun_custom_" + spc['processes'][j]['id'] + "_" + spc['id'] + ";\n";
                    }
                }
                that.model += "        if (y0[" + util.format(i) + "]) { num." + ("i",that.deterministic ? "d" : "i") + " = y0[" + util.format(i) + "]; spop2_add(" + spc['id'] + ", key, num); }\n";
                that.model += "\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "        popdone_" + spc['id'] + "[" + util.format(j) + "] = spop2_init(arbiters, " + det + ");\n";
                    }
                }
            });
            that.model += "    }\n";
            this.model += "\n";
            this.model += "    if (file0 && file0[0]!=' ') {\n";
            this.model += "        fclose(file);\n";
            this.model += "    }\n";
        }
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm, i) => {
                that.model += "    " + elm['id'] + " = 0.0;\n";
            });
            this.model += "\n";
        }
        //
        this.json['populations'].forEach( (spc, i) => {
            that.model += "    size_" + spc['id'] + " = spop2_size(" + spc['id'] + ");\n";
        } );
        this.model += "\n";
        this.write_out(1, false);
        this.model += "    for (TIME=1; TIME<tf; TIME++) {\n";
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm) => {
                that.model += "        " + elm['id'] + " = " + that.parse_value(elm['value']) + ";\n";
            });
            this.model += "\n";
        }
        //
        this.model += "        if (rep >= 0) {\n";
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
        this.model += "    *success = TIME;\n";
        this.model += "\n";
        this.model += "    if (file1 && file1[0]!=' ') {\n";
        this.model += "        file = fopen(file1,\"wb\");\n";
        this.model += "        if (!file) {\n";
        this.model += "            *success = 0;\n";
        this.model += "        } else {\n";
        this.model += "            rewind(file);\n";
        this.model += "\n";
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "            buffsz = spop2_buffsize(" + spc['id'] + ");\n";
                that.model += "            buff = spop2_savestate(" + spc['id'] + ");\n";
                that.model += "            fwrite(&buffsz, sizeof(unsigned int), 1, file);\n";
                that.model += "            fwrite(buff, buffsz, 1, file);\n";
                that.model += "            free(buff);\n";
                that.model += "\n";
            });
        }
        this.model += "            fclose(file);\n";
        this.model += "        }\n";
        this.model += "    }\n";
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
            let fun = this.parse_value(value[0], transfers=transfers);
            if (fun == "define") {
                if (!(value[1].every( (v) => that.funparnames.includes(v) ))) {
                    this.error += "Error in function definition\nYou are allowed to use these as parameter names:\n" + that.funparnames + "\nERROR: " + fun + " : " + value + "\n";
                    return "";
                }
                let def = this.parse_value(value[2], transfers=transfers);
                return "(" + value[1].join(",") + ") (" + def + ")";
            } else {
                let prm = value.slice(1).map( (v) => that.parse_value(v, transfers=transfers) );
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
                    this.error += "Unknown keyword in equation\nERROR: " + fun + " : " + value + "\n";
                    return "";
                }
            }
        } else { // Parameter
            if (typeof value === 'string' || value instanceof String) { // String
                if (this.environs.includes(value)) {
                    return "envir_"+value;
                } else if (this.populations.includes(value)) {
                    return value;
                } else if (this.processes.includes(value)) {
                    return value;
                } else if (this.parametersv.includes(value)) {
                    return "model_param[" + value + "]";
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
                } else if (value == "TIME") {
                    return "TIME";
                } else if (value == "TIME_1") {
                    return "(TIME-1)";
                } else if (!isNaN(parseFloat(value))) {
                    return value;
                } else if (!isNaN(parseInt(value))) {
                    return value;
                } else {
                    this.error += "Unknown reference to a user defined keyword\nERROR: " + value + "\n";
                    return "";
                }
            } else if (typeof value === 'number') { // Number
                return value.toString();
            } else { // NaN
                this.error += "ERROR: Invalid value encountered" + fun + " : " + value + "\n";
                return "";
        }
        }
    }
}

exports.PopJSON = PopJSON;
