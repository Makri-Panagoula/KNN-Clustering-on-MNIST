#include "../headers/MRNG.h"

using namespace std;

//Returns a set with the images with minimum distance from p 
set <pair<double, int>> neighbours(Img* p, Input* imgs, set <pair<double, int>>& R_L,LSH* lsh) {

    set <pair<double, int>> unused;
    //We search for all the approximate nearest neighbours instead of sorting
    set <pair<double, int>> R = lsh->Approx(p,unused,0);    
    //L contains a set of images with minimum distance from p
    set <pair<double, int>> L;

    //If no neighbours were found, return empty set
    if(R.size() == 0)
        return L;

    // cout<<"Nearest neighbours found in total from lsh : "<<R.size()<<endl;

    auto exact = R.begin();
    double min_dist = exact->first;
    
    //We only care about those having minimum distance from p
    for (int i = 1; i < R.size(); i++) { 
        exact++;
        double dist = exact->first;
        if(dist > min_dist) 
            break;
        L.insert(make_pair(exact->first,exact->second));
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
    //Create LSH Structure(we consider 60 hash functions and 5 Hashtables sacrificing accuracy but gaining speed)
    LSH* lsh = new LSH(80,5,imgs);    
    for(int i = 0; i < imgs->get_imgs(); i++) {
        Img* p = imgs->get_image(i);
        set <pair<double, int>> R_L;
        set <pair<double, int>>  L = neighbours(p, imgs, R_L,lsh);
        bool to_add = true;
        for (const auto& r_l : R_L) {
            Img* r = imgs->get_image(r_l.second); 
            for (const auto& l_img : L) {
                Img* t = imgs->get_image(l_img.second); 
                if(p->longestEdge(t,r)) {
                    to_add = false;
                    break;
                }
            }     
            if(to_add) {
                L.insert(make_pair(p->euclideanDistance(r),r->imgNum()));
            }
        }
        //Insert every image l of set L into the respective vector of the graph
        vector<Img*> neighbours_p;
        for (const auto& l : L) {
            neighbours_p.push_back(imgs->get_image(l.second));
        }
        this->Graph[i] = neighbours_p;
        // cout<<"Neighbours inserted : "<<neighbours_p.size()<<endl;

    }

    //Find Navigating Node by computing the nearest neighbour of the dataset's centroid by brute force
    Img* centroid = dataset_centroid(imgs);
    set<pair<double, int>> N_exact = imgs->N_Exact(centroid);
    int img_num = N_exact.begin()->second;
    this->nav_node = imgs->get_image(img_num);

    //Let go of lsh structure & centroid ,we won't need them any more
    delete lsh;    
    delete centroid;
}

//Returns the first unchecked node of the set of candidates(otherwise returns NULL)
Img* first_unchecked(Input* imgs, set<pair<double,int>> candidates,set<Img*> checked,int& i) {
    i = 1;
    for (const auto& candi : candidates){
        Img* candidate = imgs->get_image(candi.second);
        if(checked.find(candidate) == checked.end()) 
            return candidate;
        i++;
    }
    return NULL;
}

//Perform Graph Nearest Neighbour Search and return a set<pair<double, int>> holding distance and image number with the results the search found
set<pair<double,int>> MRNG::NearestNeighbour(Img* query) {

    set<pair<double,int>> candidates;
    //Initialize set with the navigation node
    double dist = nav_node->euclideanDistance(query);
    candidates.insert(make_pair(dist, nav_node->imgNum()));
    set<Img*> checked;
    int i = 1;
    //Until you reach max candidates
    while(i < this->l) {
        //Find the first_unchecked node in the set and mark it
        Img* p = first_unchecked(this->imgs,candidates,checked,i);
        if (p == NULL)
            break;
        
        checked.insert(p);
        // cout<<"initial p "<<p->imgNum()<<endl;
        //Find the neighbours of the node using the Graph and insert into set
        vector<Img*> neighbours = this->Graph[p->imgNum()];
        for (Img* neigh: neighbours) {
            double dist = neigh->euclideanDistance(query);
            //No need to check for uniqueness, since it is a set (it will have the same imgNum and distance)
            candidates.insert(make_pair(dist, neigh->imgNum()));
            // cout<<"neighbour p "<<neigh->imgNum()<<endl;
        }
    }
    cout<<"Neighbours found in expanding : "<<candidates.size()<<endl;
    return candidates;
}

MRNG::~MRNG() {
    delete[] this->Graph;
}