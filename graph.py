import numpy as np
import csv
import matplotlib.pyplot as plt
from sklearn.cluster import DBSCAN, KMeans, dbscan

datasize = 1500

def get_cmap(n, name='hsv'):
    '''Returns a function that maps each index in 0, 1, ..., n-1 to a distinct 
    RGB color; the keyword argument name must be a standard mpl colormap name.'''
    return plt.cm.get_cmap(name, n)

result = list()
with open("result.txt") as file:
    line = file.readlines()
    splitted = line[0].split(",")
    del splitted[-1]
    for element in splitted:
        result.append((int(element)))



data = np.genfromtxt('donut.csv', delimiter=',')

#print(data)

# print(data)



#clustering = KMeans(n_clusters=3, max_iter=4, init= 'random').fit(data)
clustering = DBSCAN(eps= 0.1, min_samples=3).fit(data)


#for i in range(datasize):
#   print(clustering.labels_[i], result[i])

maxID = max(max(result) ,max(clustering.labels_))

colorList = list()
for i in range(maxID+2):
    colorList.append(np.array((np.random.random(), np.random.random(), np.random.random())).reshape(1, -1))



fig, ax = plt.subplots(2)


#ax = fig.add_subplot(111)
for i in range(datasize):
    ax[0].scatter(data[i][0], data[i][1], c=colorList[result[i]+1])

for i in range(datasize):
    ax[1].scatter(data[i][0], data[i][1], c=colorList[clustering.labels_[i]+1])


plt.show()
