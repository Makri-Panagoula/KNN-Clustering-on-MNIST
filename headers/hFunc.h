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
        //Randomized v vector of the function
        vector<double> v;
        //W parameter for function 
        int w;
        //Median of the h set of values(given each v[i] belongs to [0,255])
        int median;
    public :
        hFunc(int w ,int d);
        int get_median(){return this->median;};
        unsigned int h(vector<unsigned char> p);
        ~hFunc(){};
};