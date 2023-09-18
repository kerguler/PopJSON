import numpy
from matplotlib import pyplot as plt
import population as pop

ex1a = pop.model("examples/ex1a.dylib")
out1a = ex1a.sim(30,[],[],[100.0])
plt.plot(out1a['ret'][0,:,0],'o-',c="black")
plt.savefig("figures/ex1a.png",bbox_inches="tight",dpi=300)
plt.close()

ex1a = pop.model("examples/ex1a.dylib")
ex1b = pop.model("examples/ex1b.dylib")
for n in range(3):
    out1b = ex1b.sim(30,[],[],[100.0])
    plt.plot(out1b['ret'][0,:,0],'o')
out1a = ex1a.sim(30,[],[],[100.0])
plt.plot(out1a['ret'][0,:,0],'-',c="black")
plt.savefig("figures/ex1b.png",bbox_inches="tight",dpi=300)
plt.close()

ex2a = pop.model("examples/ex2a.dylib")
out2a = ex2a.sim(30,[],[],[100.0])
plt.plot(out2a['ret'][0,:,0],'o-',c="black",label="Larva alive")
plt.plot(numpy.arange(1,30),out2a['iret'][0,:,0],'o-',c="red",label="Larva dead")
plt.plot(out2a['iret'][0,:,1],'o-',c="blue",label="Pupa")
plt.legend()
plt.savefig("figures/ex2a.png",bbox_inches="tight",dpi=300)
plt.close()

ex3a = pop.model("examples/ex3a.dylib")
out3a = ex3a.sim(30,[],[],[100.0])
plt.plot(out3a['ret'][0,:,0],'o-',c="black",label="Adult alive")
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,0],'o-',c="red",label="Adult dead")
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,1],'o-',c="blue",label="Gravid females")
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,2],'o-',c="green",label="Newly laid eggs")
plt.legend()
plt.savefig("figures/ex3a.png",bbox_inches="tight",dpi=300)
plt.close()

ex3a = pop.model("examples/ex3a.dylib")
ex3b = pop.model("examples/ex3b.dylib")
out3a = ex3a.sim(30,[],[],[100.0])
plt.plot(numpy.arange(1,30),out3a['iret'][0,:,2],'o-',c="black",label="Newly laid eggs")
for n in range(3):
    out3b = ex3b.sim(30,[],[],[100.0])
    plt.plot(numpy.arange(1,30),out3b['iret'][0,:,2],'o',c="blue")
plt.legend()
plt.savefig("figures/ex3b.png",bbox_inches="tight",dpi=300)
plt.close()
