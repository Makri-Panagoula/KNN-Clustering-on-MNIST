# Paths
SRC = ./src
HEADERS = ../headers

# Compiler
CC = g++

# Compile Options
CFLAGS = -Wall -Werror -I -g -I$(HEADERS)

LSH = main_lsh

OBJL = main_lsh.o $(SRC)/lsh.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/hashTable.o $(SRC)/input.o

ARGSL = –d datasets/input.dat –q datasets/query.dat –k 3 -L 5 -ο output_lsh -Ν 11 -R 2500

$(LSH): $(OBJL)
	$(CC) $(CFLAGS) $(OBJL) -o $(LSH) -lm -g

run_lsh : $(LSH)
	./$(LSH) $(ARGSL)

CUBE = main_cube 

OBJC = main_cube.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/input.o $(SRC)/cube.o 

ARGSC =  –d datasets/input.dat –q datasets/query.dat –k 14 -M 20000 -probes 200 -ο output_cube -Ν 10 -R 10000

$(CUBE): $(OBJC)
	$(CC) $(CFLAGS) $(OBJC) -o $(CUBE) -lm -g

run_cube : $(CUBE)
	./$(CUBE) $(ARGSC)

CLUSTER = main_cluster

OBJCL = main_cluster.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/input.o $(SRC)/cube.o 

ARGSCL = –i datasets/input.dat –c cluster.conf -o output_cluster -complete <optional> -m <method: Classic OR LSH or Hypercube>

$(CLUSTER): $(OBJCL)
	$(CC) $(CFLAGS) $(OBJCL) -o $(CLUSTER) -lm -g

run_cluster : $(CLUSTER)
	./$(CLUSTER) $(ARGSCL)

clean:
	rm -f $(OBJL) $(LSH) $(OBJC) $(CUBE) $(OBJCL) $(CLUSTER) output*