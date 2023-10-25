#include "../headers/lsh.h"


LSH::LSH(int L,int k,Input* input) : hashTables(L){

    //Setting algorithm's parameters
    this->k = k;
    this->w = 5;
    this->L = L;
    this->M = pow(2.0,12.0) - 5;
    this->TableSize = input->get_imgs() / 4;
    this->H_size = 30*k;
    this->imgs = input;

    //Creating the data structures
    for(int i = 0; i < this->H_size; i++)  {
        hFunc* h_i = new hFunc(w,input->get_pxs());
        this->hFuncs.push_back(h_i);
    }

    for(int i = 0; i < L; i++)
        this->hashTables[i] = new hashTable(k,this->H_size,this->hFuncs,TableSize,M);
    
    //For every image in the training dataset save it into the appropriate structures
    for(int i = 0; i < input->get_imgs(); i++) {
        
        //For every hashtable we save it into the proper bucket
        for(int j = 0; j < L ; j++) 
            this->hashTables[j]->store(input->get_image(i));
    }
}


//Returns a set holding a pair (distnce,img_number) all the neighbours of query point and initializes set r with approximate neighbours in radius r
set <pair<double, int>> LSH::Approx(Img* query, set<pair<double, int>>& r, int range) {

    //Out of all the potential neighbours, we compute euclidean distances, save them in an ascending ordered set and keep the best N ones
    set<pair<double, int>> N_approx;
    //For every hashtable find query's bucket(without saving)
    for(int i = 0; i < L; i++) {
        pair<unsigned int,unsigned int> hashed = hashTables[i]->g(query);
        //Get a vector of images in the same bucket as the query that have the same id as well
        vector<Img*> in_bucket = hashTables[i]->same_bucket(hashed);
        //Compute distance for every neighbour image in the same bucket and save alon with img number in the set
        for(int j = 0; j < in_bucket.size(); j++) {
            Img* cur_img = in_bucket[j];
            double distance = query->euclideanDistance(cur_img);
            //save num of image i with the distance
            N_approx.insert(make_pair(distance, cur_img->imgNum()));
            if (distance <= range) {
                r.insert(make_pair(distance, cur_img->imgNum()));
            }
        }
    }
    return N_approx;
}

//Finds the n approximate and exact nearest neighbours as well as neighbours in radius R from query point and updates output file with data and metrics
void LSH::queryNeighbours(Img* query, int n, string output, int R){
    //Get n-approximate,r-approximate neighbours along with time metrics
    const auto start_LSH{chrono::steady_clock::now()};

    set<pair<double, int>> r;
    set<pair<double, int>> N_approx = Approx(query, r, R);

    const auto end_LSH{chrono::steady_clock::now()};
    chrono::duration<double> t_lsh{end_LSH - start_LSH};
    
    //Get n-exact neighbours along with time metrics
    const auto start_exact{chrono::steady_clock::now()};

    set<pair<double, int>> N_exact = imgs->N_Exact(query);

    const auto end_exact{chrono::steady_clock::now()};
    chrono::duration<double> t_true{end_exact - start_exact};

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
    outFile<<"tLSH: <double> "<< t_lsh.count()<<" sec."<<endl<<"tTrue: <double> "<<t_true.count()<<" sec."<<endl<<endl;

    // Write the contents of the set 'r' to the output file
    outFile << "Set r:" << endl;
    for (const auto& entry : r) {
        outFile << "Distance: " << entry.first << " Image Number: " << entry.second << endl;
    }

    outFile.close();
}

LSH::~LSH() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < this->H_size; i++) 
        delete this->hFuncs[i];
    
    for (int i = 0; i < this->L; i++) 
        delete this->hashTables[i];
}