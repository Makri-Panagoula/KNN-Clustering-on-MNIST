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
        //Flag -1 when created
        int flag;
    public :
        //Initializes fields and reads image as well
        Img(int pxs, int num, ifstream& input);
        //Constructor used for MRNG
        Img(int pxs);
        double euclideanDistance(Img* b);
        vector<unsigned char> get_p(){return p;}
        int imgNum() {return num;}
        //Updates flag and returns true if it has changed
        int update_flag(int new_flag);
        //Returns image flag : -1 for unassigned, otherwise holds cluster's number (starting from 0)
        int get_flag(){return this->flag;}
        void update_p(int i, unsigned char new_val) { this->p[i] = new_val;}
        void display_p(ofstream& output);
        //Returns True if edge is pr the longest in the triangle prt (considering p image from which called)
        bool longestEdge(Img* t, Img* r);
        ~Img(){};
};