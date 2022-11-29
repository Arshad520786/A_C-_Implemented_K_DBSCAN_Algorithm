#ifndef KMEANS_H
#define KMEANS_H

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <float.h>
#include "distance.h"


int* Kmeans(double* data, int n_clusters, int datasize, int attrsize){
    srand(time(NULL));
    vector<double*> centroids;
    vector<int> centroidIndex;
    int* clusterID = (int*)malloc(sizeof(int)*datasize);

    // randomly pick n_clusters as centroids.
    int n = 0;
    while(n < n_clusters){
        int choose = rand()%datasize;
        if(find(centroidIndex.begin(), centroidIndex.end(), choose) == centroidIndex.end()){
            double* NewCentroids = (double*)malloc(sizeof(double)*attrsize);
            for(int i = 0 ; i < attrsize ; i++){
                *(NewCentroids + i) = *(data + choose * attrsize + i);
            }
            centroidIndex.push_back(choose);
            centroids.push_back(NewCentroids);
            n++;
        }
    }

    double* clusterMean[n_clusters];
    int clusterSize[n_clusters] = {0};
    for(int i = 0; i < n_clusters; i++){
        clusterMean[i] = (double*)malloc(sizeof(double) * attrsize);
        memset(clusterMean[i], 0, sizeof(sizeof(double) * attrsize));
    }
    
    int iter = 0;
    while(iter < 100){
        //assign each point to the closest centroid
        double minDistance;
        for(int i = 0; i < datasize; i++){
            minDistance = DBL_MAX; //maximum value a double can hold
            for(int j = 0; j < n_clusters; j++){
                double currDistance = distance((data + i * attrsize), centroids[j], attrsize);
                if (currDistance < minDistance){
                    minDistance = currDistance;
                    clusterID[i] = j;
                }
            }
        }
      
        
        //Calculate new mean
        for(int i = 0; i < datasize; i++){
            for(int j = 0; j < attrsize; j++){
                *(clusterMean[clusterID[i]] + j) += *(data + i*attrsize + j);
            }
            clusterSize[clusterID[i]]++;
        }

        for(int i = 0; i < n_clusters; i++){
            for(int j = 0; j < attrsize; j++){
                *(centroids[i] + j) = *(clusterMean[i] + j) / (double)clusterSize[i];
            }
        }
        iter++;

        //clear clusterMean and clusterSize
        
        for(int i = 0; i < n_clusters; i++){
            for(int j = 0; j < attrsize; j++){
                *(clusterMean[i] + j) = 0.0;
            }
            clusterSize[i] = 0;
        }   
    }

    //free memory
    for(int i = 0; i < n_clusters; i++){
        free(clusterMean[i]);
    }
    return clusterID;
}

int* Kmeanspp(double* data, int n_clusters, int max_iter, int datasize, int attrsize){
    //basically the same with kmeans excep the centroid picking part
    srand(time(NULL));
    vector<double*> centroids;
    vector<int> centroidIndex;
    int* clusterID = (int*)malloc(sizeof(int)*datasize);


    // pick first centroid
    int choose = rand()%datasize;
    double* NewCentroids = (double*)malloc(sizeof(double)*attrsize);
    for(int i = 0 ; i < attrsize ; i++){
        *(NewCentroids + i) = *(data + choose * attrsize + i);
    }
    centroidIndex.push_back(choose);
    centroids.push_back(NewCentroids);

    double* ClosestCentroidDistance = (double*)malloc(sizeof(double) * datasize);
    fill_n(&ClosestCentroidDistance[0], datasize, DBL_MAX);
    
    int n = 0;
    int maxDistanceIndex;
    // find the rest of centroids base on its distance to the closest centroid
    while(n < n_clusters - 1){
        for(int i = 0 ; i < datasize ; i++){
            double currentDistance = distance((data + i * attrsize), centroids[n], attrsize);
            if(ClosestCentroidDistance[i] > currentDistance){
                ClosestCentroidDistance[i] = currentDistance;
            } 
        }
        double maxDistance = 0;
        
        for(int i = 0 ; i < datasize ; i++){
            if(maxDistance < ClosestCentroidDistance[i]){
                maxDistance = ClosestCentroidDistance[i];
                maxDistanceIndex = i;
            }
        }

        double* NewCentroids = (double*)malloc(sizeof(double)*attrsize);
        for(int i = 0 ; i < attrsize ; i++){
            *(NewCentroids + i) = *(data + maxDistanceIndex * attrsize + i);
        }
        centroids.push_back(NewCentroids);
        n++;
    }

    free(ClosestCentroidDistance);  //free it since we don't need it anymore


    //initialize two arrays to calculate new means
    double* clusterMean[n_clusters];
    int clusterSize[n_clusters] = {0};
    for(int i = 0; i < n_clusters; i++){
        clusterMean[i] = (double*)malloc(sizeof(double) * attrsize);
        memset(clusterMean[i], 0, sizeof(sizeof(double) * attrsize));
    }
    
    //start iteration
    int iter = 0;
    while(iter < max_iter){
        //assign each point to the closest centroid
        double minDistance;
        for(int i = 0; i < datasize; i++){
            minDistance = DBL_MAX; //maximum value a double can hold
            for(int j = 0; j < n_clusters; j++){
                double currDistance = distance((data + i * attrsize), centroids[j], attrsize);
                if (currDistance < minDistance){
                    minDistance = currDistance;
                    clusterID[i] = j;
                }
            }
        }
      
        
        //Calculate new mean
        for(int i = 0; i < datasize; i++){
            for(int j = 0; j < attrsize; j++){
                *(clusterMean[clusterID[i]] + j) += *(data + i*attrsize + j);
            }
            clusterSize[clusterID[i]]++;
        }

        for(int i = 0; i < n_clusters; i++){
            for(int j = 0; j < attrsize; j++){
                *(centroids[i] + j) = *(clusterMean[i] + j) / (double)clusterSize[i];
            }
        }
        
        //clear clusterMean and clusterSize
        
        for(int i = 0; i < n_clusters; i++){
            for(int j = 0; j < attrsize; j++){
                *(clusterMean[i] + j) = 0.0;
            }
            clusterSize[i] = 0;
        }   
        iter++;
    }

    // free memory
    for(int i = 0; i < n_clusters; i++){
        free(clusterMean[i]);
    }
    
    return clusterID;
}

#endif