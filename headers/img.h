#pragma once
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Img {
    private :
        //Image number in dataset
        int num;
        //Vector containing the image's pixels
        vector<unsigned int> p;
        //Number of image's pixels
        int pxs;
    public :
        //Initializes fields and reads image as well
        Img(int pxs,int num,ifstream& input);
        vector<unsigned int> get_p(){   return p;}
        int imgNum() {  return num; }
        ~Img();
};