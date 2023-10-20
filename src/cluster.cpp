#include "headers/cluster.h"

using namespace std;

Img* chooseNextCenter(const vector<Img*>& centroids, Input* imgs) {

    Img* nextCenter = nullptr;    
    //Sum of squares of d_i, i belonging to the non-centroid set,(divider of the p(r) probability)
    int sum = 0;
    //Vector containing all the d_i, i being non centroid
    vector<double> d;  
    double max_prob = numeric_limits<double>::min();  
    Img* max_prob_i ;  
    Img* candidate;

    // Iterate through the non-centroid images in the dataset
    for (int i = 0; i < imgs->get_imgs(); i++) {
        candidate = imgs->get_image(i);
        //Check that image isn't centroid
        if(candidate->get_flag() != 0) {
            // Calculate the minimum euclidean distance to the current centroids
            double minDistance = numeric_limits<double>::max();
            
            for (Img* centroid : centroids) {
                double dist = candidate->euclideanDistance(centroid);
                if (dist < minDistance) {
                    minDistance = dist;
                }
            }
            //min distance is the d_i
            sum += pow(minDistance,2);
            d.push_back(minDistance);
        }
    }

    // Estimate the probability of every non centroid datapoint and keep the max one
    for (int i = 0; i < imgs->get_imgs(); i++) {  
        candidate = imgs->get_image(i);  
        //Check that image isn't centroid
        if(candidate->get_flag() != 0) {
            double prob_i = pow(d[i],2) / sum;
            if (prob_i > max_prob) {
                max_prob_i = candidate;
            }
        }
    }
    return max_prob_i;
}

//Initializes and returns the set of the k centroid images
vector<Img*> kmeans_init(Input* imgs,int clusters) {

    vector<Img*> centroids;

    //Choose a centroid randomly from the image dataset and save it into the set
    int chosen = rand() % imgs->get_imgs();
    //Image became centroid -> update flag to 0
    imgs->get_image(chosen)->update_flag(0);
    centroids.push_back(imgs->get_image(chosen));

    for(int i = 0; i < clusters; i++){
        Img* nextCenter = chooseNextCenter(centroids, imgs);
        //Image became centroid -> update flag to 0
        imgs->get_image(chosen)->update_flag(0);        
        centroids.push_back(nextCenter);
    } 
    return centroids;
}

//Returns index of closest cluster of vector centroids
int find_cluster(Img* point, vector<Img*>& centroids){

    // Calculate the minimum euclidean distance from centroids
    double minDistance = numeric_limits<double>::max();    
    //Index of closest centroid in vector
    int cluster;    
    //Calculate distance of datapoint from every centroid and keep the closest one
    for(int i = 0; i < centroids.size(); i++){
        double dist = point->euclideanDistance(centroids[i]);
        if (dist < minDistance) {
            minDistance = dist;
            cluster = i;
        }
    }
    return cluster;
}
