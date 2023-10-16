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
        //Gets the result of h functions and maps to {0,1}
        int f(unsigned int);
        //Finds the n approximate and exact nearest neighbours from query point and updates output file with data and metrics
        void findNearestNeighbors(Img* query,int n,string output);
        //Finds all neighbours in distance R from query point and updates output file with results        
        void rangeSearch(Img* query,int R,string output);    
        ~Cube();
};