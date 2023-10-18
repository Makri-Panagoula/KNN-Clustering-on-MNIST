#include "../headers/cube.h"

set<int> Cube::searchHammingDistance(int bucket, int hammingDistance, int maxDistance) {
    set<int> neighbors;
    if (hammingDistance > maxDistance) {
        return neighbors;
    }

    // Add all images in the current bucket to neighbors
    for (Img* img : buckets[bucket]) {
        neighbors.insert(img->imgNum());
    }

    // Recursively search neighbor buckets within the specified Hamming distance
    if (hammingDistance < maxDistance) {
        for (int i = 0; i < d; i++) {
            int nextBucket = bucket ^ (1 << i);
            set<int> nextNeighbors = searchHammingDistance(nextBucket, hammingDistance + 1, maxDistance);
            neighbors.insert(nextNeighbors.begin(), nextNeighbors.end());
        }
    }

    return neighbors;
}
