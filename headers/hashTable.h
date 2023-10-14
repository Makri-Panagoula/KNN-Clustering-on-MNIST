#pragma once
#include <stdlib.h>  
#include <time.h>
#include "../headers/hFunc.h"
#include "../headers/img.h"


using namespace std;

class hashTable {
    private:

    public :
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
        //A vector consisted of images' vectors , one for each bucket
        vector<vector<Img*>> buckets;
        hashTable(int k ,int H_size, hFunc** funcs, int TableSize,int M);
        //g-function for inserting image to appropriate bucket
        void g(Img* img);
        ~hashTable(){};
};