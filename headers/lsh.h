#pragma once
#include <iostream>
#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <fstream>
#include "../headers/hashTable.h"

using namespace std;

int LittleEndian (int word);

class LSH {

    private:
        int k;
        int w;
        int L;
        int M;
        hashTable** hashTables;
        hFunc** hFuncs;
    public:
        LSH(int L, int k, std::string inputFile);
        ~LSH();
};