#include "../headers/hashTable.h"

using namespace std;

hashTable::hashTable(int k , int H_size, hFunc* funcs, int TableSize) {

    this->k = k;
    //Initialize uniform distributor
    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,H_size);
    //Initialize random seed
    srand (time(NULL));    
    //Choose uniformly the k functions from H that will be used for g and randomly the r-operands
    for(int i = 0; i < k ; i++) {
        int chosen = distribution(generator);
        this->funcs.push_back(funcs[chosen]);
        this->r.push_back(rand());
    }
    this->TableSize = TableSize;
    //Create a vector for every bucket
    for(int i = 0; i < TableSize; i++) {
        vector<Img> bucket;
        this->buckets.push_back(bucket);
    }
}

unsigned int hashTable::g(vector<unsigned int> p) {

    //Estimate all the h-values and get the dot product with the stored k r-operands 
    vector<unsigned int> h_values;

    for(int i = 0; i < this->k; i++) 
        h_values.push_back(funcs[i].h(p));
    
    unsigned int linear_comb = inner_product(h_values.begin(),h_values.end(),this->r.begin(),0);
    unsigned int chosen = linear_comb % this->TableSize;
    //Store image in chosen bucket
    this->buckets[chosen].push_back(p);
}