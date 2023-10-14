#include "../headers/hashTable.h"

using namespace std;

hashTable::hashTable(int k , int H_size, hFunc** funcs, int TableSize,int M) {

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
    this->M = M;
    //Create a vector for every bucket
    for(int i = 0; i < TableSize; i++) {
        vector<Img*> bucket;
        this->buckets.push_back(bucket);
    }
}

void hashTable::g(Img* img) {

    //Accessing pixels' vector
    vector<unsigned int> p = img->get_p();

    //Estimate all the h-values and get the dot product with the stored k r-operands 
    vector<unsigned int> h_values;

    for(int i = 0; i < this->k; i++) 
        h_values.push_back(funcs[i]->h(p));
    
    unsigned int linear_comb = inner_product(h_values.begin(),h_values.end(),this->r.begin(),0);
    unsigned int chosen = (linear_comb % this->M) % this->TableSize;
    //Store image in chosen bucket
    this->buckets[chosen].push_back(img);
}

hashTable::~hashTable() {
    // //Deallocate Img pointers inside the buckets
    // for(int i = 0; i < this->TableSize; i++) {
    //     for(int j = 0 ; j < this->buckets[i].size(); j++)
    //         delete this->buckets[i][j];
    // }    
}