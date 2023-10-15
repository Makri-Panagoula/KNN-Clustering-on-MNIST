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
        hFunc** hFuncs;
        vector<Img*> imgs;
    public:
        LSH(int L, int k, std::string inputFile);
        int findNearestNeighbors(Img* query,int n,string output);
        int get_pxs() { return this->pxs;};
        ~LSH();
};