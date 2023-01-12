import numpy
import population as po

m = po.model("./test.dylib")

envir = numpy.repeat(30.0, 300)
y0 = numpy.repeat(0.0, m.nummet); y0[0] = 100.0
pr = numpy.repeat(1.0, m.numpar)

s = m.sim(100, envir, pr, y0, rep=1)
