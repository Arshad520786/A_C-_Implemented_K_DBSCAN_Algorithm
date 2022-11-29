#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <cstring>
#include "DBSCAN_BF.h"
#include "K_DBSCAN.h"
#include <time.h>

using namespace std;

#define dataSize 1500
#define attributeSize 2

double timediff(timespec beg_t, timespec end_t){
    if((beg_t.tv_nsec - end_t.tv_nsec) > 0)
        return  (end_t.tv_sec - beg_t.tv_sec - 1) + (1000000000.0 - beg_t.tv_nsec + end_t.tv_nsec)/1000000000.0;
    else 
        return  (end_t.tv_sec - beg_t.tv_sec) + (end_t.tv_nsec - beg_t.tv_nsec)/1000000000.0;
}


int main(int argc, char* argv[]){
    double epsilon = atof(argv[1]);
    int minpts = atoi(argv[2]);
    /*----------Load Data--------*/
    fstream f;
    f.open("donut.csv");

    if(!f.is_open()){
        cout << "File error, reading failed." << endl;
        exit(0);
    }
    double* data = (double*)malloc(sizeof(double)*dataSize*attributeSize);
    string line;
    int i = 0;
    while(getline(f, line)){
        stringstream lineStream(line);
        string temp;
        int j = 0;
        while(getline(lineStream, temp, ',')){
            *(data + i* attributeSize + j) = stof(temp);
            j++;
        }
        i++;
    }

    

    /*--------DBSCAN--------*/

    
    timespec beg_t, end_t;
    /*
    clock_gettime(CLOCK_MONOTONIC, &beg_t);
    int* output = K_DBSCAN(data, 100, epsilon, minpts, dataSize, attributeSize);
    clock_gettime(CLOCK_MONOTONIC, &end_t);

    double K_time = timediff(beg_t, end_t);
    */
    
    clock_gettime(CLOCK_MONOTONIC, &beg_t);
    int* output2 = DBSCAN_BF(data, epsilon, minpts, dataSize, attributeSize);
    clock_gettime(CLOCK_MONOTONIC, &end_t);
    
    double original_time = timediff(beg_t, end_t);
    
    for(int i = 0 ; i < dataSize ; i++){
        printf("%d,",output2[i]);    
    }
        printf("\n");    
    
    cout << "DBSCAN time : " << original_time << " s" << endl;
    //cout << "K_DBSCAN time : " << K_time << " s" << endl;

    return 0;
}