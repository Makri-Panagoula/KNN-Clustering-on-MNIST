#include <stdio.h>
#include <string> 
#include <cstring>
#include <stdlib.h> 
#include <unistd.h>
#include <sstream>
#include "headers/lsh.h"
#include "headers/cluster.h"
#include <map>

using namespace std;

int main (int argc, char* argv[]) {

    string input_file = argv[2];
    
    //Read cluster parameters from configuration file
    ifstream conf(argv[4]);
    if (!conf.is_open()) {
        cout<< "Failed to open cluster.conf" <<endl;
        exit;
    }     

    //initializing map with default parameters
    map<string, int> parameters = {
        {"number_of_clusters:", 0},
        {"number_of_vector_hash_tables:", 3},
        {"number_of_vector_hash_functions:", 4},
        {"max_number_M_hypercube:", 10},
        {"number_of_hypercube_dimensions:", 3},
        {"number_of_probes:", 2}
    };

    string line;
    string paramName;
    int paramValue;   
    while (getline(conf, line)) {
        istringstream iss(line);
        //Parse string into the parameter name and value
        if (iss>>paramName>>paramValue) 
            parameters[paramName] = paramValue;
    }
    conf.close(); 

    string output_file  = argv[6];
    
    //If complete parameter isn't given as a command argument is by default false
    int complete = 0;               
    if ( strcmp(argv[7],"-m")) {
        complete = 1;
        argv++;                     
    }    

    //Read image input dataset
    Input* imgs = new Input(input_file);
    int clusters = parameters["number_of_clusters:"];
    //Initialization of centroids
    vector<Img*> centroids = kmeans_init(imgs, clusters);

    int changed;
    do {
        //Number of clusters whose centroids have changed
        changed = 0;
        //For every datapoint , assign it to closest centroid and if datapoint updated , update centroid
        for(int i = 0 ; i < imgs->get_imgs(); i++) {

            Img* point = imgs->get_image(i);
            int prev_cluster = point->get_flag();
            //Find closest cluster according to method asked
            int closest = find_cluster(point, centroids);
            if(point->update_flag(closest)) {
                //Update centroids in previous cluster(if it wasn't -1) and closest
            }
        }

    } while (changed < 2); 
    
    //Create Search Structure where we are only gonna be saving the centroids
    //(since we want to find the nearest neighbour out of them and non-centroid point in its iteration)
    if (!strcmp(argv[8],"Classic") ) {

    }
    else if(!strcmp(argv[8],"LSH") ) {

    }
    else if(!strcmp(argv[8],"Hypercube") ) {

    }    

    
    delete imgs;
    return 0;
}