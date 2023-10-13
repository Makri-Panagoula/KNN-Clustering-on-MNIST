#pragma once
#include <random>
#include <vector>

using namespace std;

class hFunc {
    private:
        vector<double> v;
        int w;
    public :

        hFunc(int w , int d);
        int h(vector<unsigned int> p);
        ~hFunc();

        // double magnitude(vector<double> v);
};