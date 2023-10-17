#include "../headers/cube.h"

Cube::Cube(int d,int M,int probes,Input* input) {

    //Setting algorithm's parameters
    this->d = d;
    this->M = M;
    this->probes = 5;
    this->w = 5;
    this->imgs = input;
    this->t_cube = 0;
    this->t_true = 0;

    //Creating d h_functions (one for every dimension)
    this->hFuncs = new hFunc*[d];
    for(int i = 0; i < d; i++)
        this->hFuncs[i] = new hFunc(w,imgs->get_pxs());  

    //The vertices are essentially our buckets
    double vertices = pow(2.0,d);
    //Create a vector for every bucket
    for(int i = 0; i < vertices; i++) {
        vector<Img*> bucket;
        this->buckets.push_back(bucket);
    }      

    //For every image in the training dataset save it into the appropriate structures
    for(int i = 0; i < input->get_imgs(); i++) 
        store(input->get_image(i));    
}

int Cube::map_func(int h_value , hFunc* h) {
}

//Finds the n approximate and exact nearest neighbours as well as neighbours in radius R from query point and updates output file with data and metrics
void Cube::queryNeighbours(Img* query,int n,string output,int R) {

    //Get n-approximate,r-approximate neighbours along with time metrics
    time_t startCube;
    time(&startCube);

    set<pair<double, int>> r;
    set<pair<double, int>> N_approx = Approx(query, n, r, R); 

    cout<<N_approx.size()<<endl;

    time_t endCube;
    time(&endCube);
    this->t_cube += endCube - startCube;

    //Get n-exact neighbours along with time metrics
    time_t start_exact;
    time(&start_exact);

    set<pair<double, int>> N_exact = imgs->N_Exact(query);

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

    // Write the contents of the set 'r' to the output file
    outFile << "Set r:" << endl;
    for (const auto& entry : r) {
        outFile << "Distance: " << entry.first << " Image Number: " << entry.second << endl;
    }

    outFile.close();    
}

//Gets the pixels' vector and maps to a d-vector in {0,1}^d
int Cube::f(Img* img) {

    int bucket = 0; 
    //Apply (f o h) function to p d times 
    for(int i = 0; i < this->d; i++) {
        int h_value = hFuncs[i]->h(img->get_p());
        //Since the f(h(p)) contains binary values, we convert it to number by treating it as a binary
        bucket += map_func(h_value,hFuncs[i]) * 2;
    }

}

//Stores img into appropriate bucket
void Cube::store(Img* img) {
    //Save image to its hashed bucket
    this->buckets[f(img)].push_back(img);
}

Cube::~Cube() {

    // Deallocate dynamically stored memory
    for (int i = 0; i < this->d; i++) 
        delete this->hFuncs[i];
    
    delete[] this->hFuncs;        
}
