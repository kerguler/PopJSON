import atexit
import numpy
from ctypes import *
import numpy.ctypeslib as npct
array_1d_double = npct.ndpointer(dtype=numpy.float64, ndim=1, flags='CONTIGUOUS')
array_1d_int = npct.ndpointer(dtype=numpy.int32, ndim=1, flags='CONTIGUOUS')

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
        self.init.argtypes = [array_1d_int, array_1d_int, array_1d_int]
        self.numpop = numpy.arange(1,dtype=numpy.int32)
        self.numpar = numpy.arange(1,dtype=numpy.int32)
        self.numint = numpy.arange(1,dtype=numpy.int32)
        ret = self.init(self.numpop,self.numpar,self.numint)
        self.numpop = self.numpop[0]
        self.numpar = self.numpar[0]
        self.numint = self.numint[0]
        #
        atexit.register(self.dylib.destroy)
        #
        try:
            self.get_names = self.dylib.parnames
            self.get_names.restype = None
            self.get_names.argtypes = [POINTER(c_char_p), 
                                       array_1d_double,
                                       array_1d_double,
                                       array_1d_double]
            temp = (c_char_p * (self.numpop+self.numpar+self.numint))(256)
            param = numpy.ndarray(self.numpar, dtype=numpy.float64)
            parmin = numpy.ndarray(self.numpar, dtype=numpy.float64)
            parmax = numpy.ndarray(self.numpar, dtype=numpy.float64)
            ret = self.get_names(temp, param, parmin, parmax)
            temp = numpy.array([str(elm,'utf-8') for elm in temp])
            self.popnames = numpy.copy(temp[:self.numpop])
            self.parnames = numpy.copy(temp[self.numpop:(self.numpop+self.numpar)])
            self.intnames = numpy.copy(temp[(self.numpop+self.numpar):])
            self.param = numpy.copy(param)
            self.parmin = numpy.copy(parmin)
            self.parmax = numpy.copy(parmax)
        except:
            print("Falling back to default parameters")
            self.popnames = numpy.array(["coln%d" %(n) for n in range(self.numpop)])
            self.parnames = numpy.array(["par%d" %(n) for n in range(self.numpar)])
            self.intnames = numpy.array(["inter%d" %(n) for n in range(self.numint)])
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
        self.csim = self.dylib.sim
        self.csim.restype = None
        self.csim.argtypes = [c_int,
                              c_int,
                              array_1d_double,
                              array_1d_double,
                              array_1d_double,
                              POINTER(c_char_p),
                              POINTER(c_char_p),
                              array_1d_double,
                              array_1d_double,
                              array_1d_int]
        #
    def sim(self,ftime,envir,pr,y0,rep=1,file0="",file1=""):
        """
            Note: Final time point is ftime - 1
        """
        ftime = numpy.int32(ftime)
        envir = numpy.array(envir)
        pr = numpy.array(pr)
        y0 = numpy.array(y0)
        file0 = c_char_p(bytes(file0,'utf-8'))
        file1 = c_char_p(bytes(file1,'utf-8'))
        rep = numpy.int32(rep)
        rdim = rep if rep >= 0 else -rep
        ret = numpy.ndarray(rdim*ftime*self.numpop, dtype=numpy.float64)
        iret = numpy.ndarray(rdim*(ftime-1)*self.numint, dtype=numpy.float64)
        success = numpy.array(0, dtype=numpy.int32, ndmin=1)
        self.csim(ftime,
                  rep,
                  envir,
                  pr,
                  y0,
                  file0,
                  file1,
                  ret,
                  iret,
                  success)
        ret = numpy.array(ret).reshape((rdim,ftime,self.numpop))
        iret = numpy.array(iret).reshape((rdim,ftime-1,self.numint))
        return { 
            "success": success[0], 
            "ret": ret, 
            "iret": iret 
        }
        #
    def sims(self,ftime,envir,prs,y0,rep=1,file0="",file1=""):
        rets = []
        irets = []
        for pr in prs:
            sim = self.sim(ftime,envir,pr,y0,rep=rep,file0=file0,file1=file1)
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
            "rets": calcEnsemble(rets),
            "irets": calcEnsemble(irets)
        }
        
"""
TEST
"""
if __name__ == '__main__':
    pass