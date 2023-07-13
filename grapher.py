# grapher.py
#
# Author: Devon Gulley
# 
# Completion date: (ongoing)
#
# Reads out.sys and plots the positions in space of the system created by driver.cpp
# Creates an image of the positions, and can be modified to show trajectories etc
#

#To read out.sys
f = open("out.sys", "r")
line = f.readline();

#Positions are stored at an instant in time for object_j in the form position[object_j] = (x_j,y_j,z_j)
position = []

#Starting with one line:
position = line.split(";")

xs = []
ys = []
zs = []

for i in range(len(position)-1):
    position[i] = position[i].split(",")
    xs.append(position[i][0])
    ys.append(position[i][1])
    zs.append(position[i][2])

#while line:


import matplotlib as mpl
import numpy as np
import matplotlib.animation as animation
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(projection='3d')

ax.scatter(xs,ys,zs)
plt.show()
