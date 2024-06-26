#pragma once
#include <set>
#include <vector>
#include <limits>
#include <chrono>
#include <list>
#include <random>
#include <iostream>
#include <algorithm>
#include "../headers/img.h"
#include "../headers/input.h"
#include "../headers/lsh.h"
#include "../headers/cube.h"

class Cluster{
    private:
        //the number of the cluster varying from 0 to k-1 clusters
        int cluster;
        //structure that has the datapoints of the cluster
        list<Img*> datapoints;
        //holds the center of the cluster
        Img* center;
    public:
        Cluster(Img* centroid,int cluster_num);
        Img* get_centroid(){ return this->center;}
        //Removes datapoint from cluster and updates accordingly the centroid
        void remove_point(Img* point);
        //Inserts datapoint into cluster and updates accordingly the centroid
        void insert_point(Img* point);
        //Number of datapoints in cluster (not including centroid)
        int size(){ return datapoints.size();}
        //Writes in output file the number of each image included in the cluster
        void display(ofstream& outFile);
        Img* centroid(){return this->center;}
        //Returns the number of cluster(from 0 to k-1)
        int num(){return this->cluster;}
        //Computes average distance of datapoint to all the datapoints in the cluster
        double avg_dist(Img* datapoint);
        double silhouette(vector<Cluster*> &clusters);
        //Updates cluster centroid to new center
        void update_centroid(Img* new_center) {this->center = new_center;}
        //Projects datapoints in cluster by corresponding them to those in the given dataset
        void project_data(Input* init_imgs);
        //Estimates the d(x,C)^2 by summing the euclidean distances of the datapoints in the cluster from its centroid
        double d_x_C();
        ~Cluster(){};
};

//Chooses the next centroid out of the non-centroid input images based on d_i^2
Img* chooseNextCenter(const set<Img*>& centroids, Input* imgs);

//Initializes and returns a vector of the k centroid images
vector<Img*> kmeans_init(Input* imgs, int clusters);

//Returns index of closest cluster of vector clusters
int find_cluster(Img* point, vector<Cluster*> &clusters);

//Returns the minimum distance between the centroids divided by 2 that will serve as the initial value for R
double initial_R(vector<Img*>& centroids);