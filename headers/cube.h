#pragma once
#include <stdlib.h>  
#include <vector>  
#include "../headers/hFunc.h"
#include "../headers/img.h"

class Cube {

    private:
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
        //Input images dataset
        vector<Img*> imgs;        
    public:
        Cube(int d,int M,int probes,string inputFile);
        //Maps h result to {0,1}
        int map_function(int h);
        //Gets the pixels vector and maps to a d-vector in {0,1}^d
        vector<int> f(vector<unsigned char>);
        //Finds the n approximate and exact nearest neighbours from query point and updates output file with data and metrics
        void findNearestNeighbors(Img* query,int n,string output);
        //Finds all neighbours in distance R from query point and updates output file with results        
        void rangeSearch(Img* query,int R,string output);    
        ~Cube();
};