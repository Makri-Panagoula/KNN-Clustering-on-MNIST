#include <iostream>
#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <iostream>
#include <fstream>

using namespace std;

int LittleEndian (int );

class SearchStruct {
public:
    SearchStruct(int L, int k, std::string inputFile);
private:
    int k;
    int w;
    int L;
    int M;
    HashTable* hashTables;
    HFunc* hFunctions;
};