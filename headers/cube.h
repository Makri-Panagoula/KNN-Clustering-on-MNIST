#pragma once
#include <stdlib.h>  
#include <vector>  
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
        //H set functions
        hFunc** hFuncs;
        //A vector consisting of 2^d vectors,one for each bucket holding the images hashed to it
        vector<vector<Img*>> buckets;
        //Maps h result to {0,1}
        int map_func(int h);           
    public:
        Cube(int d,int M,int probes,Input* imgs);
        //Gets the pixels' vector and hashes tp bucket
        int f(Img* img);
        //stores img into appropriate bucket
        void store(Img* img);
        //Finds the n approximate and exact nearest neighbours from query point and updates output file with data and metrics
        void findNearestNeighbors(Img* query,int n,string output);  
        ~Cube();
};