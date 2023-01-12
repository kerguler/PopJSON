import json

class parser:
    def __init__(self, filename) -> None:
        self.filename = filename
        self.json = json.load(open(self.filename, "r"))
        self.deterministic = self.json['modelTypes'][self.json['model']['type']]['deterministic']
        #
    def write_model(self):
        self.write_header()
        self.write_init()
        self.write_parnames()
        self.write_destroy()
        self.write_sim()
        #
    def write_header(self):
        if self.json['model']['type'] == "Population":
            print("#include <math.h>")
            print("#include \"population.h\"")
            print()
            if not self.deterministic:
                print("extern gsl_rng *RANDOM;")
                print()
        elif self.json['model']['type'] == "ODE":
            print("#include <stdlib.h>")
            print("#include \"lsoda.h\"")
            print()
        print("#define NumPar %d" %(len(self.json['parameters'].keys())))
        print("#define NumMet %d" %(len(self.json['stages'])))
        print()
        i = 0
        for i in range(len(self.json['stages'])):
            print("#define STAGE_%s %d" %(self.json['stages'][i]['id'],i))
            i += 1
        print()
        i = 0
        for param, prop in self.json['parameters'].items():
            print("#define PARAM_%s %d" %(param,i))
            i += 1
        print()
        i = 0
        for trx, prop in self.json['transformations'].items():
            print("#define TRANS_%s %d" %(trx,i))
            i += 1
        print()
        #
    def write_init(self):
        print("void init(int *np, int *nm) {")
        if self.json['model']['type'] == "Population":
            if not self.deterministic:
                print("    spop2_random_init();")
                print()
            if 'istep' in self.json['modelTypes'][self.json['model']['type']]['parameters']:
                print("    spop2_set_eps(%g);" %(self.json['modelTypes'][self.json['model']['type']]['parameters']['istep']))
                print()
        print("    *np = NumPar;")
        print("    *nm = NumMet;")
        print("}")
        print()
        #
    def write_parnames(self):
        print("void parnames(char **names) {")
        print("    char temp[NumMet+NumPar][256] = {")
        print("        \"%s\"," %("\", \"".join([s['id'] for s in self.json['stages']])))
        print("        \"%s\"" %("\", \"".join(self.json['parameters'].keys())))
        print("    };")
        print("    int i;")
        print("    for (i=0; i<(NumMet+NumPar); i++)")
        print("        names[i] = strdup(temp[i]);")
        print("}")
        print()
        #
    def write_destroy(self):
        print("void destroy(void) {")
        if self.json['model']['type'] == "Population" and not self.deterministic:
            print("    spop2_random_destroy();")
        print("}")
        print()
        #
    def write_sim(self):
        print("void sim(int tf, double *envir, double *pr, double *y0, double *ret) {")
        if 'environ' in self.json:
            for i in range(len(self.json['environ'])):
                print("    double *%s = envir + %d * tf;" %(self.json['environ'][i]['id'], i))
            print()
        if self.json['model']['type'] == "Population":
            for spc in self.json['stages']:
                print("    population %s;" %(spc['id']))
            print()
            print("    number num = numZERO;")
            det = ['STOCHASTIC','DETERMINISTIC'][self.deterministic]
            for i in range(len(self.json['stages'])):
                spc = self.json['stages'][i]
                print("    {")
                print("        char arbiters[%d] = {%s};" %(len(spc['processes'])+1,", ".join([s['arbiter'] for s in spc['processes']]+["STOP"])))
                print("        %s = spop2_init(arbiters, %s);" %(spc['id'],det))
                print("        number key[%d] = {%s};" %(len(spc['processes']),", ".join(["numZERO" for s in spc['processes']])))
                print("        if (y0[%d]) { num.%s = y0[%d]; spop2_add(%s, key, num); }" %(i,["i","d"][self.deterministic],i,spc['id']))
                print("    }")
            print()
        print("    int tm = 0;")
        print("    for (tm=1; tm<tf; tm++) {")
        print("    }")
        print()
        if self.json['model']['type'] == "Population":
            for spc in self.json['stages']:
                print("    spop2_free(&%s);" %(spc['id']))
            print()
        print("}")
        print()

if __name__ == '__main__':
    import sys
    parser(sys.argv[1]).write_model()
