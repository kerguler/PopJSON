import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex6a...")
ex6a = pop.model("examples/ex6a.dylib")
#
C = 3
#
dfun = lambda p0,p1: abs(p1[0]-p0[0])+abs(p1[1]-p0[1])
pts = [(x,y) for x in range(C) for y in range(C)]
tprobs = numpy.array([[0.1 if dfun(p0,p1)==1 else 0.0 for p1 in pts] for p0 in pts])
numpy.fill_diagonal(tprobs,1.0-numpy.sum(tprobs,axis=1))
tprobs = tprobs.reshape(C**4)
#
print(["| %s |\n" %(" | ".join([str(a) for a in row])) for row in tprobs.reshape((C**2,C**2))])
#
out6a = ex6a.sim(1000,envir={"tprob":tprobs},pr=ex6a.param,y0={"adult_0":100.0})
for i in [0,5,10,100]:
    plt.imshow(out6a['ret'][0,i,:].reshape((C,C)),interpolation='none',vmin=0,vmax=100)
    plt.xticks(numpy.arange(C))
    plt.yticks(numpy.arange(C))
    plt.gca().xaxis.set_tick_params(size=0,labeltop=True,labelbottom=False)
    plt.gca().yaxis.set_tick_params(size=0)
    plt.title("Step %d" %i,fontsize=18)
    plt.savefig("figures/ex6a%d.png" %i,bbox_inches="tight",dpi=100)
    plt.close()
