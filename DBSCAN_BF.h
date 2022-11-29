#ifndef DBSCAN_BF_H
#define DBSCAN_BF_H
#include <vector>
#include <math.h>
#include <string.h>
#include "distance.h"

using namespace std;

enum Pstatus{
    nil = 0,
    core = 1,
    border = 2,
    noise = 3
};

void DensityConnect(int pointIndex, int k, int* clusterID, vector<int> EpsNeighborhood[], Pstatus pointStatus[]){
    for(auto iter = EpsNeighborhood[pointIndex].begin(); iter != EpsNeighborhood[pointIndex].end(); ++iter){
        int currentPoint = *iter;
        if(pointIndex == currentPoint || clusterID[currentPoint] != -1){
            continue;
        }
        clusterID[currentPoint] = k;
        if(pointStatus[currentPoint] == core){
            DensityConnect(currentPoint, k, clusterID, EpsNeighborhood, pointStatus);
        }
    }
}

int* DBSCAN_BF(double* dataset, double epsilon, int minpts, int size, int attsize, int startingClusterID = 0){
    // return clusterID of each data point
    vector<int> EpsNeighborhood[size];
    Pstatus pointStatus[size]; //Core, Border or Noise

    for(int i = 0 ; i < size ; i++){
        pointStatus[i] = nil;
    }

    int* clusterID = (int*)malloc(sizeof(int)*size);
    fill_n(clusterID, size, -1);

    // find each point's epsilon neighborhood and if they are core point
    for(int i = 0 ; i < size ; i++){
        double* currentPoint = dataset + i * attsize;
        for(int j = 0 ; j < size ; j++){
            double* datapoint = dataset + j * attsize;
            if(distance(currentPoint, datapoint, attsize) < epsilon){
                EpsNeighborhood[i].push_back(j);
            }
        }
        if(EpsNeighborhood[i].size() >= minpts){
            pointStatus[i] = core;
        }
    }
    // for each core point density connect it's cluster
    int k = startingClusterID;
    for(int i = 0 ; i < size ; i++){
        if(pointStatus[i] == core && clusterID[i] == -1){
            k = k + 1;
            clusterID[i] = k;
            DensityConnect(i, k, clusterID, EpsNeighborhood, pointStatus);
        }
    }
    // find noise points
    for(int i = 0 ; i < size ; i++){
        if(clusterID[i] == -1){
            pointStatus[i] = noise;
        }
    }
    // identify border points
    for(int i = 0 ; i < size ; i++){
        if(pointStatus[i] == 0){
            pointStatus[i] = border;
        }
    }
    return clusterID;
}

#endif 