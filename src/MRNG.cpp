#include "../headers/MRNG.h"

using namespace std;

//Returns a set with the images with minimum distance from p 
set<Img*> neighbours(Img* p, Input* imgs, set<Img*>& R_L) {
    
    //L contains a set of images with minimum distance from p
    set<Img*> L;
    //R contains a set of all images apart from p
    set<Img*> R;

    double min_dist = numeric_limits<double>::max();
    //For every image in the dataset other than p calculate distance 
    for(int i = 0; i < imgs->get_imgs(); i++) {

        Img* candidate = imgs->get_image(i);
        if(candidate != p) {
            R.insert(candidate);
            double dist = p->euclideanDistance(candidate);
            if(dist < min_dist) {
                min_dist = dist;
                //Remove previous elements from set since we have found a better distance
                L.clear();
            }
            else if(dist == min_dist){
                L.insert(candidate);
            }
        }
    }
    //Perform set subtraction between R and L
    set_difference(R.begin(),R.end(),L.begin(),L.end(),inserter(R_L,R_L.begin()));
    return L;
}

//Returns the centroid out of the whole dataset 
Img* dataset_centroid(Input* imgs) {

    int pxs = imgs->get_pxs();
    Img* centroid = new Img(pxs);
    int img_count = imgs->get_imgs();

    //For every dimension calculate the average of all images and assign to centroid
    for(int d = 0; d < pxs; d++) {
        unsigned int avg_d = 0;
        for(int i = 0; i < img_count; i++) {
            Img* img = imgs->get_image(i);
            vector<unsigned char> p = img->get_p();
            avg_d += p[d];
        }
        avg_d /= img_count;
        centroid->update_p(d,avg_d);
    }

    return centroid;
}

MRNG::MRNG(int l, Input* imgs) {

    this->l = l;
    this->imgs = imgs;

    //For every image we will create a vector with its nearest neighbours(if less as many as available)
    this->Graph = new vector<Img*>[imgs->get_imgs()];

    for(int i = 0; i < imgs->get_imgs(); i++) {
        Img* p = imgs->get_image(i);
        set<Img*> R_L;
        set<Img*> L = neighbours(p, imgs, R_L);
        bool to_add = true;
        for (const auto& r : R_L) {
            bool to_add = true;
            for (const auto& t : L) {
                if(p->longestEdge(t,r)) {
                    to_add = false;
                    break;
                }
            }     
            if(to_add) {
                L.insert(r);
            }
        }
        //Insert every image l of set L into the respective vector of the graph
        vector<Img*> neighbours_p;
        for (const auto& l : L) {
            neighbours_p.push_back(l);
        }
        this->Graph[i] = neighbours_p;
    }

    //Find Navigating Node by computing the nearest neighbour of the dataset's centroid by brute force
    Img* centroid = dataset_centroid(imgs);
    set<pair<double, int>> N_exact = imgs->N_Exact(centroid);
    int img_num = N_exact.begin()->second;
    this->nav_node = imgs->get_image(img_num);
}

//Returns the first unchecked node of the set and if such founds updates its flag as marked (otherwise returns NULL)
Img* first_unchecked(Input* imgs, set<pair<double,int>> candidates) {

    for (const auto& candi : candidates){
        Img* candidate = imgs->get_image(candi.second);
        //Every image instance when created has its flag initialized to -1 , by convention we turn it into 0
        if(candidate->update_flag(0)) 
            return candidate;
    }
    return NULL;
}

//Perform Graph Nearest Neighbour Search and return a set<pair<double, int>> holding distance and image number with the results the search found
set<pair<double,int>> MRNG::NearestNeighbour(Img* query) {

    set<pair<double,int>> candidates;
    //Initialize set with the navigation node
    double dist = nav_node->euclideanDistance(query);
    candidates.insert(make_pair(dist, nav_node->imgNum()));
    int i = 1;
    //Until you reach max candidates
    while(i < this->l) {
        //Find the first_unchecked node in the set and mark it
        Img* p = first_unchecked(this->imgs,candidates);
        if (p == NULL){
            break;
        }
        //Find the neighbours of the node using the Graph and insert into set
        vector<Img*> neighbours = this->Graph[p->imgNum()];
        for (Img* neigh: neighbours) {
            double dist = neigh->euclideanDistance(query);
            //No need to check for uniqueness, since it is a set (it will have the same imgNum and distance)
            candidates.insert(make_pair(dist, neigh->imgNum()));
            i++;
        }
    }
    return candidates;
}

MRNG::~MRNG() {
    delete nav_node;
    delete[] this->Graph;
}