import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex8...")
ex = pop.model("examples/ex8.dylib")

N = 100000

out = ex.sim(N, y0={
    "xx": 1.0,
    "yy": 1.0,
    "zz": 1.0
}, pr = [10.0, 28.0, 8.0/3.0, 0.001])

# Plot
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')

t = numpy.arange(out['ret'].shape[1])
ax.scatter(out['ret'][0, :, 0], out['ret'][0, :, 1], out['ret'][0, :, 2], c=t, s=0.1)  # color by time

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

plt.tight_layout()
plt.savefig("figures/ex8.png",bbox_inches="tight",dpi=300)
plt.close()


