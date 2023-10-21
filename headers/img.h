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
        vector<unsigned char> p;
        //Number of image's pixels
        int pxs;
        //Flag -1 for unassigned, 0 for centroid, positive for assigned
        int flag;
    public :
        //Initializes fields and reads image as well
        Img(int pxs,int num, ifstream& input);
        double euclideanDistance(Img* b);
        vector<unsigned char> get_p(){return p;}
        int imgNum() {return num;}
        //Updates cluster where image is assigned and returns true if it has changed
        int update_flag(int new_cluster);
        int get_flag(){return this->flag;}
        void update_p(int i , unsigned char new_val) { this->p[i] = new_val;}
        ~Img(){};
};