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
        vector<pair<Img*, int>> bucket;
        this->buckets.push_back(bucket);
    }
}

pair<int, int> hashTable::g(Img* img) {

    //Accessing pixels' vector
    vector<unsigned int> p = img->get_p();

    //Estimate all the h-values and get the dot product with the stored k r-operands 
    vector<unsigned int> h_values;

    for(int i = 0; i < this->k; i++) 
        h_values.push_back(funcs[i]->h(p));
    
    unsigned int linear_comb = inner_product(h_values.begin(),h_values.end(),this->r.begin(),0);
    int id = linear_comb % this->M;
    unsigned int chosen = id % this->TableSize;
    pair<int, int> to_return;
    to_return.first = id;
    to_return.second = chosen;
    return to_return;
}

void hashTable::store(Img* img) {

    pair<int,int> hashed = this->g(img);
    pair<Img*,int> to_store;
    to_store.first = img;
    to_store.second = hashed.first;
    //Store image in chosen bucket
    this->buckets[hashed.second].push_back(to_store);
}

vector<Img*> hashTable::same_bucket(pair<int, int> query) {
    int buck_num = query.first;
    int query_id = query.second;
    vector<Img*> neighbours;
    //Access vector of wanted bucket
    vector<pair<Img*, int>> bucket = this->buckets[buck_num];
    for(int i = 0; i < bucket.size(); i++) {
        pair<Img*, int> item = bucket[i];
        //Check if item has the same id as query and only in that case consider ing as potential neighbour
        if(item.second == query_id) {
            neighbours.push_back(item.first);
        }
    }
    return neighbours;
}

    
