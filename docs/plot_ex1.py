import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex1a...")
ex1a = pop.model("examples/ex1a.dylib")
out1a = ex1a.sim(30,y0={"larva":100.0})
plt.plot(out1a['ret'][0,:,0],'o-',c="black")
plt.savefig("figures/ex1a.png",bbox_inches="tight",dpi=300)
plt.close()

print("Processing ex1a and ex1b...")
ex1a = pop.model("examples/ex1a.dylib")
ex1b = pop.model("examples/ex1b.dylib")
for n in range(3):
    out1b = ex1b.sim(30,y0={"larva":100.0})
    plt.plot(out1b['ret'][0,:,0],'o')
out1a = ex1a.sim(30,y0={"larva":100.0})
plt.plot(out1a['ret'][0,:,0],'-',c="black")
plt.savefig("figures/ex1b.png",bbox_inches="tight",dpi=300)
plt.close()

print("Processing ex1c...")
ex1c = pop.model("examples/ex1c.dylib")
out1c = ex1c.sim(30,y0={"larva":0.0})
plt.plot(out1c['ret'][0,:,0],'o-',c="black")
plt.savefig("figures/ex1c.png",bbox_inches="tight",dpi=300)
plt.close()
