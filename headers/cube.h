#pragma once
#include <stdlib.h>  
#include <vector> 
#include <map> 
#include <bitset>  
#include <chrono>
#include <string>
#include "../headers/hFunc.h"
#include "../headers/input.h"

class Cube {
    private:
        //Input images dataset
        Input* imgs;     
        //Maximum number of datapoints to be checked
        int M;
        //Maximum number of vertices to be checked
        int probes;
        //Dimension upon which data will be projected        
        int d;
        //Parameter for h functions
        int w; 
        //d maps of h values -> {0,1} , one for every f_i in order to ensure constistency
        vector<map<unsigned int, int>> f_values;
        //H set functions
        vector <hFunc*> hFuncs;
        //An array consisting of 2^d vectors,one for each bucket holding the images hashed to it
        vector<Img*> *buckets;         
    public:
        Cube(int d,int M,int probes,Input* imgs);
        //Gets the pixels' vector and hashes to a bucket
        int f(Img* img);
        //stores img into appropriate bucket
        void store(Img* img);
        //Returns a set holding a pair (distnce,img_number) with the n-approximate neighbours and initializes set r with approximate neighbours in radius r
        set <pair<double, int>>  Approx(Img* query, set<pair<double, int>>& r, int range);             
        //Finds the n approximate and exact nearest neighbours as well as neighbours in radius R from query point and updates output file with data and metrics
        void queryNeighbours(Img* query,int n,string output,int R);    
        ~Cube();
};