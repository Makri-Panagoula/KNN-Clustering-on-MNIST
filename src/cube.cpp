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

int Cube::map_func(int h_value , hFunc* h) {
    return h_value > h->get_median();
}

//Gets the pixels' vector and maps to a d-vector in {0,1}^d
int Cube::f(Img* img) {

    int bucket = 0; 
    //Apply (f o h) function to p d times 
    for(int i = 0; i < this->d; i++) {
        int h_value = hFuncs[i]->h(img->get_p());
        //Since the f(h(p)) contains binary values, we convert it to number by treating it as a binary
        bucket += map_func(h_value,hFuncs[i]) * 2;
    }

}

//Stores img into appropriate bucket
void Cube::store(Img* img) {
    //Save image to its hashed bucket
    this->buckets[f(img)].push_back(img);
}

Cube::~Cube() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < this->d; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;        
}
