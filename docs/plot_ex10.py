import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex10...")
ex = pop.model("examples/ex10.dylib")

N = 50

k = min(numpy.random.poisson(5), N)
indices = numpy.random.choice(N, size=k, replace=True)
values = numpy.abs(numpy.random.normal(0, 1, size=k))

prec = numpy.repeat(0.0, N)
prec[indices] = values
prec[0] = 1.0

out = ex.sim(N,
             y0={
                "larva": 100.0
             },
             envir={
                "prec": prec
             },
             pr=ex.param)

width = 1.0
x = numpy.arange(len(prec))

fig, ax1 = plt.subplots(figsize=(10, 5))
ax1.bar(x + width/2, prec, alpha=0.5, width=width, label="Precipitation (mm)", color="#4C72B0")
ax1.plot(out['iret'][0,:,ex.intids['bsvol']],'o-',label="bsvol")
ax1.plot(out['iret'][0,:,ex.intids['bsmort']],'o-',label="bsmort")
ax1.axhline(y=0.75)
ax1.legend()
ax2 = ax1.twinx()
ax2.plot(out['ret'][0,:,ex.popids['larva']],label="Larva")
ax2.legend()
plt.show()
