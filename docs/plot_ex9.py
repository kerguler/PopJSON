import numpy
from matplotlib import pyplot as plt
import population as pop

print("Processing ex9...")
ex = pop.model("examples/ex9.dylib")

N = 100000

out = ex.sim(N, y0={
    "xx": 1.0,
    "yy": 1.0,
    "zz": 1.0
})

# Extract trajectory
traj = out['ret'][0, 1:, :]
x = traj[:, 0]
y = traj[:, 1]
z = traj[:, 2]

# Plot
fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection='3d')

ax.plot(x, y, z)

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
ax.set_title('Lorenz Trajectory')

plt.tight_layout()
plt.show()


