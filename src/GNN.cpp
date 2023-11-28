#include "../headers/GNN.h"

using namespace std;

GNN::GNN(int k, int E, int R, Input* imgs) {

    this->E = E;
    this->R = R;
    this->T = 50;
    this->imgs = imgs;
    //For every image we will create a vector with its k nearest neighbours(if less as many as available)
    this->Graph = new vector<Img*>[imgs->get_imgs()];
    //Create LSH Structure(we consider 3 hash functions and 5 Hashtables sacrificing accuracy but gaining speed)
    LSH* lsh = new LSH(3,5,imgs);
    //Perform query search for k neighbours with each image as query
    for(int i = 0; i < imgs->get_imgs(); i++) {
        Img* query = imgs->get_image(i);
        this->Graph[i] = lsh->NearestNeighbours(k,query);
    }
    //Let go of lsh structure we won't need it any more
    delete lsh;
}

//Perform Graph Nearest Neighbour Search and write in the output file the neighbours and metrics
set <pair<double,int>> GNN::NearestNeighbour(Img* query) {

    set<pair<double,int>> candidates;
    
    //We need a probability distribution that considers all outcomes equally likely (as in flipping a coin) therefore uniform is the only option
    default_random_engine i_generator;
    i_generator.seed(chrono::system_clock::now().time_since_epoch().count());
    int images = this->imgs->get_imgs();
    uniform_int_distribution<int> distribution(0,images-1);   

    //For every rerun
    for(int r = 1; r <= this->R; r++) {
        //Choose with random uniform distribution the navigation node
        Img* candidate = imgs->get_image(distribution(i_generator));     
        //For every greedy step   
        for(int t = 1; t <= this->T; t++) {
            //Neighbours of candidate
            vector<Img*> neighbours = this->Graph[candidate->imgNum()];         
            double min_dist = numeric_limits<double>::max(); 
            Img* next_candidate = NULL;  
            //Take the first E neighbors or less if there aren't enough
            int maxNeighbors = min((int)neighbours.size(), E); 
            //For every neighbour of the candidate calculate its distance from query node
            for(int n = 0 ; n < maxNeighbors; n++) {
                Img* neigh = neighbours[n];
                double query_dist = query->euclideanDistance(neigh);
                candidates.insert(make_pair(query_dist, neigh->imgNum()));
                //Next candidate will be the neighbour of current node nearest to query
                if(query_dist < min_dist) {
                    min_dist = query_dist;
                    next_candidate = neigh;
                }
            }
            //If current node didn't have any neighbours 
            if(next_candidate == NULL)
                return candidates;

            candidate = next_candidate;
        }
    }
    return candidates;
}

GNN::~GNN() {
    delete[] this->Graph;
}