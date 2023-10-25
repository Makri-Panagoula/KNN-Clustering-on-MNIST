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
    ofstream outFile(output_file); 

    //Read image input dataset
    Input* imgs = new Input(input_file);
    int k_clusters = parameters["number_of_clusters:"];
    //Initialization of centroids
    vector<Img*> centroids = kmeans_init(imgs, k_clusters,outFile);

    vector<Cluster*> clusters;
    //Initialize clusters with centroid
    for(int i = 0; i < k_clusters; i++) {
        Cluster* cluster = new Cluster(centroids[i],i);
        clusters.push_back(cluster);
    }

    int changed;
    int closest;
    //Keep track of starting time
    const auto start_cluster{chrono::steady_clock::now()};

    do {
        //Number of clusters whose centroids have changed
        changed = 0;
        //For every datapoint , assign it to closest centroid and if datapoint updated , update centroid
        for(int i = 0 ; i < imgs->get_imgs(); i++) {

            Img* point = imgs->get_image(i);
            int prev_cluster = point->get_flag();
            //Find closest cluster according to method asked
            if(!strcmp(argv[8],"Classic") ) {
                closest = find_cluster(point, clusters);
            }
            //If datapoint has changed cluster
            if(point->update_flag(closest)) {
                //Update centroids in previous cluster(if it wasn't -1) and closest
                // if(prev_cluster != -1)
                //     clusters[prev_cluster]->remove_point(point, changed); 
                clusters[closest]->centroid()->display_p(outFile);
                clusters[closest]->insert_point(point, changed);
                clusters[closest]->centroid()->display_p(outFile);

            }
        }       
    } while (changed < 2); 

    //Keep track of ending time
    const auto end_cluster{chrono::steady_clock::now()};
    chrono::duration<double> t_cluster{end_cluster - start_cluster};    
    // //Create Search Structure where we are only gonna be saving the centroids
    // //(since we want to find the nearest neighbour out of them and non-centroid point in its iteration)

    // // else if(!strcmp(argv[8],"LSH") ) {

    // // }
    // // else if(!strcmp(argv[8],"Hypercube") ) {

    // // }  
    //Write time metrics into output file
    outFile<<"Algorithm: "<<argv[8]<<endl;

    // for(int i = 0; i < k_clusters; i++) {
    //     outFile<<"CLUSTER-"<<i+1<<" size: "<<clusters[i]->size()<<" , centroid : ";
    //     clusters[i]->centroid()->display_p(outFile);
    //     outFile<<endl;
    //     if(complete)
    //         clusters[i]->display(outFile);
    // }

    outFile<<endl<<"clustering time: "<<t_cluster.count()<<" sec."<<endl<<"Silhouette [";
    exit(1);
    // double total_s = 0.0;

    // for(int i = 0; i < k_clusters; i++) {
    //     double s_i = clusters[i]->silhouette(clusters);
    //     outFile<<s_i<<" , ";
    //     total_s += s_i ; 
    // }

    // total_s/=k_clusters;
    // outFile<<total_s<<" ]"<<endl;

    outFile.close();      

    delete imgs;
    return 0;
}