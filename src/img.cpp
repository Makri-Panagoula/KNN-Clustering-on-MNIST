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
