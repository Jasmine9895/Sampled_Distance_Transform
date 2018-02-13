import matplotlib.pyplot as plt
import numpy as np
with open('example.txt') as f:
    #w, h = [int(x) for x in next(f).split()]
    array = [[float(x) for x in line.split()] for line in f]

print(array[0])
print(array[1])
t = np.arange(0,10)
print t

plt.plot(t,array[1],t,array[0])
#plt.subplot(211)
plt.title("array")
#
#plt.plot(t,array[0])
#plt.subplot(212)
#plt.title("distance_transform")


plt.show()
