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
    this->w = 4;
    this->L = L;
    this->M = 1;
    this->TableSize = imgs / 8;

    //Creating the data structures
    this->hFuncs = new hFunc*[2*k];
    for(int i = 0; i < 2*k; i++)
        this->hFuncs[i] = new hFunc(w,pixs);
    
    this->hashTables = new hashTable*[L];
    for(int i = 0; i < L; i++)
        this->hashTables[i] = new hashTable(k,2*k,this->hFuncs,TableSize,M);
    
    //For every image in the training dataset we read
    for(int i = 0; i < imgs; i++) {
        Img* img = new Img(pixs,i,input);
        this->imgs.push_back(img);
        //For every hashtable we save it into the proper bucket
        for(int j = 0; j < L ; j++) 
            hashTables[j]->store(img);
    }
    
    //We have processed input dataset, we won't need it anymore
    input.close();
}

int LSH::findNearestNeighbors(Img* query,int n,string output){

    //Images where we will compute the distance,aka possible neighbours out of all the buckets
    vector<Img*> neighbours;

    //For every hashtable find query's bucket(without saving)
    for(int i = 0; i < L; i++) {
        pair<int,int> hashed = hashTables[i]->g(query);
        //Get a vector of images in the same bucket as the query that have the same id as well
        vector<Img*> in_bucket = hashTables[i]->same_bucket(hashed);
        //Concatenate with larger vector 
        neighbours.insert(neighbours.end(), in_bucket.begin(), in_bucket.end());
    }

    ofstream outFile(output);
    if (!outFile.is_open()) {
        cout << "Failed to open the output file." << endl;
        exit;
    }    
    outFile << "Query: "<<query->imgNum()<<endl;

    priority_queue<pair<double, int>> minHeap;
    //Out of all the potential neighbours, we compute euclidean distances, save them in a min heap and keep the best N ones
    for (int i = 0; i < neighbours.size(); i++) {
        Img* cur_img = neighbours[i];
        double distance = query->euclideanDistance(cur_img);
        //save num of image i with the distance
        minHeap.push(make_pair(distance, cur_img->imgNum()));
    }
    cout<<neighbours.size()<<endl;
    //takes the first n neighbors or less if there aren't enough
    int maxNeighbors = min((int)neighbours.size(), n); 
    pair<int,int> n_neigh;

    for (int i = 1; i <= maxNeighbors; ++i) {
        n_neigh = minHeap.top();
        outFile<<"Nearest Neighbour-" << i << " :" << n_neigh.second <<endl<< "distanceLSH: <double> " << n_neigh.first <<endl<< "distanceTrue: <double> "<<endl;        
        minHeap.pop();
    }
    outFile.close();
    return 0;
}

LSH::~LSH() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < 2 * this->k; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;    

    for(int i = 0; i < this->imgs.size(); i++)
        delete this->imgs[i]; 

    for (int i = 0; i < this->L; i++) 
        delete this->hashTables[i];
    
    delete[] this->hashTables;    
}