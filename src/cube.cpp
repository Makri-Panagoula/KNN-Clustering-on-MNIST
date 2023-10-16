#include "../headers/cube.h"

Cube::Cube(int d,int M,int probes,string inputFile) {

    //Read image input dataset
    //...
    this->d = d;
    this->M = M;
    this->probes = 5;
    this->w = 5;

    //Creating h_functions
    this->hFuncs = new hFunc*[d];
    for(int i = 0; i < d; i++)
        this->hFuncs[i] = new hFunc(w,this->pxs);    
}

Cube::~Cube() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < this->d; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;        
}
