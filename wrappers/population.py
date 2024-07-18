import atexit
import numpy
from ctypes import *
import numpy.ctypeslib as npct
array_1d_double = npct.ndpointer(dtype=numpy.float64, ndim=1, flags='CONTIGUOUS')
array_1d_int = npct.ndpointer(dtype=numpy.int32, ndim=1, flags='CONTIGUOUS')

# https://stackoverflow.com/questions/50964033/forcing-ctypes-cdll-loadlibrary-to-reload-library-from-file
dlclose_func = CDLL(None).dlclose
dlclose_func.argtypes = (c_void_p,)
dlclose_func.restype = c_int

def calcEnsemble(sim):
    if len(sim)==0:
        return None
    p = numpy.percentile(sim,[2.5,50,97.5],axis=0)
    return {
        'lower':p[0],
        'median':p[1],
        'higher':p[2],
        'mean':numpy.mean(sim,axis=0),
        'std':numpy.std(sim,axis=0)
        }

class model:
    def __init__(self, filename):
        self.filename = filename
        self.dylib = cdll.LoadLibrary(self.filename)
        #
        self.init = self.dylib.init
        self.init.restype = None
        self.init.argtypes = [array_1d_int, 
                              array_1d_int, 
                              array_1d_int, 
                              array_1d_int, 
                              array_1d_int]
        self.numpop = numpy.arange(1,dtype=numpy.int32)
        self.numpar = numpy.arange(1,dtype=numpy.int32)
        self.numint = numpy.arange(1,dtype=numpy.int32)
        self.numenv = numpy.arange(1,dtype=numpy.int32)
        self.stoch = numpy.arange(1,dtype=numpy.int32)
        ret = self.init(self.numpop,
                        self.numpar,
                        self.numint,
                        self.numenv,
                        self.stoch)
        self.numpop = self.numpop[0]
        self.numpar = self.numpar[0]
        self.numint = self.numint[0]
        self.numenv = self.numenv[0]
        self.stoch = self.stoch[0]
        #
        atexit.register(self.destroy)
        #
        try:
            self.get_names = self.dylib.parnames
            self.get_names.restype = None
            self.get_names.argtypes = [POINTER(c_char_p), 
                                       array_1d_double,
                                       array_1d_double,
                                       array_1d_double]
            temp = (c_char_p * (self.numpop+self.numpar+self.numint+self.numenv))(256)
            param = numpy.ndarray(self.numpar, dtype=numpy.float64)
            parmin = numpy.ndarray(self.numpar, dtype=numpy.float64)
            parmax = numpy.ndarray(self.numpar, dtype=numpy.float64)
            ret = self.get_names(temp, param, parmin, parmax)
            temp = numpy.array([str(elm,'utf-8') for elm in temp])
            self.popnames = numpy.copy(temp[:self.numpop])
            self.parnames = numpy.copy(temp[self.numpop:(self.numpop+self.numpar)])
            self.intnames = numpy.copy(temp[(self.numpop+self.numpar):(self.numpop+self.numpar+self.numint)])
            self.envnames = numpy.copy(temp[(self.numpop+self.numpar+self.numint):])
            self.param = numpy.copy(param)
            self.parmin = numpy.copy(parmin)
            self.parmax = numpy.copy(parmax)
        except:
            print("Falling back to default parameters")
            self.popnames = numpy.array(["coln%d" %(n) for n in range(self.numpop)])
            self.parnames = numpy.array(["par%d" %(n) for n in range(self.numpar)])
            self.intnames = numpy.array(["inter%d" %(n) for n in range(self.numint)])
            self.envnames = numpy.array(["envir%d" %(n) for n in range(self.numenv)])
            self.param = numpy.repeat(0.0, self.numpar)
            self.parmin = numpy.repeat(0.0, self.numpar)
            self.parmax = numpy.repeat(0.0, self.numpar)
        #
        self.popids = {}
        for elm in self.popnames:
            self.popids[elm] = numpy.where(elm==self.popnames)[0][0]
        #
        self.parids = {}
        for elm in self.parnames:
            self.parids[elm] = numpy.where(elm==self.parnames)[0][0]
        #
        self.intids = {}
        for elm in self.intnames:
            self.intids[elm] = numpy.where(elm==self.intnames)[0][0]
        #
        self.envids = {}
        for elm in self.envnames:
            self.envids[elm] = numpy.where(elm==self.envnames)[0][0]
        #
        self.csim = self.dylib.sim
        self.csim.restype = None
        self.csim.argtypes = [c_int,
                              c_int,
                              array_1d_double,
                              array_1d_double,
                              array_1d_double,
                              c_char_p,
                              c_char_p,
                              array_1d_double,
                              array_1d_double,
                              array_1d_int]
        #
    def destroy(self):
        self.dylib.destroy
        dlclose_func(self.dylib._handle)
        #
    def _sim(self,ftime,envir,pr,y0={},rep=1,file0="",file1=""):
        """
            Note: Final time point is ftime - 1
        """
        ftime = numpy.int32(ftime)
        if self.numenv > 0:
            envir = numpy.hstack([numpy.insert(numpy.array(envir[key],dtype=numpy.float64),0,len(envir[key])) for key in self.envnames])
        else:
            envir = numpy.array([])
        y0 = numpy.hstack([numpy.array(y0[key] if key in y0 else 0.0,dtype=numpy.float64) for key in self.popnames])
        pr = numpy.array(pr)
        file0 = bytes(file0,'utf-8') if file0 else bytes(" ",'utf-8')
        file1 = bytes(file1,'utf-8') if file1 else bytes(" ",'utf-8')
        rep = numpy.int32(rep)
        rdim = rep if rep >= 0 else -rep
        retrow = ftime*self.numpop
        iretrow = (ftime-1)*self.numint
        ret = numpy.ndarray(rdim*retrow, dtype=numpy.float64)
        iret = numpy.ndarray(rdim*iretrow, dtype=numpy.float64)
        success = numpy.array([0 for r in range(rdim)], dtype=numpy.int32, ndmin=1)
        for r in range(rdim):
            self.csim(ftime,
                      rep,
                      envir,
                      pr,
                      y0,
                      file0,
                      file1,
                      ret[(r*retrow):((r+1)*retrow)],
                      iret[(r*iretrow):((r+1)*iretrow)],
                      success[r:(r+1)])
        ret = numpy.array(ret).reshape((rdim,ftime,self.numpop))
        iret = numpy.array(iret).reshape((rdim,ftime-1,self.numint))
        return { 
            "success": success[0], 
            "ret": ret, 
            "iret": iret 
        }
        #
    def sim(self,ftime,envir={},pr=[],prs=[],y0={},rep=1,file0="",file1="",boil=False):
        rets = []
        irets = []
        #
        if len(pr) > 0 and len(prs) == 0:
            prs = [pr]
        elif len(pr) > 0 and len(prs) > 0:
            print("Please supply 'pr' for a single parameter set or 'prs' for a list of parameter sets.")
            return { 
                "success": 0,
                "ret": [],
                "iret": []
            }
        elif len(pr) == 0 and len(prs) == 0:
            prs = [[]]
        #
        for pr in prs:
            sim = self._sim(ftime,envir,pr,y0,rep=rep,file0=file0,file1=file1)
            if sim['success'] == ftime:
                if len(rets) == 0:
                    rets = sim['ret']
                else:
                    rets = numpy.vstack([rets,sim['ret']])
                if len(irets) == 0:
                    irets = sim['iret']
                else:
                    irets = numpy.vstack([irets,sim['iret']])
        return { 
            "success": len(rets),
            "ret": calcEnsemble(rets) if boil else rets,
            "iret": calcEnsemble(irets) if boil else irets
        }
        
"""
TEST
"""
if __name__ == '__main__':
    pass