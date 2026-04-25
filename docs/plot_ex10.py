import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex10...")
ex = pop.model("examples/ex10.dylib")

N = 50

k = min(numpy.random.poisson(10), N)
indices = numpy.random.choice(N, size=k, replace=True)
values = numpy.abs(numpy.random.normal(0, 1, size=k))

prec = numpy.repeat(0.0, N)
prec[indices] = values

out = ex.sim(N,
             y0={
                "larva": 100.0
             },
             envir={
                "prec": prec
             })

width = 1.0
x = numpy.arange(len(prec))

fig, ax1 = plt.subplots(figsize=(10, 5))

# --- Bars: Precipitation ---
ax1.bar(
    x + width/2,
    prec,
    width=width,
    alpha=0.6,
    color="#4C72B0",
    label="Precipitation (mm)"
)

# --- Lines: rates ---
ax1.plot(
    x,
    out['iret'][0, :, ex.intids['bsvol']],
    'o-',
    color="#55A868",
    linewidth=2,
    markersize=5,
    label="bsvol"
)

ax1.plot(
    x,
    out['iret'][0, :, ex.intids['bsmort']],
    'o-',
    color="#C44E52",
    linewidth=2,
    markersize=5,
    label="bsmort"
)

ax1.axhline(y=0.75, color="gray", linestyle="--", linewidth=1)

ax1.set_xlabel("Time")
ax1.set_ylabel("Rates / Precipitation")
ax1.grid(axis="y", linestyle="--", alpha=0.4)

# --- Secondary axis ---
ax2 = ax1.twinx()

ax2.plot(
    x,
    out['ret'][0, :, ex.popids['larva']],
    color="#8172B2",
    linewidth=2,
    label="Larva"
)

ax2.set_ylabel("Larval population")

# --- Align zeros ---
ax2.set_ylim(bottom=0.0)
ax1.set_ylim(bottom=0.0)

# --- Combined legend ---
lines_1, labels_1 = ax1.get_legend_handles_labels()
lines_2, labels_2 = ax2.get_legend_handles_labels()

ax1.legend(
    lines_1 + lines_2,
    labels_1 + labels_2,
    frameon=False
)

plt.tight_layout()
plt.savefig("figures/ex10.png",bbox_inches="tight",dpi=300)
plt.close()