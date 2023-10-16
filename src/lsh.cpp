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
    this->pxs = rows*cols;

    //Setting algorithm's parameters
    this->k = k;
    this->w = 5;
    this->L = L;
    this->M = pow(2.0,32.0) - 5;
    this->TableSize = imgs / 4;
    this->H_size = 30*k;
    this->t_lsh = 0;
    this->t_true = 0;

    //Creating the data structures
    this->hFuncs = new hFunc*[this->H_size];
    for(int i = 0; i < this->H_size; i++)
        this->hFuncs[i] = new hFunc(w,this->pxs);
    
    this->hashTables = new hashTable*[L];
    for(int i = 0; i < L; i++)
        this->hashTables[i] = new hashTable(k,this->H_size,this->hFuncs,TableSize,M);
    
    //For every image in the training dataset we store it in a vector
    for(int i = 0; i < imgs; i++) {
        Img* img = new Img(this->pxs,i,input);
        this->imgs.push_back(img);
        //For every hashtable we save it into the proper bucket
        for(int j = 0; j < L ; j++) 
            hashTables[j]->store(img);
    }
    
    //We have processed input dataset, we won't need it anymore
    input.close();
}

// Brute-force function to calculate the distance of q to all points in the dataset and return an ordered set containing pairs in the format (distance,img number)
set <pair<double, int>>  N_Exact(Img* query, const vector<Img*>& imgs) {

    set<pair<double, int>> distances;

    for (int i = 0; i < imgs.size(); i++) {
        double distance = query->euclideanDistance(imgs[i]);
        distances.insert(make_pair(distance, imgs[i]->imgNum()));
    }

    return distances;    
}

//Returns a set holding a pair (distnce,img_number) with the n-approximate neighbours
set <pair<double, int>>  LSH::N_Approx(Img* query,int n) {

    //Out of all the potential neighbours, we compute euclidean distances, save them in an ascending ordered set and keep the best N ones
    set<pair<double, int>> N_approx;
    //For every hashtable find query's bucket(without saving)
    for(int i = 0; i < L; i++) {
        pair<int,int> hashed = hashTables[i]->g(query);
        //Get a vector of images in the same bucket as the query that have the same id as well
        vector<Img*> in_bucket = hashTables[i]->same_bucket(hashed);
        //Compute distance for every neighbour image in the same bucket and save alon with img number in the set
        for(int j = 0; j < in_bucket.size(); j++) {
            Img* cur_img = in_bucket[j];
            double distance = query->euclideanDistance(cur_img);
            //save num of image i with the distance
            N_approx.insert(make_pair(distance, cur_img->imgNum()));
        }
    }
    return N_approx;
}

void LSH::findNearestNeighbors(Img* query,int n,string output){

    //Get n-approximate neighbours along with time metrics
    time_t start_LSH;
    time(&start_LSH);

    set<pair<double, int>> N_approx = N_Approx(query,n); 

    cout<<N_approx.size()<<endl;

    time_t end_LSH;
    time(&end_LSH);
    this->t_lsh += end_LSH - start_LSH;

    //Get n-exact neighbours along with time metrics
    time_t start_exact;
    time(&start_exact);

    set<pair<double, int>> N_exact = N_Exact(query,this->imgs);

    time_t end_exact;
    time(&end_exact);
    this->t_true += end_exact - start_exact;

    ofstream outFile(output, ios::app);
    if (!outFile.is_open()) {
        cout << "Failed to open the output file." << endl;
        exit;
    }    

    outFile << "Query: "<<query->imgNum()<<endl;
   
    //takes the first n neighbors or less if there aren't enough
    int maxNeighbors = min((int)N_approx.size(), n); 
    auto approx = N_approx.begin();
    auto exact = N_exact.begin();

    //Iterate through sets and write in output file
    for (int i = 0; i < maxNeighbors; i++) { 
        outFile<<"Nearest Neighbour-" << i + 1 << " :" << approx->second <<endl<< "distanceLSH: <double> " << approx->first <<endl<< "distanceTrue: <double> "<< exact->first<<endl;
        approx++;
        exact++;
    }

    outFile.close();
}

//Finds all neighbours in distance R from query point and updates output file with results        
void LSH::rangeSearch(Img* query,int R,string output) {
    
}

LSH::~LSH() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < this->H_size; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;    

    for(int i = 0; i < this->imgs.size(); i++)
        delete this->imgs[i]; 

    for (int i = 0; i < this->L; i++) 
        delete this->hashTables[i];
    
    delete[] this->hashTables;    
}