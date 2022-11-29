#ifndef DISTANCE_H
#define DISTANCE_H

#include <math.h>
using namespace std;

double distance(double* pointA, double* pointB, int attsize){
    double sum = 0.0;
    for (int i = 0; i < attsize; i++){
        sum += (*(pointA + i) - *(pointB + i)) * (*(pointA + i) - *(pointB + i));
    }
    return sqrt(sum);
}

#endif