Ευαγγελία Γούλα - 1115202000040
Παναγούλα Μακρή - 1115202000119

------ THIRD ASSIGNMENT-------

--Code Organization & Approach--:
The experimentation process on the autoencoder model is presented on the experiment.ipynb followed by commented diagrams.To avoid overfitting we implement regularization techniques such as batch normalization and dropout in every layer as well as early stopping with patience = 3 (since due to the large number of data we used a rather small number of epochs for training).We opted for using the optuna framework to utilize its pruning algorithms for optimal results, we performed hyperparameter tuning on dropout rate,epochs,batch size,kernel size, number of filters, filters per layer and latent dimension.
The best model we find there we use in the reduce.py file which can be executed running : make run_reduce.

To run the second assignment in the latent space please provide after the -d and -q the respective datasets in the new dataspace and after all the commands -id datafile_in_initial_dataspace -iq queryfile_in_initial_dataspace.For exhaustive search in new space run the second assignment with m=3. 
Similarly, when running the clustering after the compulsory command line arguments add -id datafile_in_initial_dataspace.We perform the clustering in the new dimension and then we find the nearest neighbour of every centroid in the dataset which we eventually project into its initial dataspace.We estimate the silhouette by projecting the rest of the datapoints in the clusters as well.

--Results--:
Our model presents  0.005191380623728037 test loss and  0.8145348429679871 test accuracy.
As expected, we notice that the the latent dimension has the biggest influence on the performance, since the bigger it is (30) the bigger the model's capacity to encapsulate information and successfully extract the most important features of the input.A larger number of layers with big filters ([32, 64, 128, 256]) as well is conducive to model's complexity improving the loss score.A rather moderate batch (128) size is being used meaning the weights are being updated more often and generalize better.Since the dataset is not too complex, it can be learned quickly in small number of epochs (20).Lastly, dropout rate is moderate (0.5) leading to more robust representations and better generalization,(using batch normalization reduces the need for dropout too).The model performs well with a rather small batch size and therefore needs a small number of epochs.


-------Clustering testings-------:  (everything here is tested for 10.000 images)

We estimated Objective Function's Value for Hypercube since this was the method with the best trade off between query time and accuracy.

method: Classic

in the original space:  clustering time: 3.72138 sec.
                        Silhouette [1 , 0.195482 , 0.0676906 , 0.278887 , 0.278667 , 0.278257 , 0.349831 ]
in the new space:       clustering time: 3.74718 sec.
                        Silhouette [-0.00453017 , 0.00135744 , -0.000209328 , -0.00220132 , 0.00586274 , 0.0107627 , 0.00184035 ]

While the clustering time is comparable in the different spaces, there is a reduction in silhouette scores in the new space, which suggests that the clustering performance has degraded after dimensionality reduction.

method: LSH

in the original space: clustering time: 4.08806 sec.
                       Silhouette [1 , 0.197806 , -0.0145723 , 0.277724 , 0.266968 , 0.280108 , 0.334672 ]
in the new space:      clustering time: 3.52786 sec.
                       Silhouette [-0.00536397 , 0.000227006 , 0.000410369 , -0.0039588 , 0.0111781 , 0.00971051 , 0.00203387 ]
                     
LSH shows a reduction in clustering time when applied to the new (reduced) space. Silhouette scores in both the original and new spaces using LSH are generally low, and there might be challenges in achieving well-defined clusters in both spaces.


method: Hypercube:  

in the original space: Objective Function's Value : 7.51175e+12
                       Clustering Time: 1.62866 sec.
                       Silhouette [-0.016072 , 1 , 0.200669 , 0.17003 , 0.286082 , 0.280723 , 0.320239 ]
                       
in the new space:      Objective Function's Value : 3.62356e+14
                       Clustering Time: 1.72669 sec.
                       Silhouette [-0.00536931 , 0.000225461 , 0.000412049 , -0.0039565 , 0.0111829 , 0.00971299 , 0.0020346 ]

The increase in the objective function's value in the new space suggests that the Hypercube method might not be preserving the relevant information well during dimensionality reduction. The clustering time difference is relatively small, indicating a comparable efficiency in clustering between the original and new spaces. Similar to the LSH results, the silhouette scores in the new space using the Hypercube method are generally lower compared to the original space. Negative silhouette scores suggest potential challenges in clustering quality.


-------Graph Search testings-------:

~~GNN~~
                     |   in the original space   |  in the new space
tAverageApproximate  |     0.0143473 sec         |    0.014885 sec
tTrue                |     0.182167 sec          |    0.149065 sec
MAF                  |       3.39889             |      2.14755



~~MRNG~~
                     |   in the original space   |  in the new space
tAverageApproximate  |      0.00111419 sec       |    0.00267578 sec
tTrue                |      1.04515 sec          |    0.0148013 sec
MAF                  |       1.23283             |      1.90689



~~exhaustive search~~
                     |   in the original space  |  in the new space
tAverageApproximate  |      0.191546 sec        |    0.192168 sec
tTrue                |      0.190367 sec        |    0.149924 sec
MAF                  |          1               |      2.3958



------ SECOND ASSIGNMENT-------

------ A PART-------
~~GNN~~
--Code Organization & Approach--:
While constructing the graph, in order to find the k edges (or less if not as many exist) we search for the k-NN using lsh with 3 hash functions and 5 hashtables each having 100 buckets for speed reasons. We depict the graph as an array of 60000 vectors.

--Compiling--:

You can run the program using the command make run_graph with m = 1. In order to change command line arguments, you can change the line keeping the wanted format as in:
ARGS_G = –d datasets/input.dat –q datasets/query.dat –k 50 -E 20 -R -N 5 -l 30 -m 1 -o output_graph. If not given the default values for k,E,R,N are 50,30,1. Argument l can be omitted as it is not used in GNN. Query and output files can be omitted as well as long as they are provided later in program's execution.

------ B PART-------
~~MRNG~~
--Code Organization & Approach--:
The search on the MRNG Graph is implemented according to the algorithm suggested on the second paper, search stops once the index of the first unchecked node of the set is equal to l instead of checking just l nodes since we observed better results like that.In the MRNG construction instead of sorting all the distances of the other images from p to create Rp, we take the k-NN by lsh (k being 59999).Rp won't have all the distances from image p (since lsh only looks for neighbours in each own bucket) but the more we images we check in Rp the less likely it is they will be added in Lp since (they have an increasingly bigger distance from p) and rp is probably going to be the longest edge in the prt triangle so we are still getting a pretty close approximation of Lp.We initialize the lsh structure with 80 hash functions to make the datapoints more well distributed in the buckets and 5 hashtables each having 100 buckets ( essentially images / 600) since we opt for speed over accuracy.We opted for the lsh structure instead of hypercube since we noticed it executes faster.  

--Compiling--:
You can run the program using the command make run_graph with m = 2. In order to change command line arguments, you can change the line keeping the wanted format as in:
ARGS_G = –d datasets/input.dat –q datasets/query.dat –k 50 -E 20 -R -N 5 -l 30 -m 2 -o output_graph.If not given the default value for l is 20 , arguments k,E,R,N can be omitted as they are not used in MRNG.Query and output files can be omitted as well as long as they are provided later in program's execution.

------ C PART-------
For all the experiments we used 1000 datapoints as queries considering this a representative yet fast enough number for testing.Because the algorithms are based on randomness small differences in the results are accepted.

~~LSH~~
   h-functions  Hashtables    Average Approximate t      MAF            Average True t
         3          5            0.00516687 sec.       1.33841           1.00496 sec.        (default parameters)
         10         5            0.00907484 sec.       1.29561           1.21628 sec.
         3          10           0.0115723 sec.        1.25458           1.29883 sec.
         10         10           0.0128257 sec.        1.23356           1.30881 sec.
         100        10           0.0230354 sec.        1.25362           1.24013 sec.

      When the number of h-functions is increased, the execution time increased. It is observed that there is a trade-off between accuracy (MAF) and execution time as the accuracy decreases. While increasing the number of h-functions even more, it clearly leads to a significant increase in runtime, but the accuracy (MAF) remained similar to previous experiments. This indicates that excessively high numbers of h-functions is not necessary for this particular problem. 
      Increasing the number of Hashtables seems to have a slightly negative impact on the execution time and here the accuracy shows some decrease as well.
      The combination of both a larger number of h-functions and Hashtables leads to a further increase in runtime, but with a slight improvement in accuracy compared to the corresponding configurations used in the sample (default parameters: 3 h-functions, 5 Hashtables).


~~Hypercube~~
   dimension   Max Datapoints   Max Vertices  Average Approximate t    MAF      Average True t
      14            10            2            0.0018715   sec.      1.42502     1.02852 sec.        (default parameters)   
      14           200            2            0.00540427  sec.      1.26481     1.03417 sec.
      14           200           20            0.00538861  sec.      1.23713     1.03878 sec.
      7            200           20            0.00531243  sec.      1.24481     1.03146 sec.
    
    The more datapoints we examine the better results we get since with a good function that would make the datapoints well distributed, we would expect 60000/(2^d) datapoints per bucket and the first ones we examine might not be the best.However, it is likely, since we create a lot of buckets, some to have less datapoints than others, when then again the more buckets-vertices we examine the better results we will get. Lastly, projecting the data to a smaller dimension appears to be less accurate, we assume this is due to the algorithm visiting less vertices (since the buckets will be more dense) and therefore missing the chance to find a better one in a neighbouring bucket. All these optimizations, on the hyperparameters, though come with the cost of time.
    
    
~~GNN~~
   Extensions     Random Restarts   Average Approximate t       MAF         Average True t
      50               20              0.0201578 sec.         1.18568        1.28697 sec.      (default parameters)   
      50               50              0.0465412 sec.         1.17282        1.37024 sec.
      100              20              0.0157333 sec.         1.19141        0.995496 sec.

   Increasing the number of Random Restarts appears to have a positive, yet limited effect on accuracy. This decrease of MAF indicates that the number of Restarts has contributed to the extended exploration of the solution space. However there is an increase in runtime (Average Approximate t) by about double. When increasing the number of Extensions, the time decreases a lot, with a small trade off the accuracy.
   Comparing the two scenarios, it appears that the number of Extensions has a greater effect on the accuracy (MAF), while the number of Random Restarts mainly affects the execution time.


~~MRNG~~
   l     Average Approximate t       MAF         Average True t
  15       0.00111419 sec.         1.23283        1.04515 sec.         
  30       0.00131998 sec.         1.23155        1.0462 sec. 

We notice that MRNG has an astounding trade off between query time and accuracy , since it starts already from a good enough point (essentially the centroid of all the dataset) and it performs only a small number of extensions. However, we don't observe a big differentiation in results when we alter the l parameter. This might be because MAF essentially is the worst accuracy out of all the queries and in case of outliers the algorithm is going to perorm poorly since it starts from the mean and the number of extensions might not be sufficient to calculate a better estimation of its nearest neighbour.

--Conclusions--
We realize that the algorithms depend on randomness therefore the metrics on the accuracy aren't an absolutely trustworty criterion on the efficiency, when the execution of algorithms generally differs depending on CPU scheduling so we can only compare times keeping in mind the average true time for every execution as well.We notice that the best accuracy happens with the GNN since it already implements internally LSH queries during the construction of the graph.Both LSH and Hypercube have exceptionally good time metrics since their query time complexity is O(d * n^p) and with the appropriate parameters the performance is still sattisfying enough.Lastly, MRNG has the best trade off between query time and accuracy, since it has a very cautious routine when initializing the graph with neighbours and starts from an already good enough query node.


------ FIRST ASSIGNMENT-------
~~LSH~~
--Code Organization & Approach--:
The LSH implementation reads the input dataset, that has all the information about the images. The LSH constructor initializes the Locality Sensitive Hashing algorithm by creating L number of hashtables using k number of hashfunctions and then stores each image of the training dataset based on the lsh technique. After that we take a small sample of the images in the query dataset to perform the algorithms on them and call the queryNeighbours function (lsh.cpp), so as to find and compare the nearest neighbors of a given query image. In order to do that, we create two different vectors are created, one for the approximate neighbours and one for the N-Exact neighbours in a specific radious.The set H including the available hash functions out of which each g-function is constructed has bigger cardinality than k to increase randomness, arbitrarily chosen, here 30*k. The k r-operands are chosen randomly and uniquely for every g-function for the same reason.We use the amplified lsh algorithm with the querying id trick (using mod M before mod TableSize) for improved performance.We choose the first 2 images of the query set to test the algorithms.  

--Compiling--:
You can run the program using the command make run_lsh. In order to change command line arguments, you can change the line keeping the wanted format as in:
ARGSL = –d input_file –q query_file –k number_of_h_funcs_g_consists_of -L number_of_hashtables -ο output_file -Ν 11 -R 2500.If not given the default values for k,L,N,R are 4, 5, 1 and 10000 respectively.


~~HYPERCUBE~~
--Code Organization & Approach--:
The hypercube implementation creates 1 hashtable (with 2^d buckets as many as the vertices of the hypercube) using the h functions from LSH (hFunc.h and hFunc.cpp) applying a different one for each of the d-dimensions of the hypercube to hash the pixels vector (p) of the datapoint and later applies a uniformly distributed random function f mapping the h(p) to {0,1} creating a binary vector belonging to R^d which we convert to a decimal that represents the bucket our datapoint will be hashed into (store function).The f function decides on the {0,1} value with uniform random distribution and we use a map for each of the d f-functions that holds the h_i(p) as a key and the mapped number as value to maintain consistency.To perform the search, we hash the query datapoint into a bucket and until we reach maximum datapoints or maximum vertices to be checked we traverse the neighbouring buckets-vertices of the hypercube with increasing Hamming distance. The source files related with the implementation can be found in cube.cpp (and its header cube.h) and main_cluster.cpp which mainly performs input parameter processing and calls the functions from cube.cpp.

--Compiling--:
Run with make run_cube , to change the paramaters update the ARGSC in Makefile.Parameters must be given in the following order:
–d input_file –q  query_file –k dimensions_hypercube -M max_datapoints_to_be_checked -probes max_vertices_to_be_checked -ο output_file -Ν nearest_neighbours -R radius for range search.
k,M,N,R can be omitted and then they take their default values : 14, 10, 2, 1 and 10000 respectively.


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
