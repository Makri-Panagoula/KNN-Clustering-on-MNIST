#pragma once
#include <random>
#include <vector>
#include <algorithm>
#include <functional> 
#include <cmath> 
#include <numeric>
#include <iostream>

using namespace std;

class hFunc {
    private:
        //Randomized single precision v vector of the function
        vector<double> v;
        //W parameter for function 
        int w;
    public:
        hFunc(int w, int d);
        unsigned int h(vector<unsigned char> p);
        void print(); 
        ~hFunc(){};
};