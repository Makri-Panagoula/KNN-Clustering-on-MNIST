#pragma once
#include <random>
#include <vector>
#include <algorithm>
#include <functional> 
#include <cmath> 
#include <numeric>

using namespace std;

class hFunc {
    private:
        //Randomized v vector of the function
        vector<double> v;
        //W parameter for function 
        int w;
    public :

        hFunc(int w ,int d);
        int h(vector<unsigned int> p);
        ~hFunc(){};
};