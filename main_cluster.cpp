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
    LSH *lsh = NULL;
    //Create Search Structure where we are only gonna be saving the centroids
    //(since we want to find the nearest neighbour out of them and non-centroid point in its iteration)    
    if(!strcmp(argv[8],"LSH") ) {
        lsh = new LSH(parameters["number_of_vector_hash_tables:"],
                      parameters["number_of_vector_hash_functions:"],
                      imgs);
    }       
    Cube* cube = NULL;
    if(!strcmp(argv[8],"Hypercube") ) {
        cube = new Cube(parameters["number_of_hypercube_dimensions:"],
                        parameters["max_number_M_hypercube:"],
                        parameters["number_of_probes:"],
                        imgs);
    }  
    
    double R = initial_R(centroids);
    int cur_assigned = 0;
    int prev_assigned;

    do {
        //Number of clusters whose centroids have changed
        changed = 0;
        
        //Find closest cluster according to method asked
        if(!strcmp(argv[8],"Classic") ) {
            //For every datapoint , assign it to closest centroid and if datapoint updated , update centroid
            for(int i = 0 ; i < 60000; i++) {

                Img* point = imgs->get_image(i);
                int prev_cluster = point->get_flag();                
                closest = find_cluster(point, clusters);
                //If datapoint has changed cluster
                if(point->update_flag(closest)) {
                    //Update centroids in previous cluster(if it wasn't -1) and closest
                    if(prev_cluster != -1)
                        clusters[prev_cluster]->remove_point(point, changed); 
                    clusters[closest]->insert_point(point, changed);
                }  
            }
        }
        else if(!strcmp(argv[8],"LSH") ) {

            //Repeat until you can't find new neighbours that haven't already been asssigned to centroids
            do {
                prev_assigned = cur_assigned;
                cur_assigned = 0;
                //For every centroid perform a range search on centroid with radius R
                for(int i = 0; i < k_clusters; i++) {

                    //A map that corresponds every image that will be assigned in this iteration to its closest distance
                    map<Img*,double> bestDist;
                    map<Img*,double>::iterator marked;

                    Img* centroid = clusters[i]->centroid();

                    //Set holds a pair with distance from query point and image number
                    set<pair<double, int>> cluster_points;
                    lsh->Approx(centroid,cluster_points,R);
                    cur_assigned += cluster_points.size();

                    //Iterate neighbouring datapoints
                    for(const auto& pair : cluster_points) {

                        Img* img = imgs->get_image(pair.first);
                        double dist = pair.second;
                        int prev_cluster = img->get_flag();
                        int cur_cluster = clusters[i]->num();
                        //If image hasn't been assigned, insert it into map and update its flag with cluster's number
                        if( prev_cluster == -1) {
                            bestDist[img] = dist;
                            img->update_flag(cur_cluster);
                        }
                        //Else check if image had been marked in current iteration, otherwise, if datapoint had been assigned in a previous iteration for a different radius, we mustn't reexamine it
                        else {
                            marked = bestDist.find(img);
                            if(marked != bestDist.end()) {
                                double prev_dist = bestDist[img];
                                if(dist < prev_dist) {
                                    bestDist[img] = dist;
                                    img->update_flag(cur_cluster);
                                }
                            }
                        }    
                    }
                }

                R*=2.0;
            }while(cur_assigned > prev_assigned);

            //Now that we have finally decided upon assignments perform them
            for(int i = 0; i < imgs->get_imgs(); i++) {

                Img* img = imgs->get_image(i);
                int cluster = img->get_flag();

                if(cluster != -1) 
                    clusters[cluster]->insert_point(img,changed);
            }
        }
        else if(!strcmp(argv[8],"Hypercube") ) {
            //A map that corresponds every image to its closest distance
            map<Img*,double> bestDist;

            //Repeat until you can't find new neighbours that haven't already been asssigned to centroids
            do {
                prev_assigned = cur_assigned;
                cur_assigned = 0;
                //For every centroid perform a range search on centroid with radius R
                for(int i = 0; i < k_clusters; i++) {

                    Img* centroid = clusters[i]->centroid();

                    //Set holds a pair with distance from query point and image number
                    set<pair<double, int>> cluster_points;
                    cube->Approx(centroid,cluster_points,R);
                    cur_assigned += cluster_points.size();

                    //Iterate neighbouring datapoints
                    for(const auto& pair : cluster_points) {

                        Img* img = imgs->get_image(pair.first);
                        double dist =  pair.second;
                        int prev_cluster = img->get_flag();
                        int cur_cluster = clusters[i]->num();
                        //If image hasn't been assigned, insert it into map and update its flag with cluster's number
                        if( prev_cluster == -1) {
                            bestDist[img] = dist;
                            img->update_flag(cur_cluster);
                        }
                        //Else if image belonged to another cluster update in case distance from current centroid is smaller
                        else if(prev_cluster != cur_cluster) {
                            double prev_dist = bestDist[img];
                            if(dist < prev_dist) {
                                bestDist[img] = dist;
                                img->update_flag(cur_cluster);
                            }
                        }
                    }
                }
                R*=2.0;
            }while(cur_assigned > prev_assigned);

            //Now that we have finally decided upon assignments perform them
            for(int i = 0; i < imgs->get_imgs(); i++) {

                Img* img = imgs->get_image(i);
                int cluster = img->get_flag();

                if(cluster != -1) {
                    clusters[cluster]->insert_point(img,changed);
                }
                else {
                    //If image hadn't been assigned to a cluster find the closest one with brute force
                    cluster = find_cluster(img,clusters);
                    clusters[cluster]->insert_point(img,changed);
                }
            }
        }

    }while (changed < 2); 

    //Keep track of ending time
    const auto end_cluster{chrono::steady_clock::now()};
    chrono::duration<double> t_cluster{end_cluster - start_cluster};    

    //Write time metrics into output file
    outFile<<"Algorithm: "<<argv[8]<<endl;

    for(int i = 0; i < k_clusters; i++) {
        outFile<<"CLUSTER-"<<i+1<<" size: "<<clusters[i]->size()<<" , centroid : ";
        clusters[i]->centroid()->display_p(outFile);
        outFile<<endl;
        if(complete)
            clusters[i]->display(outFile);
    }

    outFile<<endl<<"clustering time: "<<t_cluster.count()<<" sec."<<endl<<"Silhouette [";
    double total_s = 0.0;

    for(int i = 0; i < k_clusters; i++) {
        double s_i = clusters[i]->silhouette(clusters);
        outFile<<s_i<<" , ";
        total_s += s_i ; 
    }

    total_s/=k_clusters;
    outFile<<total_s<<" ]"<<endl;

    if(!strcmp(argv[8],"LSH") ) {
        delete lsh;
    } 
    if(!strcmp(argv[8],"Hypercube") ) {
        delete cube;
    }  
    outFile.close();      

    delete imgs;
    return 0;
}