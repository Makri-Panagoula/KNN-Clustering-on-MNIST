#include "../headers/cluster.h"

using namespace std;

Cluster::Cluster(Img* centroid,int cluster_num) {
    this->cluster = cluster_num;
    this->center = centroid;
}

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
int find_cluster(Img* point, vector<Cluster*>& clusters){

    // Calculate the minimum euclidean distance from centroids
    double minDistance = numeric_limits<double>::max();    
    //Index of closest centroid in vector
    int cluster;    
    //Calculate distance of datapoint from every centroid and keep the closest one
    for(int i = 0; i < clusters.size(); i++){
        Img* centroid = clusters[i]->get_centroid();
        double dist = point->euclideanDistance(centroid);
        if (dist < minDistance) {
            minDistance = dist;
            cluster = i;
        }
    }
    return cluster;
}

//Removes datapoint from cluster and updates accordingly the centroid
void Cluster::remove_point(Img* point,int &changed) {

    vector<unsigned char> old_center = center->get_p();
    vector<unsigned char> new_point = point->get_p();
    int len = datapoints.size(); 
    //Calculating the new mean by the previous one
    for(int i = 0 ; i < new_point.size(); i++) {
        unsigned char p_i = (old_center[i] * len - new_point[i]) / (len - 1);
        center->update_p(i,p_i);
        if(p_i != old_center[i] )
            changed++;
    }
    datapoints.remove(point);
}


//Inserts datapoint into cluster and updates accordingly the centroid
void Cluster::insert_point(Img* point, int &changed){

    //Estimate new centroid for this cluster
    vector<unsigned char> old_center = center->get_p();
    vector<unsigned char> new_point = point->get_p();
    int len = datapoints.size(); 
    //Calculating the new mean by the previous one
    for(int i = 0 ; i < new_point.size(); i++) {
        unsigned char p_i = (old_center[i] * len + new_point[i]) / (len + 1);
        center->update_p(i,p_i);
        if(p_i != old_center[i] )
            changed++;        
    }
    datapoints.push_front(point);
}

void Cluster::display(ofstream& outFile) {
    outFile<<endl<<"Images : ";
    list<Img*>::iterator it;
    for(it = datapoints.begin(); it != datapoints.end(); ++it) {
        Img* datapoint = *it;
        outFile<<datapoint->imgNum()<<" , "<<endl;
    }
}

//Computes average distance of datapoint to all the datapoints in the cluster
double Cluster::avg_dist(Img* datapoint) {
    //Number of different datapoints out of which we will compute the average distance
    int different = datapoints.size();
    //average distance between point1 and the rest of the points of the cluster
    double a = 0.0;     
    for (auto point2 = datapoints.begin(); point2 != datapoints.end(); point2++){
        if (datapoint != *point2){
            a += datapoint->euclideanDistance(*point2);
            different--;
        }
    }
    a /= different;
    return a;
}

double Cluster::silhouette(vector<Cluster*>& clusters){
    
    double s = 0;

    for (auto point1 = datapoints.begin(); point1 != datapoints.end(); point1++){

        //minimum distance between point1 and the other centroids    
        double min_dist = numeric_limits<double>::max(); 
        //Cluster with closest distance to datapoint
        int b_cluster; 

        //calculate a(i)
        double a = this->avg_dist(*point1);
        //Find next best neighbour, second closest centroid, traverse the other clusters, compute distance (if the cluster isn't our current) and keep the min
        for (auto otherCluster : clusters){
            if (otherCluster->num() != this->cluster) {
                Img* other_centroid = otherCluster->centroid();
                double dist = (*point1)->euclideanDistance(other_centroid);
                if( dist < min_dist) {
                    min_dist = dist;
                    b_cluster = otherCluster->num();
                }
            }
        }
        //calculate b(i)
        double b = clusters[b_cluster]->avg_dist(*point1);

        if (a < b) {
            s += 1 - (a/b);
        } else if (a > b) {
            s += (b/a) - 1;
        }

    }
    s /= datapoints.size();
    return s;
}