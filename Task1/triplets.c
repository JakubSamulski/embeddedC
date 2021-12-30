#include "stdlib.h"
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "hashTable.h"


typedef struct Pair{
    int first;
    int second;
}Pair;

typedef struct Triplet{
    int first;
    int second;
    int third;
}Triplet;

// o(n) using histogram
Pair* findPairs(int * array,int sum){
    Pair *res = NULL;

    int maxElem=array[0];
    for(int i =1;i< array_length(array);i++){
        if(maxElem<array[i]){
            maxElem = array[i];
        }
    }
    int histogram[maxElem];
    for(int i =0;i<maxElem;i++){
        histogram[i]=0;
    }

    for(int i =0;i< array_length(array);i++){
        histogram[array[i]]++;
    }

    for(int i =0;i< array_length(array);i++){
        int index = sum - array[i];
        if(histogram[index]!=0){
            Pair pair;
            pair.first = array[i];
            pair.second = index;
            array_push(res,pair);
        }
    }
    return res;
}

Triplet * sumOfThree(int * array, int targetSum){

    Triplet *result = NULL;
    int* sumsArr = NULL;

    for (int i = 0; i < array_length(array); i++){
        array_push(sumsArr, targetSum - array[i]);
    }

    for (int i = 0; i < array_length(array); i++){
        int targetSumInner = sumsArr[i];
        for (int j = 0; j < array_length(array); j++){
            if(i!=j) {
                insert(targetSumInner - array[j], array[j]);
            }
        }
        for (int j = 0; j < array_length(array); j++) {
            if(i!=j) {
                item = search(array[j]);
                if (item != NULL) {
                    Triplet triplet;
                    triplet.first = array[j];
                    triplet.second = item->data;
                    triplet.third = targetSum - sumsArr[i];
                    array_push(result, triplet);
                }
            }
        }

    }
    return result;
}


void printPairs(Pair *pairs){
    for(int i =0;i< array_length(pairs);i++){
        printf("(%d,%d)\n",pairs[i].first,pairs[i].second);
    }
}

void printTriplets(Triplet *triplets)
{
    for(int i=0;i< array_length(triplets);i++){
        printf("[%d,%d,%d]\n",triplets[i].first,triplets[i].second,triplets[i].third);
    }
}


int main(){

    int * array=NULL;
    array_push(array,1)
    array_push(array,2)
    array_push(array,3)
    array_push(array,4)
    system("clear");

    Pair *pairs = findPairs(array,5);
    printPairs(pairs);

//    Triplet *tres = sumOfThree(array,6);
//    printTriplets(tres);
}