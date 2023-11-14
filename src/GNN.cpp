#include "../headers/GNN.h"


GNN::GNN(int k , int E, int R ,Input* imgs) {

    this->E = E;
    this->R = R;
    this->T = 50;
    //For every image we will create a vector with its k nearest neighbours(if less as many as available)
    this->Graph = new vector<Img*>[imgs->get_imgs()];
    //Create LSH Structure(we consider 3 hash functions and 5 Hashtables sacrificing accuracy but gaining speed)
    LSH* lsh = new LSH(3,5,imgs);
    //Perform query search for k neighbours with each image as query
    for(int i = 0; i < imgs->get_imgs(); i++) {

        Img* query = imgs->get_image(i);
        this->Graph[i] = lsh->NearestNeighbours(k,query);
    }
}

//Perform Graph Nearest Neighbour Search and write in the output file the neighbours and metrics
void GNN::NearestNeighbour(Img* query,int N,string output_file) {

}

GNN::~GNN() {
    delete[] this->Graph;
}