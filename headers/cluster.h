#include <set>
#include <vector>
#include <limits>
#include <random>
#include "headers/img.h"
#include "headers/input.h"

Img* chooseNextCenter(const std::set<Img*>& centroids, Input* imgs);

//Initializes and returns the set of the k centroid images
vector<Img*> kmeans_init(Input* imgs, int clusters);

//Returns index of closest cluster of vector centroids
int find_cluster(Img* point, vector<Img*>& centroids);