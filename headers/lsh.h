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
#include "../headers/input.h"

using namespace std;

int LittleEndian (int word);

class LSH {

    private:
        //Number of h-functions out of which every g function will be constructed
        int k;
        //w parameter for h-function
        int w;
        //Number of Hashtables we will use
        int L;
        //Parameter for Querying trick, out of which we will draw datapoint's id
        double M;
        //Number of h-functions we construct and out from we will choose
        int H_size;
        //Number of buckets we choose every Hashtable to have
        int TableSize;
        //Array containing the pointers to each and every Hashtable
        hashTable** hashTables;
        //H set functions
        hFunc** hFuncs;
        //Input images dataset
        Input* imgs;
        //Returns a set with the n approximate neighbours of query point q stored as (distance, image number)
        set <pair<double, int>>  N_Approx(Img* query,int n);
        //Total seconds for all the query runs of LSH algorithm
        double t_lsh;
        //Total seconds for all the query runs of bruteforce algorithm
        double t_true;
    public:
        LSH(int L, int k, Input* input);
        //Finds the n approximate and exact nearest neighbours from query point and updates output file with data and metrics
        void findNearestNeighbors(Img* query,int n,string output);
        //Returns total seconds for all the query runs of LSH algorithm
        double get_tLSH(){return this->t_lsh;};
        //Returns total seconds for all the query runs of bruteforce algorithm
        double get_tTrue(){return this->t_true;};
        ~LSH();
    };