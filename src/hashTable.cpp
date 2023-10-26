#include "../headers/hashTable.h"

using namespace std;

hashTable::hashTable(int k , int H_size, vector <hFunc*>& H, int TableSize,int M) : r(k) , funcs(k) {

    this->k = k;
    this->TableSize = TableSize;
    this->M = M;    
    //Initialize uniform distributor
    default_random_engine b_generator;
    b_generator.seed(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0,H_size-1); 
    //Initialize random seed
    srand (time(NULL));    
    //Choose uniformly the k functions from H that will be used for g and randomly the r-operands
    for(int i = 0; i < k ; i++) {
        int chosen = distribution(b_generator);
        this->funcs[i] = H[chosen];
        this->r[i] = rand() % M;
    }

    //Create a vector for every bucket
    for(int i = 0; i < TableSize; i++) {
        vector<pair<Img*, unsigned int>> bucket;
        this->buckets.push_back(bucket);
    }
}

pair<unsigned int , unsigned int> hashTable::g(Img* img) {

    //Accessing pixels' vector
    vector<unsigned char> p = img->get_p();
    unsigned int dot_prod = 0;

    //To avoid overflow we use the slides' operations on modulo ( (a [] b) mod m = ( a mod m  [] b mod m ) mod m ) )
    for(int i = 0; i < this->k; i++) {
        unsigned int h_value = this->funcs[i]->h(p) % this->M;
        //r_i is already taken up to M
        dot_prod += (h_value * r[i]) % this->M;
    }
    //exit(1);

    pair<unsigned int, unsigned int> to_return;
    to_return.first = dot_prod % this->M;                       //Id
    to_return.second = to_return.first % this->TableSize;       //Bucket
    return to_return;
}

void hashTable::store(Img* img) {

    pair<unsigned int,unsigned int> hashed = this->g(img);
    pair<Img*,unsigned int> to_store;
    to_store.first = img;
    to_store.second = hashed.first;
    //Store image in chosen bucket
    this->buckets[hashed.second].push_back(to_store);
}

vector<Img*> hashTable::same_bucket(pair<unsigned int, unsigned int> query) {
    int query_id = query.first;
    int buck_num = query.second;
    vector<Img*> neighbours;
    //Access vector of wanted bucket
    vector<pair<Img*, unsigned int>> bucket = this->buckets[buck_num];
    for(int i = 0; i < bucket.size(); i++) {
        pair<Img*, int> item = bucket[i];
        //Check if item has the same id as query and only in that case consider ing as potential neighbour
        if(item.second == query_id) {
            neighbours.push_back(item.first);
        }
    }
    return neighbours;
}