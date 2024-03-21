import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex2a...")
ex2a = pop.model("examples/ex2a.dylib")
out2a = ex2a.sim(30,y0={"larva":100.0})
plt.plot(out2a['ret'][0,:,0],'o-',c="black",label="Larva alive")
plt.plot(numpy.arange(1,30),out2a['iret'][0,:,0],'o-',c="red",label="Larva dead")
plt.plot(numpy.arange(1,30),out2a['iret'][0,:,1],'o-',c="blue",label="Pupa")
plt.legend()
plt.savefig("figures/ex2a.png",bbox_inches="tight",dpi=300)
plt.close()
