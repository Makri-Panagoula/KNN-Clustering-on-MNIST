#include "../headers/img.h"
#include <vector>
#include <set>

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
        // Brute-force function to calculate the distance of q to all points in the dataset and return an ordered set containing pairs in the format (distance,img number)
        set <pair<double, int>>  N_Exact(Img* query);        
        ~Input(); 
};