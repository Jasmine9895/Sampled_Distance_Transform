import matplotlib.pyplot as plt
import numpy as np
with open('example.txt') as f:
    #w, h = [int(x) for x in next(f).split()]
    array = [[float(x) for x in line.split()] for line in f]

print(array[0])
print(array[1])
t = np.arange(0,30)
print(t)

#plt.plot(t,array[1],t,array[2])
plt.plot(t,array[0],t,array[1],t,array[2]) #normal case
#plt.plot(t,array[0],t,array[1],t,array[2],t,array[3]) #normal case
plt.xticks(np.arange(min(t), max(t)+1, 2.0))
plt.grid()
#plt.subplot(211)
plt.title("array")
#
#plt.plot(t,array[0])
#plt.subplot(212)
#plt.title("distance_transform")

#This is so that the plot window closes, to end the program
plt.show(block=False)
input("Hit Enter To Close")
plt.close()
