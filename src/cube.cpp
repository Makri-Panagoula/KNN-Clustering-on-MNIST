#include "../headers/cube.h"

Cube::Cube(int d,int M,int probes,Input* input) {

    //Setting algorithm's parameters
    this->d = d;
    this->M = M;
    this->probes = 5;
    this->w = 5;
    this->imgs = imgs;

    //Creating d h_functions (one for every dimension)
    this->hFuncs = new hFunc*[d];
    for(int i = 0; i < d; i++)
        this->hFuncs[i] = new hFunc(w,imgs->get_pxs());  

    //The vertices are essentially our buckets
    double vertices = pow(2.0,d);
    //Create a vector for every bucket
    for(int i = 0; i < vertices; i++) {
        vector<Img*> bucket;
        this->buckets.push_back(bucket);
    }      

    //For every image in the training dataset save it into the appropriate structures
    for(int i = 0; i < input->get_imgs(); i++) 
        store(input->get_image(i));    
}

//Gets the pixels' vector and maps to a d-vector in {0,1}^d
vector<int> Cube::f(Img* img) {

    vector<int> cube_v; 
    //Apply (f o h) function to p d times 
    for(int i = 0; i < this->d; i++) {

    }
}

//Stores img into appropriate bucket
void Cube::store(Img* img) {

    vector<int> project = f(img);
    //Somehow convert to bucket number 
    this->buckets[bucket].push_back(img);
}

Cube::~Cube() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < this->d; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;        
}
