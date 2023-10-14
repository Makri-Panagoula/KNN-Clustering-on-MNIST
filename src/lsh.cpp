#include "../headers/lsh.h"

//Swaps endian-ness since MNIST is in big endian architecture in contrast with our system 
int LittleEndian (int i) {

    unsigned char b1 = i & 0x000000FF;
    unsigned char b2 = (i & 0x00FF0000) >> 8;
    unsigned char b3 = (i & 0x00FF0000) >> 16;
    unsigned char b4 = (i & 0xFF000000) >> 24;
    return (b1<<24) | (b2<<16) | (b3<<8) | b4;
}

LSH::LSH(int L,int k,string inputFile) {

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
    imgs = LittleEndian(imgs);    
    input.read((char*)&rows,4);
    rows = LittleEndian(rows);    
    input.read((char*)&cols,4);
    cols = LittleEndian(cols);  
    int pixs = rows*cols;

    //Setting algorithm's parameters
    this->k = k;
    this->w = 50;
    this->L = L;
    this->M = imgs / 8;

    //Creating the data structures
    this->hFuncs = new hFunc*[2*k];
    for(int i = 0; i < 2*k; i++)
        this->hFuncs[i] = new hFunc(w,pixs);
    
    this->hashTables = new hashTable*[L];
    for(int i = 0; i < L; i++)
        this->hashTables[i] = new hashTable(k,2*k,this->hFuncs,M);
    
    //For every image in the training dataset we read
    for(int i = 0; i < imgs; i++) {
        Img* img = new Img(pixs,i,input);
        //For every hashtable we save it into the proper bucket
        for(int i = 0; i < L ; i++) 
            hashTables[i]->g(img);
    }
    
    //We have processed input dataset, we won't need it anymore
    input.close();
}

LSH::~LSH() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < 2 * this->k; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;     

    for (int i = 0; i < this->L; i++) 
        delete this->hashTables[i];
    
    delete[] this->hashTables;    
}