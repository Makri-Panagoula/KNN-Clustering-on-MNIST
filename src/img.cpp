#include "../headers/img.h"

Img::Img(int pxs,int num,ifstream& input) {

    this->pxs = pxs;
    this->num = num;
    char pixel;
    for (int i = 0; i < pxs; i++) {
        input.read((char*)&pixel,1);
        this->p.push_back(pixel);
    }
}

double Img::euclideanDistance(Img* img_b) {

    vector<unsigned int> a = this->get_p();
    vector<unsigned int> b = img_b->get_p();

    if (a.size() != b.size()) {
        //checking if vectors have same number of elements
        return -1.0;
    }

    double distance = 0.0;
    for (size_t i = 0; i < a.size(); i++) {
        distance += pow(a[i] - b[i], 2);
    }
    cout<<endl<<sqrt(distance)<<endl;
    return sqrt(distance);
}