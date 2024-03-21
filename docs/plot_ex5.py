import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex5a...")
ex5a = pop.model("examples/ex5a.dylib")
t2m = [273.15+30.0 for n in range(120)]
out5a30 = ex5a.sim(len(t2m),{"temp":t2m},pr=ex5a.param,y0={"immat":100.0})
t2m = [273.15+15.0 for n in range(120)]
out5a15 = ex5a.sim(len(t2m),{"temp":t2m},pr=ex5a.param,y0={"immat":100.0})
plt.plot(out5a30['ret'][0,:,0],'-', c="black",lw=1,label="Immatures (30°C)")
plt.plot(out5a30['ret'][0,:,1],'-', c="black",lw=2,label="Adults (30°C)")
plt.plot(out5a15['ret'][0,:,0],'-', c="blue",lw=1,label="Immatures (15°C)")
plt.plot(out5a15['ret'][0,:,1],'-', c="blue",lw=2,label="Adults (15°C)")
plt.xlabel("Time (days)")
plt.ylabel("")
plt.legend()
plt.savefig("figures/ex5a.png",bbox_inches="tight",dpi=300)
plt.close()
