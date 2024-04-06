**_Implementation details and conducted experiments on algorithms' parameters can be found on `Details.txt`_**
## KNN -Algorithms
**LSH**

**Hypercube**

**Graph Based ANNs**
- GNN
- MRNG


## Clustering Algorithms Overview

**Centroid Initialization**:  
Regardless of the assignment algorithm, centroid initialization employs the k-means++ algorithm. In k-means, the first centroid is chosen randomly, while subsequent ones are selected based on their squared minimum distances from initialized centroids (d_i^2, without normalization).

**Lloyd's Algorithm**:  
Lloyd's algorithm iteratively compares datapoints' current cluster with the closest centroid. Each datapoint holds a flag indicating its cluster assignment. Reassignment involves removing from the current cluster and inserting into the new one, updating centroids accordingly.

**Reverse Assignment**:  
Regardless of data structure (LSH/Hypercube), the approach remains consistent. Starting from a radius, a range search locates centroids' nearest datapoints. Assignment occurs before radius doubling. Silhouette Score, assessing clustering quality, indicates small fluctuation for chosen cluster count, but is sensitive to initial centroid selection.

**Evaluation**:  
In the end, we evaluate the Silhouette Score to assess clustering quality, observing fluctuations and considering the impact of initial centroid selection on performance.

## Dimensionality Reduction

**Approach**:  
To enhance algorithm efficiency, we explore dimensionality reduction using an autoencoder. Hyperparameter tuning is conducted on:
- Dropout rate
- Epochs
- Batch size
- Kernel size
- Number of filters
- Filters per layer
- Latent dimension
- Patience
Experimentation details are provided in the `experiment.ipynb` notebook, accompanied by annotated diagrams. To prevent overfitting, we employ regularization techniques such as batch normalization and dropout in each layer, supplemented by early stopping . The best model is saved and utilized to reduce dimensions in `reduce.py` using initial files.

**Execution**:  
To execute the second assignment in the latent space, datasets are specified after the `-d` and `-q` commands, followed by `-id` for the datafile and `-iq` for the queryfile in the initial dataspace. For exhaustive search in the new space, `m=3` is specified. Similarly, when performing clustering in the new dimension, `-id` is included for the datafile in the initial dataspace. Clustering is then conducted, and nearest neighbors of each centroid are identified and projected back into the initial dataspace. Silhouette estimation is subsequently conducted by projecting the remaining datapoints in the clusters.


## Usage

- LSH:
  - Run with `make run_lsh`.
  - Modify command line arguments using the format:
    ```plaintext
    ARGSL = –d input_file –q query_file –k number_of_h_funcs_g_consists_of -L number_of_hashtables -ο output_file -Ν 11 -R 2500.
    ```
  - Default values for k, L, N, and R are 4, 5, 1, and 10000 respectively.

- Hypercube:
  - Run with `make run_cube`.
  - To change parameters, update the `ARGSC` in Makefile.
  - Parameters must be given in the following order:
    ```plaintext
    –d input_file –q query_file –k dimensions_hypercube -M max_datapoints_to_be_checked -probes max_vertices_to_be_checked -ο output_file -Ν nearest_neighbours -R radius for range search.
    ```
  - k, M, N, R can be omitted and then they take their default values: 14, 10, 2, 1, and 10000 respectively.

- Clustering:
  - Run with `make run_cluster`.
  - To experiment with clustering methods, modify the last argument:
    ```plaintext
    ARGSCL = –i datasets/input.dat –c cluster.conf -o output_cluster -m Classic.
    ```
  - Methods available are "Classic", "LSH", "Hypercube".
  - Add "-complete" after the output file to display cluster contents.

- Graph Based ANNs:
  - Run with `make run_graph` with either m = 1 or m = 2 for GNN or MRNG respectively.
  - To modify command line arguments, adjust the line according to the desired format:
    ```plaintext
    ARGS_G = –d datasets/input.dat –q datasets/query.dat -m <value> -o output_graph .
    ```
  - Default values for k, E, R, and N can be used or omitted, as they are not utilized in MRNG.
  - For m = 2, the default value for l is 20, and for m = 1, l can be omitted as it's not used in GNN.
  - Query and output files can also be provided later in the program's execution.
