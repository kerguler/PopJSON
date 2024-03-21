import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex1E...")
ex1E = pop.model("examples/ex1E.dylib")
t2m = numpy.arange(0,40,0.01)+273.15
out1Ef = ex1E.sim(len(t2m), envir={"temp":t2m}, pr=ex1E.param)
plt.plot(t2m[1:],out1Ef['iret'][0,:,0],'-',c="black")
plt.xlabel("Temperature (K)")
plt.ylabel("Development time (days)")
plt.ylim(0,100)
plt.savefig("figures/ex1Ebr.png",bbox_inches="tight",dpi=300)
plt.close()

t2m = 273.15+30.0+numpy.random.randint(-10,10,30)
out1E = ex1E.sim(len(t2m),envir={"temp":t2m},pr=ex1E.param,y0={"larva":100.0})
plt.plot(numpy.arange(1,len(t2m)),out1E['iret'][0,:,0],'-',c="blue", label="Development time (days)")
plt.plot(out1E['ret'][0,:,0],'o-',c="black", label="Number of larvae")
plt.legend()
plt.xlabel("Time (days)")
plt.ylabel(" ")
plt.ylim(0,110)
plt.savefig("figures/ex1E.png",bbox_inches="tight",dpi=300)
plt.close()
