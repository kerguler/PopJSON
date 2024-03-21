import numpy
import matplotlib
from matplotlib import pyplot as plt
from matplotlib import cm
import population as pop

print("Processing ex7a...")
ex = pop.model("examples/ex7a.dylib")

G = 3
C = 8
tf = 200

dfun = lambda p0,p1: abs(p1[0]-p0[0])+abs(p1[1]-p0[1])
pts = [(x,y) for x in range(C) for y in range(C)]
tprobs = numpy.array([[0.25 if dfun(p0,p1)==1 else 0.0 for p1 in pts] for p0 in pts])
numpy.fill_diagonal(tprobs,1.0-numpy.sum(tprobs,axis=1))
tprobs = tprobs.reshape(C*C*C*C)

genes = [0, 1, 2]
funcube = lambda g0,g1: {
    (0,0):[1.0,0.0,0.0], # AA x AA
    (0,1):[0.5,0.5,0.0], # AA x Aa
    (0,2):[0.0,1.0,0.0], # AA x aa
    (1,0):[0.5,0.5,0.0], # Aa x AA
    (1,1):[0.25,0.5,0.25], # Aa x Aa
    (1,2):[0.0,0.5,0.5], # Aa x aa
    (2,0):[0.0,1.0,0.0], # aa x AA
    (2,1):[0.0,0.5,0.5], # aa x Aa
    (2,2):[0.0,0.0,1.0], # aa x aa
    }[(g0,g1)]
cube = numpy.array([[funcube(g0,g1) for g0 in genes] for g1 in genes])
cube = cube.reshape(G**3)

envir = {
            "tprob_0":tprobs,
            "tprob_1":tprobs,
            "tprob_2":tprobs,
            "inheritance":cube
        }
out = ex.sim(tf,envir=envir,pr=ex.param,y0={"immature_0_0":100.0,"immature_2_63":100.0})
ret = out['ret'][0,:,:].reshape(tf,2,G,C,C)

X = range(8)
Y = range(8)
X, Y = numpy.meshgrid(X, Y)
for i in [0,40,160]:
    fig = plt.figure()
    ax = fig.add_subplot(1,2,1,projection='3d')
    tmp = ax.plot_wireframe(X,Y,ret[i,0,0,:,:],color="red",label="AA")
    tmp = ax.plot_wireframe(X,Y,ret[i,0,1,:,:],color="green",label="Aa")
    tmp = ax.plot_wireframe(X,Y,ret[i,0,2,:,:],color="blue",label="aa")
    tmp = ax.set_title("Immature stages")
    tmp = ax.set_zlim(0,10)
    tmp = ax.set_xticklabels([])
    tmp = ax.set_yticklabels([])
    tmp = ax.set_zticklabels([])
    tmp = ax.legend()
    ax = fig.add_subplot(1,2,2,projection='3d')
    tmp = ax.plot_wireframe(X,Y,ret[i,1,0,:,:],color="red",label="AA")
    tmp = ax.plot_wireframe(X,Y,ret[i,1,1,:,:],color="green",label="Aa")
    tmp = ax.plot_wireframe(X,Y,ret[i,1,2,:,:],color="blue",label="aa")
    tmp = ax.set_title("Adults")
    tmp = ax.set_zlim(0,10)
    tmp = ax.set_xticklabels([])
    tmp = ax.set_yticklabels([])
    tmp = ax.set_zticklabels([])
    tmp = ax.legend()
    tmp = plt.savefig("figures/ex7a%03d.png" %i,bbox_inches="tight",dpi=100)
    tmp = plt.close()
