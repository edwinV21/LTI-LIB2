
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.legend_handler import HandlerLine2D
import numpy as np

plt.xlabel('Numero Iteraciones')
plt.ylabel('Error')

plt.figure(1)
#plt.subplot(211)
line1, = plt.plot(*np.loadtxt("pesagraph.txt",unpack=True),label="PESA", linewidth=2.0)


# now switch back to figure 1 and make some changes
plt.figure(1)
#plt.subplot(211)
line2,=plt.plot(*np.loadtxt("nsgagraph.txt",unpack=True),label="NSGA2",linewidth=2.0)
#ax = plt.gca()
#ax.set_xticklabels([])
#NSGA_patch = mpatches.Patch(color='green', label='NSGA2')

plt.legend(handler_map={line1: HandlerLine2D(numpoints=4)})
#PESA_patch = mpatches.Patch(color='blue', label='PESA')
#plt.legend(handles=[PESA_patch],loc=2)

plt.show()
