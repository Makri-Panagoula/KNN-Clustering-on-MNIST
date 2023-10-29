Ευαγγελία Γούλα - 1115202000040
Παναγούλα Μακρή - 1115202000119

------ A PART-------

~~LSH~~
--Code Organization & Approach--:
The LSH implementation reads the input dataset, that has all the information about the images. The LSH constructor initializes the Locality Sensitive Hashing algorithm by creating L number of hashtables using k number of hashfunctions and then stores each image of the training dataset based on the lsh technique. After that we take a small sample of the images in the query dataset to perform the algorithms on them and call the queryNeighbours function (lsh.cpp), so as to find and compare the nearest neighbors of a given query image. In order to do that, we create two different vectors are created, one for the approximate neighbours and one for the N-Exact neighbours in a specific radious.The set H including the available hash functions out of which each g-function is constructed has bigger cardinality than k to increase randomness, arbitrarily chosen, here 30*k. The k r-operands are chosen randomly and uniquely for every g-function for the same reason.We use the amplified lsh algorithm with the querying id trick (using mod M before mod TableSize) for improved performance.We choose the first 2 images of the query set to test the algorithms.  

--Compiling--:
You can run the program using the command make run_lsh. In order to change command line arguments, you can change the line keeping the wanted format as in:
ARGSL = –d input_file –q query_file –k bumber_of_h_funcs_g_consists_of -L number_of_hashtables -ο output_file -Ν 11 -R 2500.If not given the default values for k,L,N,R are 4, 5 , 1 and 10000 respectively.

~~HYPERCUBE~~
--Code Organization & Approach--:
The hypercube implementation creates 1 hashtable (with 2^d buckets as many as the vertices of the hypercube) using the h functions from LSH (hFunc.h and hFunc.cpp) applying a different one for each of the d-dimensions of the hypercube to hash the pixels vector (p) of the datapoint and later applies a uniformly distributed random function f mapping the h(p) to {0,1} creating a binary vector belonging to R^d which we convert to a decimal that represents the bucket our datapoint will be hashed into (store function).The f function decides on the {0,1} value with uniform random distribution and we use a map for each of the d f-functions that holds the h_i(p) as a key and the mapped number as value to maintain consistency.To perform the search, we hash the query datapoint into a bucket and until we reach maximum datapoints or maximum vertices to be checked we traverse the neighbouring buckets-vertices of the hypercube with increasing Hamming distance. The source files related with the implementation can be found in cube.cpp (and its header cube.h) and main_cluster.cpp which mainly performs input parameter processing and calls the functions from cube.cpp.

--Compiling--:
Run with make run_cube , to change the paramaters update the ARGSC in Makefile.Parameters must be given in the following order:
–d input_file –q  query_file –k dimensions_hypercube -M max_datapoints_to_be_checked -probes max_vertices_to_be_checked -ο output_file -Ν nearest_neighbours -R radius for range search.
k,M,N,R can be omitted and then they take their default values : 14 , 10 , 2 , 1 and 10000 respectively.

------ B PART-------
Regardless of the assignment algorithm, the initialization of the centroids is being done with the k-means++ algorithm and the update with MacQueen repeating until the centroids of the clusters remain unchanged during the updates. In the k-means we choose the first centroid randomly from images on the dataset and the following ones out of the images in the dataset based on their probability,d_i^2,(we don't normalize), where d_i is the minimum distance of image i from all the initialized centroids. The clustering algorithm parameters are given in the cluster.conf file. Files regarding the implementation are main_cluster.cpp,cluster.cpp and cluster.h

--Compiling--:
You can run this part using the command make run_cluster. 
In order to test the different methods of clustering implementation, you can change the last argument in this line:
ARGSCL = –i datasets/input.dat –c cluster.conf -o output_cluster -m Classic
The implemented methods are "Classic", "LSH", "Hypercube".With the parameter -complete after the output file , the contents of the clusters are demonstrated in the output file as well

~~Lloyd~~
--Code Organization & Approach--:
In the implementation of the classic version, Loyd's algorithm, we compare the previous cluster that each point was in with the actual closest centroid. The index of the closest cluster is found in the find_cluster function by calculating the distance of the datapoint to each ceνtroid and choosing the closest. Each datapoint has a flag that indicates if it is already assigned to a cluster. If the flag is -1, means it's not yet assigned, otherwise it holds the number of the cluster that it's assigned to. If the datapoint was already assigned to another cluster and now has to change cluster, we first remove it for the current cluster and then insert it to the new one. When removing or inserting, we update the new centroid of the clusters as the mean of its datapoints.

~~Reverse Assignment~~
--Code Organization & Approach--:
Regardless of which data structure we use to save our datapoints(LSH/Hypercube) the approach remains the same.Before searching for clusters we save the datapoints in the wanted search structure and starting from a radius = minimum distance of centroids / 2 we perform a range search for every centroid with this radius ,repeating until no new points are assigned to the clusters doubling the radius in each outter loop.For a given radius , we use a map called bestDist mapping each image that hadn't been assigned for a different radius (checking the unassigned set initialized in every MacQueen iteration) to the current best distance from centroids we have found(its cluster number held in cluster map) so that if the same datapoint belongs to multiple balls we will choose the closest one. We assign the datapoints before doubling the radius and we can't reassign them for the same iteration of the Macqueen loop. 

In the end, we calclulate the Silhouette Score, which is a measure of how similar an object is to its cluster compared to other clusters. It provides a way to assess the quality of clustering.
We calculate the average silhouette score for each cluster, as well as for the entire dataset.
We notice that the clusters' silhouettes have a relatively small fluctuation (for multiple runs) which is what we were expecting since the number of clusters we chose (10) is the ideal for the dataset (digits).However, the initial random choice of centoids plays a huge role in the end result , since a poor one might get the algorithm stuck on a local minimum.We also notice that Lloyd's has generally a better performance than reverse assignment , which is due to the accuracy - speed tradeoff we choose when computing the nearest cluster.The result is a low total silhouette score, which suggests that using these methods the objects are not well matched to the clusters, therefore the metrics are either not good enough for our dataset or there is big existence of outliers.