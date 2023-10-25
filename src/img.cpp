#include "../headers/img.h"

Img::Img(int pxs,int num,ifstream& input) {

    this->pxs = pxs;
    this->num = num;
    unsigned char pixel;
    for (int i = 0; i < pxs; i++) {
        input.read((char*)&pixel,1);
        this->p.push_back((unsigned char)pixel);
    }
    this->flag = -1;     //-1 means it's not yet part of any cluster
}

int Img::update_flag(int new_cluster) {
    int changed = new_cluster != this->flag;
    this->flag = new_cluster;
    return changed;
}

double Img::euclideanDistance(Img* img_b) {

    vector<unsigned char> a = this->get_p();
    vector<unsigned char> b = img_b->get_p();
    
    //checking if vectors have same number of elements
    if (a.size() != b.size()) 
        return -1.0;

    double distance = 0.0;
    for (size_t i = 0; i < a.size(); i++) 
        distance += pow(a[i] - b[i], 2);
        
    distance = sqrt(distance);
    return distance;
}

void Img::display_p(ofstream& output) {
    for(int i = 0; i < p.size(); i++)
        output<<(int)p[i]<<" , ";
}
