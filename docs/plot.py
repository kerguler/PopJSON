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
plt.plot(out2a['iret'][0,:,0],'o-',c="red",label="Larva dead")
plt.plot(out2a['iret'][0,:,1],'o-',c="blue",label="Pupa")
plt.legend()
plt.savefig("figures/ex2a.png",bbox_inches="tight",dpi=300)
plt.close()
