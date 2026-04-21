import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex8...")
ex = pop.model("examples/ex8.dylib")

N = 100

k = min(numpy.random.poisson(20), N)
indices = numpy.random.choice(N, size=k, replace=True)
values = numpy.abs(numpy.random.normal(0, 1, size=k))

prec = numpy.repeat(0.0, N)
prec[indices] = values

out = ex.sim(N,
             y0={
                "bsvol": 0.0
             },
             envir={
                "prec": prec
             },
             pr=[1.0, 0.5])

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
plt.savefig("figures/ex8.png",bbox_inches="tight",dpi=300)
plt.close()
