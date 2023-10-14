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
        int TableSize;
        hashTable** hashTables;
        hFunc** hFuncs;
        vector<Img*> imgs;
    public:
        LSH(int L, int k, std::string inputFile);
        int findNearestNeighbors(Img* query,int n);
        int get_pxs() { return this->pxs;};
        ~LSH();
};