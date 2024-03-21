import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex4a...")
ex4a = pop.model("examples/ex4a.dylib")
out4a = ex4a.sim(30,y0={"adult":100.0})
plt.plot(out4a['ret'][0,:,0],'o-',c="black",label="Adult alive")
plt.plot(numpy.arange(1,30),out4a['iret'][0,:,0],'o-',c="red",label="Adult dead")
plt.plot(numpy.arange(1,30),out4a['iret'][0,:,1],'o-',c="blue",label="Gravid females")
plt.plot(numpy.arange(1,30),out4a['iret'][0,:,2],'o-',c="green",label="Newly laid eggs")
plt.plot(numpy.arange(1,30),out4a['iret'][0,:,3],'o-',c="orange",label="Total number of eggs")
plt.legend()
plt.savefig("figures/ex4a.png",bbox_inches="tight",dpi=300)
plt.close()
