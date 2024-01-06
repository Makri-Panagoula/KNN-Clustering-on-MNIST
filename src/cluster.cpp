#include "../headers/cluster.h"

using namespace std;

Cluster::Cluster(Img* centroid,int cluster_num) {
    this->cluster = cluster_num;
    this->center = centroid;
    this->datapoints.push_front(centroid);
}

//Chooses the next centroid out of the non-centroid input images based on d_i^2
Img* chooseNextCenter(vector<Img*>& centroids, Input* imgs) {

    double max_prob = numeric_limits<double>::min();  
    Img* max_prob_i = nullptr;  
    Img* candidate;

    //Iterate through the non-centroid images in the dataset
    //We won't actually express it as a probability since it is uselessly computationally complex(the result will be the same since the divider is the same for all)
    for (int i = 0; i < imgs->get_imgs(); i++) {
        candidate = imgs->get_image(i);
        double d_i = numeric_limits<double>::max();
        //Check that image is unassigned aka not a centroid(since at this point of execution the only assigned points are centroids)
        if(candidate->get_flag() == -1) {
            //Calculate the minimum euclidean distance to the current centroids,which will be the d_i
            for (Img* centroid : centroids) {
                double dist = candidate->euclideanDistance(centroid);
                if (dist < d_i) {
                    d_i = dist;
                }              
            }
            double prob_i = pow(d_i,2);
            if (prob_i > max_prob) {
                max_prob = prob_i;
                max_prob_i = candidate;
            }              
        }
    }
    return max_prob_i;
}

//Initializes and returns the set of the k centroid images
vector<Img*> kmeans_init(Input* imgs, int clusters) {

    vector<Img*> centroids;

    //Choose a centroid uniformly at random from the image dataset and save it into the set
    default_random_engine generator;
    generator.seed(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0,imgs->get_imgs() - 1);    
    int chosen = distribution(generator);

    //Image became centroid -> update flag to 0 since it is the centroid of the first cluster
    centroids.push_back(imgs->get_image(chosen));
    centroids[0]->update_flag(0);

    for(int i = 1; i < clusters ; i++){
        Img* nextCenter = chooseNextCenter(centroids, imgs); 
        //Image became centroid -> update flag to its corresponding cluster
        nextCenter->update_flag(i);     
        centroids.push_back(nextCenter);
    } 

    return centroids;
}

//Returns index of closest cluster of vector centroids
int find_cluster(Img* point, vector<Cluster*>& clusters) {

    // Calculate the minimum euclidean distance from centroids
    double minDistance = numeric_limits<double>::max();    
    //Index of closest centroid in vector
    int cluster;    
    //Calculate distance of datapoint from every centroid and keep the closest one
    for(int i = 0; i < clusters.size(); i++) {
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
void Cluster::remove_point(Img* point) {

    vector<unsigned char> old_center = center->get_p();
    vector<unsigned char> new_point = point->get_p();
    //Adding one for the centroid
    int len = datapoints.size() + 1; 
    //Calculating the new mean by the previous one
    for(int i = 0 ; i < new_point.size(); i++) {

        long double new_mean = old_center[i] + ceil( ( old_center[i] - new_point[i] ) /  (len - 1));
        unsigned char p_i = (unsigned char) new_mean;    
        center->update_p(i,p_i);
    }
    datapoints.remove(point);
}

//Inserts datapoint into cluster and updates accordingly the centroid
void Cluster::insert_point(Img* point){

    //Estimate new centroid for this cluster
    vector<unsigned char> old_center = center->get_p();
    vector<unsigned char> new_point = point->get_p();
    //Inserting new datapoint
    datapoints.push_front(point);    
    //Adding one for the centroid
    int len = datapoints.size() + 1; 

    //Calculating the new mean by the previous one
    for(int i = 0 ; i < new_point.size(); i++) {

       long double new_mean = old_center[i] + ceil((new_point[i] - old_center[i]) / len);
       unsigned char p_i = (unsigned char) new_mean;    
       center->update_p(i,p_i);
    }
}

//Writes in output file the number of each image included in the cluster
void Cluster::display(ofstream& outFile) {
    outFile<<endl<<"Images : {";
    list<Img*>::iterator it;
    for(it = datapoints.begin(); it != datapoints.end(); ++it) {
        Img* datapoint = *it;
        outFile<<datapoint->imgNum()<<" , ";
    }
    outFile<<"}"<<endl<<endl;
}

//Computes average distance of datapoint to all the datapoints in the cluster
double Cluster::avg_dist(Img* datapoint) {

    //average distance between point1 and the points of the cluster
    double a = 0.0;     
    for (auto point2 = datapoints.begin(); point2 != datapoints.end(); point2++){
        a += datapoint->euclideanDistance(*point2);
    }
    a /= datapoints.size();
    return a;
}

double Cluster::silhouette(vector<Cluster*>& clusters){
    double s = 0.0;

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
            s += 1.0 - (a/b);
        } else if (a > b) {
            s += (b/a) - 1.0;
        }
    }
    s /= datapoints.size();
    return s;
}

//Returns the minimum distance between the centroids divided by 2 that will serve as the initial value for R
double initial_R(vector<Img*>& centroids) {

    // Calculate the minimum euclidean distance from centroids
    double minDist = numeric_limits<double>::max();     

    for (size_t i = 0; i < centroids.size(); i++) {
        for (size_t j = i + 1; j < centroids.size(); j++) {
            double dist = centroids[i]->euclideanDistance(centroids[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }

    return minDist / 2.0;    
}

void Cluster::project_data(Input* init_imgs) {
    for (auto point1 = datapoints.begin(); point1 != datapoints.end(); point1++)
        *point1 = init_imgs->get_image((*point1)->imgNum());
}