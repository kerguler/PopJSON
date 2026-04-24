import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex10...")
ex = pop.model("examples/ex10.dylib")

N = 100

temp = numpy.random.normal(25.0, 5.0, size=N)

k = min(numpy.random.poisson(20), N)
indices = numpy.random.choice(N, size=k, replace=True)
values = numpy.abs(numpy.random.normal(0, 1, size=k))

prec = numpy.repeat(0.0, N)
prec[indices] = values

out = ex.sim(N,
             y0={
                "larva": 100.0
             },
             envir={
                "temp": temp,
                "prec": prec
             },
             pr=ex.param)

x = numpy.arange(len(prec))
fig, ax1 = plt.subplots(figsize=(10, 5))
width = 1.0
ax1.bar(x + width/2, prec, alpha=0.5, width=width, label="Precipitation (mm)", color="#4C72B0")
ax1.plot(out['iret'][0,:,ex.intids['bsvol']])
ax2 = ax1.twinx()
ax2.plot(temp)
ax2.plot(out['ret'][0,:,ex.popids['larva']])
plt.show()


x = numpy.arange(len(prec))

fig, ax1 = plt.subplots(figsize=(10, 5))

# --- Bars for precipitation and evaporation ---
width = 1.0
ax1.bar(x + width/2, prec, width=width, label="Precipitation (mm)", color="#4C72B0")

ax1.set_ylabel("Water flux (mm)")
ax1.set_xlabel("Time step")
ax1.set_title("Precipitation, Evaporation, and Model Output")
ax1.grid(axis="y", linestyle="--", alpha=0.4)

ax1.set_ylim(bottom=0)

# --- Secondary axis for model output ---
ax2 = ax1.twinx()
ax2.plot(x, out['ret'][0,:,0], '-', color="black", label="Model output")
ax2.set_ylabel("Model output")

ax2.set_ylim(bottom=0)

# --- Combine legends ---
handles1, labels1 = ax1.get_legend_handles_labels()
handles2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(handles1 + handles2, labels1 + labels2, frameon=False, loc="upper right")

plt.tight_layout()
plt.savefig("figures/ex9.png",bbox_inches="tight",dpi=300)
plt.close()
