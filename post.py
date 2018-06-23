
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt



head = ['r', 'g', 'b', 'cluster']
data = pd.read_table('initial.dat', sep=',', header=None, names=head)

from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

nuage= ax.scatter(data.r, data.g, data.b, c=data.cluster, marker='o')
fig.colorbar(nuage, shrink=0.5, aspect=5)

ax.set_xlabel('r')
ax.set_ylabel('g')
ax.set_zlabel('b')

plt.show()