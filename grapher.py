# grapher.py
#
# Author: Devon Gulley
# 
# Completion date: (ongoing)
#
# Reads out.sys and plots the positions in space of the system created by driver.cpp
# Displays the motion of the system, and can be modified to show trajectories etc
#

#To read out.sys
f = open("out.sys", "r")

#Positions are stored at an instant in time for object_j in the form position[object_j] = (x_j,y_j,z_j)
position = []



#while line:


import matplotlib as mpl
import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(projection='3d')
xs = []
ys = []
zs = []


#Read the data from every 200th line and record it in the visual state
visualStates = []
step = 1
with open("out.sys") as handle:
    for lineno, line in enumerate(handle):
        if lineno % step == 0:
            visualStates.append(line)



def plotState(n):

    position = visualStates[n].split(";")
    xs = []
    ys = []
    zs = []
    for i in range(len(position)-1):
        position[i] = position[i].split(",")
        xs.append(float(position[i][0]))
        ys.append(float(position[i][1]))
        zs.append(float(position[i][2]))
    ax.cla()
    #ax.set_anchor('W')
    view = 0.125;
    ax.set_xlim(0.5-view,0.5+view)
    ax.set_ylim(0.5-view,0.5+view)
    ax.set_zlim(0.5-view,0.5+view)
    ax.scatter(xs,ys,zs)



ani = animation.FuncAnimation(fig,plotState,frames= len(visualStates), interval=10)
plt.show()
