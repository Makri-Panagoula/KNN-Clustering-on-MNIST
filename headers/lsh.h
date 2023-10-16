#pragma once
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <fstream>
#include <queue>
#include <cmath>
#include <set>
#include <ctime>
#include <utility>
#include <bits/stdc++.h> 
#include "../headers/hashTable.h"
#include "../headers/img.h"

using namespace std;

int LittleEndian (int word);

class LSH {

    private:
        int k;
        int w;
        int L;
        double M;
        int pxs;
        //Number of h-functions we construct and out from we will choose
        int H_size;
        int TableSize;
        hashTable** hashTables;
        //H set functions
        hFunc** hFuncs;
        //Input images dataset
        vector<Img*> imgs;
        set <pair<double, int>>  N_Approx(Img* query,int n);
        double t_lsh;
        double t_true;
    public:
        LSH(int L, int k, string inputFile);
        //Finds the n approximate and exact nearest neighbours from query point and updates output file with data and metrics
        void findNearestNeighbors(Img* query,int n,string output);
        //Finds all neighbours in distance R from query point and updates output file with results        
        void rangeSearch(Img* query,int R,string output);
        int get_pxs() { return this->pxs;};
        //Returns total seconds for all the query runs of LSH algorithm
        double get_tLSH(){return this->t_lsh;};
        //Returns total seconds for all the query runs of bruteforce algorithm
        double get_tTrue(){return this->t_true;};
        ~LSH();
    };