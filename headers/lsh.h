#pragma once
#include <iostream>
#include <chrono>
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
        vector<hashTable*> hashTables;
        //H set functions
        vector<hFunc*> hFuncs;
        //Input images dataset
        Input* imgs;

    public:
        LSH(int L, int k, Input* input);
        //Finds the n approximate and exact nearest neighbours as well as neighbours in radius R from query point and updates output file with data and metrics
        void queryNeighbours(Img* query, int n, string output, int R);
        //Returns a set holding a pair (distnce,img_number) all the neighbours of query point and initializes set r with approximate neighbours in radius r
        set <pair<double, int>> Approx(Img* query, set<pair<double, int>>& r, int radius);        
        ~LSH();
    };