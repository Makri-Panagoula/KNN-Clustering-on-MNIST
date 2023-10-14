#pragma once
#include <stdlib.h>  
#include <time.h>
#include "../headers/hFunc.h"
#include "../headers/img.h"

using namespace std;

class hashTable {
    private:
        //Number of hash functions out of which the g function will be constructed
        int k;
        //Number of Buckets
        int TableSize;
        //Number we will use for the first mod(we follow the amplified lsh approach)
        int M;
        //A vector consisting of the numerings of the k h-functions needed for the g-function
        vector<hFunc*> funcs;
        //A vector consisting of the k r operands needed for the linear combination
        vector<unsigned int> r; 
        //A vector consisted of other vectors, one for each bucket,where we save a pair of the id and the Img*
        vector<vector<pair<Img*, int>>> buckets;        
    public :

        hashTable(int k ,int H_size, hFunc** funcs, int TableSize,int M);
        //g-function for hashing,returns a pair where fist is id and second bucket
        pair<int, int> g(Img* img);
        //stores img into appropriate bucket
        void store(Img* img);
        //Returns a vector of images in the same bucket deemed as neighbours
        vector<Img*> same_bucket(pair<int, int>);
        ~hashTable(){};
};