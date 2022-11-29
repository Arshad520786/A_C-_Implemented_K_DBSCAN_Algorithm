#ifndef K_DBSCAN_H
#define K_DBSCAN_H

#include "DBSCAN_BF.h"
#include "Kmeans.h"
#include "distance.h"


int* K_DBSCAN(double* data, int n_clusters, double eps, int minpts, int datasize, int attrsize){
    int* clusterID = (int*)malloc(sizeof(int)*datasize);
    
    // group each point with K-means++ 

    int* groups = Kmeanspp(data, n_clusters, 4, datasize, attrsize);
    
    // initialize data in each group for DBSCAN

    int groupSize[n_clusters] = {0};
    for(int i = 0; i < datasize; i++){
        groupSize[groups[i]]++;
    }

    double* groupData[n_clusters];
    int* originTable[n_clusters];
    for(int i = 0; i < n_clusters; i++){
        groupData[i] = (double*)malloc(sizeof(double)*groupSize[i]*attrsize);
        originTable[i] = (int*)malloc(sizeof(int)*groupSize[i]);
    }

    int written[n_clusters] = {0};
    for(int i = 0; i < datasize; i++){
        for(int j =0; j < attrsize; j++){
            *(groupData[groups[i]] + written[groups[i]] * attrsize + j) = *(data + i * attrsize + j);
        }
        *(originTable[groups[i]] + written[groups[i]]) = i;
        written[groups[i]]++;
    }

    free(groups);


    //For each group, perform DBSCAN.
    int* groupResult[n_clusters];
    int maxClusterID = 0;
    for(int i = 0; i < n_clusters; i++){
        groupResult[i] = DBSCAN_BF(groupData[i], eps, minpts, groupSize[i], attrsize, maxClusterID);
        for(int j = 0; j < groupSize[i]; j++){
            if (groupResult[i][j] > maxClusterID)
                maxClusterID = groupResult[i][j];
        }
    }

    // write the reuslt of group dbscan back to original data
    for(int i = 0; i < n_clusters; i++){
        for(int j = 0; j < groupSize[i]; j++){
            clusterID[originTable[i][j]] = groupResult[i][j];
        }
    }

    // free groupResult
    for(int i = 0; i < n_clusters; i++){
        free(groupResult[i]);
    }
    

    // calculate the mean of the point and farthest distance from mean to a group point
    double* GroupMean[n_clusters];
    double farthestDistanceOfGroup[n_clusters];
    for(int i = 0; i < n_clusters; i++){
        GroupMean[i] = (double*)malloc(sizeof(double) * attrsize);
        memset(GroupMean[i], 0, sizeof(sizeof(double) * attrsize));
        for(int j = 0; j < groupSize[i]; j++){
            for(int attr = 0; attr < attrsize; attr++){
                *(GroupMean[i] + attr) += *(groupData[i] + j * attrsize + attr);
            }
        }

        for(int attr = 0; attr < attrsize; attr++){
            *(GroupMean[i] + attr) = *(GroupMean[i] + attr) / (double)groupSize[i];
        }

        farthestDistanceOfGroup[i] = DBL_MIN;
        for(int j = 0; j < groupSize[i]; j++){
            double currentDistance = distance(GroupMean[i], groupData[i] + j * attrsize, attrsize);
            if(currentDistance > farthestDistanceOfGroup[i]){
                farthestDistanceOfGroup[i] = currentDistance;
            }
        }
    }

    

    //assign which groups need to perform DBSCAN together
    int counter = 1;
    int flag[n_clusters] = {0};
    for(int i = 0; i < n_clusters; i++){
        for(int j= 0; j < n_clusters; j++){
            double currentDistance = distance(GroupMean[i], GroupMean[j], attrsize) - farthestDistanceOfGroup[i] - farthestDistanceOfGroup[j];
            if (currentDistance <= eps){
                if(flag[i] == 0 && flag[j] == 0){
                    flag[i] = counter;
                    flag[j] = counter;
                    counter++;
                }
                else{
                    if(flag[i] > flag[j])
                        flag[j] = flag[i];
                    else if (flag[j] > flag[i])
                        flag[i] = flag[j];
                }
            }
        }
    }

    // free GroupMean
    for(int i = 0; i < n_clusters; i++){
        free(GroupMean[i]);
    }

    // transfer groups with same flag to a vector
    vector<vector<int>> groupVector;
    int maxFlag = INT32_MIN;
    for(int i = 0; i < n_clusters; i++){
        if(maxFlag < flag[i])
            maxFlag = flag[i];
    }
        
    int flagCount[maxFlag + 1] = {0};
    for(int i = 0; i < n_clusters; i++){
        flagCount[flag[i]]++;
    }


    for(int i = 1; i <= maxFlag; i++){
        if(flagCount[i] == 0)
            continue;
        vector<int> newvector;
        groupVector.push_back(newvector);
        for(int j = 0; j < n_clusters; j++){
            if(flag[j] == i){
                groupVector.back().push_back(j);
            }
        }
    }

    // initialize new data to perform dbscan on groups with same flag(in same vector)

    double* newGroupData[groupVector.size()];
    int* newOriginTable[groupVector.size()];
    int newGroupSize[groupVector.size()] = {0};
    int NewWritten[groupVector.size()] = {0};
    for(int i = 0; i < groupVector.size(); i++){
        for(auto iter = groupVector[i].begin(); iter < groupVector[i].end(); iter++){
            newGroupSize[i] += groupSize[*iter];
        }
        newGroupData[i] = (double*)malloc(sizeof(double)*newGroupSize[i]*attrsize);
        newOriginTable[i] = (int*)malloc(sizeof(int)*newGroupSize[i]);
        
        for(int j = 0 ; j < groupVector[i].size(); j++){
            for(int k = 0 ; k < groupSize[groupVector[i][j]] ; k++){
                for(int attr = 0 ; attr < attrsize ; attr++){
                    *(newGroupData[i] + NewWritten[i] * attrsize + attr) = *(groupData[groupVector[i][j]] + k * attrsize + attr);
                }
                *(newOriginTable[i] + NewWritten[i]) = *(originTable[groupVector[i][j]] + k);
                NewWritten[i]++;
            } 
        }
    }

    for(int i = 0 ; i < n_clusters; i++){
        free(groupData[i]);
        free(originTable[i]);
    }


    // for each New group perform DBSCAN
    int* newGroupResult[groupVector.size()];
    for(int i = 0; i < groupVector.size(); i++){
        newGroupResult[i] = DBSCAN_BF(newGroupData[i], eps, minpts, newGroupSize[i], attrsize, maxClusterID);
        for(int j = 0; j < newGroupSize[i]; j++){
            if (newGroupResult[i][j] > maxClusterID)
                maxClusterID = newGroupResult[i][j];
            clusterID[newOriginTable[i][j]] = newGroupResult[i][j];
        }
        free(newGroupData[i]);
        free(newOriginTable[i]);
    }
    

    return clusterID;
}


#endif