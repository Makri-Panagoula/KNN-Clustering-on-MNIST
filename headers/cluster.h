#pragma once
#include <set>
#include <vector>
#include <limits>
#include <list>
#include <random>
#include <iostream>
#include <algorithm>
#include "../headers/img.h"
#include "../headers/input.h"

class Cluster{
    private:
        //the number of the cluster
        int cluster;
        //structure that has the datapoints of the cluster
        list<Img*> datapoints;
        //holds the center of the cluster
        Img* center;
    public:
        Cluster(Img* centroid,int cluster_num);
        Img* get_centroid(){ return this->center;}
        //Removes datapoint from cluster and updates accordingly the centroid
        void remove_point(Img* point, int &changed);
        //Inserts datapoint into cluster and updates accordingly the centroid
        void insert_point(Img* point, int &changed);
        int size(){ return datapoints.size();}
        void display(ofstream& outFile);
        Img* centroid(){return this->center;}
        int num(){return this->cluster;}
        //Computes average distance of datapoint to all the datapoints in the cluster
        double avg_dist(Img* datapoint);
        double silhouette(vector<Cluster*>& clusters);
        ~Cluster(){};
};


Img* chooseNextCenter(const set<Img*>& centroids, Input* imgs);

//Initializes and returns the set of the k centroid images
vector<Img*> kmeans_init(Input* imgs, int clusters);

//Returns index of closest cluster of vector centroids
int find_cluster(Img* point, vector<Cluster*> &centroids);