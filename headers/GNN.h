#pragma once
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <fstream>
#include <vector>
#include "../headers/input.h"
#include "../headers/lsh.h"
#include "../headers/cube.h"

class GNN {

    private:
        //An array of vectors where each cell i holds the k-Nearest Neighbours of the i-th image
        vector<Img*> *Graph;    
        //Extensions
        int E;               
        //Random Restarts
        int R ; 
        //Greedy Steps
        int T;

    public:
        GNN(int k , int E, int R ,Input* imgs);
        //Perform Graph Nearest Neighbour Search and write in the output file the neighbours and metrics
        void NearestNeighbour(Img* query,int N,string output_file);
        ~GNN();
};