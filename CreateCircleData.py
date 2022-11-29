import numpy as np
import matplotlib.pyplot as plt
from sklearn import cluster, datasets
from sklearn.preprocessing import StandardScaler
import csv

n_samples = 1000000
noisy_circles = datasets.make_circles(n_samples=n_samples, factor=0.5, noise=0.05)


for data in noisy_circles[0]:
    plt.scatter(data[0], data[1])


with open("donut.csv", 'w', newline = '') as csvfile:
    writer = csv.writer(csvfile)
    writer = writer.writerows(noisy_circles[0])

plt.show()