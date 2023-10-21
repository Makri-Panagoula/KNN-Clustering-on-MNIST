# Paths
SRC = ./src
HEADERS = ../headers

# Compiler
CC = g++

# Compile Options
CFLAGS = -Wall -Werror -I -g -I$(HEADERS)

LSH = main_lsh
OBJ_L = $(SRC)/lsh.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/hashTable.o $(SRC)/input.o
OBJ_LSH = main_lsh.o $(OBJ_L)

ARGSL = –d datasets/input.dat –q datasets/query.dat –k 3 -L 5 -ο output_lsh -Ν 11 -R 2500

$(LSH): $(OBJ_LSH)
	$(CC) $(CFLAGS) $(OBJ_LSH) -o $(LSH) -lm -g3

run_lsh : $(LSH)
	./$(LSH) $(ARGSL)

CUBE = main_cube 
OBJC = $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/input.o $(SRC)/cube.o 
OBJ_CUBE = main_cube.o $(OBJC)

ARGSC =  –d datasets/input.dat –q datasets/query.dat –k 14 -M 20000 -probes 200 -ο output_cube -Ν 10 -R 10000

$(CUBE): $(OBJ_CUBE)
	$(CC) $(CFLAGS) $(OBJ_CUBE) -o $(CUBE) -lm -g3

run_cube : $(CUBE)
	./$(CUBE) $(ARGSC)

CLUSTER = main_cluster
OBJC = $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/input.o $(SRC)/cube.o 
OBJ_CL = main_cluster.o $(OBJ_L) $(SRC)/cube.o $(SRC)/cluster.o

ARGSCL = –i datasets/input.dat –c cluster.conf -o output_cluster -m Classic

$(CLUSTER): $(OBJ_CL)
	$(CC) $(CFLAGS) $(OBJ_CL) -o $(CLUSTER) -lm -g3

run_cluster : $(CLUSTER) $(LSH) $(CUBE)
	./$(CLUSTER) $(ARGSCL)

valgrind_cluster :  $(OBJ_CL)
	valgrind --leak-check=full ./$(CLUSTER) $(ARGSCL)
clean:
	rm -f $(OBJ_CL) $(OBJ_LSH) $(OBJ_CUBE) $(CUBE) $(LSH) $(CLUSTER) output*