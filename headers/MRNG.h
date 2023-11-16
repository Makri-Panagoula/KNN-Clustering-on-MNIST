#pragma once
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <fstream>
#include <set>
#include <chrono>
#include <random>
#include <vector>
#include "../headers/input.h"
#include "../headers/lsh.h"
#include "../headers/cube.h"

class MRNG {

    private:
        //An array of vectors where each cell i holds the k-Nearest Neighbours of the i-th image
        vector<Img*> *Graph;    
        //Number of candidates 
        int l;
        //Image Dataset
        Input* imgs;
        //Navigating Node
        Img* nav_node;

    public:
        MRNG(int l, Input* imgs);
        //Perform Graph Nearest Neighbour Search and return a set<pair<double, int>> holding distance and image number with the results the search found
        set<pair<double,int>> NearestNeighbour(Img* query);
        ~MRNG();
};