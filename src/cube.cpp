#include "../headers/cube.h"

Cube::Cube(int d,int M,int probes,Input* input) : hFuncs(d){

    //Setting algorithm's parameters
    this->d = d;
    this->M = M;
    this->probes = probes;
    this->w = 5;
    this->imgs = input;

    //Creating d h_functions (one for every dimension) and d maps (one for every f_i)
    for(int i = 0; i < d; i++) {
        this->hFuncs[i] = new hFunc(w,imgs->get_pxs());  
        map<unsigned int, int> f_value;
        this->f_values.push_back(f_value);
    }

    //The vertices are essentially our buckets
    int vertices = pow(2.0,d);
    this->buckets = new vector<Img*>[vertices];
    //Create a vector for every bucket
    for(int i = 0; i < vertices; i++) {
        vector<Img*> bucket;
        this->buckets[i] = bucket;
    }      

    //For every image in the training dataset save it into the appropriate structures
    for(int i = 0; i < input->get_imgs(); i++) 
        store(input->get_image(i));    
}

void Hamming(string binary,int i,int ham_dist,set<string>& buckets) {
    if (ham_dist == 0) {
        buckets.insert(binary);
        return;
    }
    if (i < 0) return;
    // flip current bit doing xor with 1
    binary[i] ^= 1;
    Hamming(binary, i-1, ham_dist-1, buckets);
    // or don't flip it (flip it again to undo)
    binary[i] ^= 1;
    Hamming(binary, i-1, ham_dist, buckets);
}

//Returns a set holding a pair (distnce,img_number) with the n-approximate neighbours and initializes set r with approximate neighbours in radius r
set <pair<double, int>> Cube::Approx(Img* query, set<pair<double, int>>& r, int range) {

    //Out of all the potential neighbours, we compute euclidean distances, save them in an ascending ordered set and keep the best N ones
    set<pair<double, int>> N_approx;
    //Find query's bucket applying f
    int bucket = f(query);
    //Convert to binary string in order to perform hamming distance
    string binary = bitset<32>(bucket).to_string();   
    //Neighbour vertices from bucket
    set<string> vertices;
    set<string>::iterator vertex;
    //Total datapoints checked
    int datapoints = 0;
    //Total buckets checked    
    int checked_buckets = 0;
    //Check buckets with increasing Hamming distance from 0 (for hashed bucket) to d 
    for(int dist = 0; dist <= d; dist++) { 

        //Check neighbour buckets with increasing hamming distance until you have reached M datapoints or probes neighbour buckets
        Hamming(binary,31,dist,vertices);
        for(vertex = vertices.begin(); vertex!= vertices.end(); vertex++) {

            unsigned long neighbour = stoul(*vertex, nullptr, 2);
            vector<Img*> in_bucket = this->buckets[neighbour];

            //Traverse datapoints of its bucket and add them to N_approx and r set if suitable
            for(int i = 0; i < in_bucket.size(); i++) {

                Img* cur_img = in_bucket[i];
                double distance = query->euclideanDistance(cur_img);
                //save num of image i with the distance
                N_approx.insert(make_pair(distance, cur_img->imgNum()));

                if (distance <= range) 
                    r.insert(make_pair(distance, cur_img->imgNum()));

                //We have examined max datapoints 
                if(++datapoints == this->M)
                    return N_approx;
            }

            //We have examined max neighbour buckets
            if(++checked_buckets == this->probes )
                return N_approx;
        }
    }
    return N_approx;
}

//Finds the n approximate and exact nearest neighbours as well as neighbours in radius R from query point and updates output file with data and metrics
void Cube::queryNeighbours(Img* query,int n,string output,int R) {

    //Get n-approximate,r-approximate neighbours along with time metrics
    const auto start_cube{chrono::steady_clock::now()};

    set<pair<double, int>> r;
    set<pair<double, int>> N_approx = Approx(query, r, R); 

    cout<<N_approx.size()<<endl;

    const auto end_cube{chrono::steady_clock::now()};
    chrono::duration<double> t_cube{end_cube - start_cube};

    //Get n-exact neighbours along with time metrics
    const auto start_true{chrono::steady_clock::now()};

    set<pair<double, int>> N_exact = imgs->N_Exact(query);

    const auto end_true{chrono::steady_clock::now()};
    chrono::duration<double> t_true{end_true - start_true};

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
        outFile<<"Nearest Neighbour-" << i + 1 << " :" << approx->second <<endl<< "distanceHypercube: <double> " << approx->first <<endl<< "distanceTrue: <double> "<< exact->first<<endl;
        approx++;
        exact++;
    }
    outFile<<"tHyperCube: <double> "<<t_cube.count()<<" sec."<<endl<<"tTrue: <double> "<<t_true.count()<<" sec."<<endl<<endl;

    //Write the contents of the set 'r' to the output file
    outFile << "Set r:" << endl;
    for (const auto& entry : r) {
        outFile << "Distance: " << entry.first << " Image Number: " << entry.second << endl;
    }

    outFile.close();    
}

//Gets the pixels' vector and maps to a d-vector in {0,1}^d which then converts to a decimal being its hashed bucket
int Cube::f(Img* img) {

    int bucket = 0;
    int binary;
    map<unsigned int,int>::iterator existing;
    //We need a probability distribution that considers all outcomes equally likely (as in flipping a coin) therefore uniform is the only option
    default_random_engine b_generator;
    b_generator.seed(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0,1);    
    //Apply (f o h)_i function to p d times
    for(int i = 0; i < this->d; i++) {

        unsigned int h_value = hFuncs[i]->h(img->get_p());
        //Check if h value has already been matched with an f value, where in that case for consistency we have to keep that f value
        if((existing = f_values[i].find(h_value)) != f_values[i].end())
            binary = existing->second;
        else {
            //Randomly decide on {0,1}
            binary = distribution(b_generator);
            //Insert new h_value to reference later on
            f_values[i].insert({h_value,binary});
        }
        //Since the f(h(p)) contains binary values, we convert it to number by treating it as a binary
        bucket += binary * 2;
    }
    return bucket;
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
    
    delete[] this->buckets;
}
