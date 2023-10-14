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
        //A vector consisting of the numerings of the k h-functions needed for the g-function
        vector<hFunc*> funcs;
        //A vector consisting of the k r operands needed for the linear combination
        vector<unsigned int> r;        
        //A vector consisted of images' vectors , one for each bucket
        vector<vector<Img*>> buckets;
    public :

        hashTable(int k ,int H_size, hFunc** funcs, int TableSize);
        //g-function for inserting image to appropriate bucket
        unsigned int g(Img* img);
        ~hashTable();
};