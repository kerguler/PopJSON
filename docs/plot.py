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
