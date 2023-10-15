#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

class Img {
    private :
        //Image number in dataset
        int num;
        //Vector containing the image's pixels
        vector<char> p;
        //Number of image's pixels
        int pxs;
    public :
        //Initializes fields and reads image as well
        Img(int pxs,int num,ifstream& input);
        double euclideanDistance(Img* b);
        vector<char> get_p(){return p;}
        int imgNum() {  return num; }
        ~Img(){};
};