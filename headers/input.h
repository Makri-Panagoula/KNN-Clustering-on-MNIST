#include "../headers/img.h"
#include <vector>

class Input {
    private:
        //Pixels per image
        int pxs;
        //Input images dataset
        vector<Img*> imgs;
        //Number of images in dataset  
        int img_count;
    public:
        Input(string inputFile);
        //Returns how many pixels an image has
        int get_pxs(){ return this->pxs;};
        //Returns how many images are in input dataset
        int get_imgs(){ return this->img_count;}   
        //Returns img with image num i
        Img* get_image(int i){ return this->imgs[i];}
        ~Input(); 
};