#include "../headers/MRNG.h"

using namespace std;

MRNG::MRNG(int k, int E, int R,int l, Input* imgs) {

    this->E = E;
    this->R = R;
    this->T = 50;
    this->l = l;
    this->imgs = imgs;
}

//Perform Graph Nearest Neighbour Search and return a set<pair<double, int>> holding distance and image number with the results the search found
set<pair<double,int>> MRNG::NearestNeighbour(Img* query) {

    set<pair<double,int>> candidates;

    return candidates;
}

MRNG::~MRNG() {

}