# Paths
SRC = ./src
HEADERS = ../headers

# Compiler
CC = g++

# Compile Options
CFLAGS = -Wall -Werror -I -g -I$(HEADERS)

#----------------------LSH make, run & valgrind-----------------------------

LSH = main_lsh
OBJ_L = $(SRC)/lsh.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/hashTable.o $(SRC)/input.o
OBJ_LSH = main_lsh.o $(OBJ_L)

ARGSL = –d datasets/input.dat –q datasets/query.dat –k 3 -L 5 -ο output_lsh -Ν 10 -R 2500

$(LSH): $(OBJ_LSH)
	$(CC) $(CFLAGS) $(OBJ_LSH) -o $(LSH) -lm -g3

run_lsh: $(LSH)
	./$(LSH) $(ARGSL)

valgrind_lsh:	$(LSH)
	valgrind --track-origins=yes --leak-check=full ./$(LSH) $(ARGSL)


#----------------------Hypercube make, run & valgrind----------------------

CUBE = main_cube 
OBJC = $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/input.o $(SRC)/cube.o 
OBJ_CUBE = main_cube.o $(OBJC)

ARGSC = –d datasets/input.dat –q datasets/query.dat –k 7 -M 200 -probes 20 -ο output_cube -Ν 10 -R 10000

$(CUBE): $(OBJ_CUBE)
	$(CC) $(CFLAGS) $(OBJ_CUBE) -o $(CUBE) -lm -g3

run_cube: $(CUBE)
	./$(CUBE) $(ARGSC)

valgrind_cube:	$(CUBE)
	valgrind --track-origins=yes --leak-check=full ./$(CUBE) $(ARGSC)


#----------------------Cluster make, run & valgrind----------------------

CLUSTER = main_cluster
OBJ_CL = main_cluster.o $(OBJ_L) $(SRC)/cube.o $(SRC)/cluster.o

ARGSCL = –i datasets/input.dat –c cluster.conf -o output_cluster -m Classic

$(CLUSTER): $(OBJ_CL)
	$(CC) $(CFLAGS) $(OBJ_CL) -o $(CLUSTER) -lm -g3

run_cluster: $(CLUSTER) $(LSH) $(CUBE)
	./$(CLUSTER) $(ARGSCL)

valgrind_cluster:  $(CLUSTER)
	valgrind --track-origins=yes --leak-check=full ./$(CLUSTER) $(ARGSCL)


#----------------------Graph Search make, run & valgrind----------------------

GRAPH_SEARCH = graph_search
OBJS_G = graph_search.o $(OBJ_L) $(SRC)/cube.o
ARGS_G = –d datasets/input.dat –q datasets/query.dat –k 7 -E 200 -R -N 5 -l 15 -m 1 -o output_graph

$(GRAPH_SEARCH): $(OBJS_G)
	$(CC) $(CFLAGS) $(OBJS_G) -o $(GRAPH_SEARCH) -lm -g3

run_graph: $(GRAPH_SEARCH) $(LSH) $(CUBE)
	./$(GRAPH_SEARCH) $(ARGS_G)

valgrind_graph:  $(GRAPH_SEARCH)
	valgrind --track-origins=yes --leak-check=full ./$(GRAPH_SEARCH) $(ARGS_G)

clean:
	rm -f $(OBJS_G) $(OBJ_CL) $(OBJ_LSH) $(OBJ_CUBE) $(LSH) $(CUBE) $(CLUSTER) $(GRAPH_SEARCH) output*