#include "../headers/input.h"

//Swaps endian-ness since MNIST is in big endian architecture in contrast with our system 
int LittleEndian (int i) {

    unsigned char b1 = i & 0x000000FF;
    unsigned char b2 = (i & 0x00FF0000) >> 8;
    unsigned char b3 = (i & 0x00FF0000) >> 16;
    unsigned char b4 = (i & 0xFF000000) >> 24;
    return (b1<<24) | (b2<<16) | (b3<<8) | b4;
}

Input::Input(string inputFile) {

    ifstream input(inputFile, ios::binary | ios::in);
    if(! input.is_open()) {
        cout << "Failed to read input dataset file!" << endl;
        exit;
    }

    //Processsing the first four words of the input file that contain general info about the dataset
    int magic_num = 0;
    int imgs = 0;
    int rows = 0;    
    int cols = 0;        
    input.read((char*)&magic_num,4);
    magic_num = LittleEndian(magic_num);
    input.read((char*)&imgs,4);
    imgs = 1000;
    this->img_count = imgs;    
    input.read((char*)&rows,4);
    rows = LittleEndian(rows);    
    input.read((char*)&cols,4);
    cols = LittleEndian(cols);  
    this->pxs = rows*cols;

    //For every image in the training dataset we store it in a vector
    for(int i = 0; i < imgs; i++) {
        Img* img = new Img(this->pxs,i,input);
        this->imgs.push_back(img);    
    }
    //We have processed input dataset, we won't need it anymore
    input.close();
}

// Brute-force function to calculate the distance of q to all points in the dataset and return an ordered set containing pairs in the format (distance,img number)
set <pair<double, int>>  Input::N_Exact(Img* query) {

    set<pair<double, int>> distances;

    for (int i = 0; i < img_count; i++) {
        Img* img = imgs[i];
        double distance = query->euclideanDistance(img);
        distances.insert(make_pair(distance, img->imgNum()));
    }

    return distances;    
}

Input::~Input() {
    //Release dynamically allocated memory
    for(int i = 0; i < this->img_count; i++)
        delete this->imgs[i];    
}