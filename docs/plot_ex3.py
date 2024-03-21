import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex3a...")
ex3a = pop.model("examples/ex3a.dylib")
out3a = ex3a.sim(30,y0={"adult":100.0})
plt.plot(out3a['ret'][0,:,0],'o-',c="black",label="Adult alive")
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,0],'o-',c="red",label="Adult dead")
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,1],'o-',c="blue",label="Gravid females")
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,2],'o-',c="green",label="Newly laid eggs")
plt.legend()
plt.savefig("figures/ex3a.png",bbox_inches="tight",dpi=300)
plt.close()

print("Processing ex3a and ex3b...")
ex3a = pop.model("examples/ex3a.dylib")
ex3b = pop.model("examples/ex3b.dylib")
out3a = ex3a.sim(30,y0={"adult":100.0})
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,2],'o-',c="black",label="Newly laid eggs")
for n in range(3):
    out3b = ex3b.sim(30,y0={"adult":100.0})
    plt.plot(numpy.arange(1,30),out3b['iret'][0,:,2],'o',c="blue")
plt.legend()
plt.savefig("figures/ex3b.png",bbox_inches="tight",dpi=300)
plt.close()
