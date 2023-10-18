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

OBJC = main_cube.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/input.o $(SRC)/cube.o $(SRC)/hamming.o

ARGSC =  –d datasets/input.dat –q datasets/query.dat –k 14 -M 10 -probes 2 -ο output_cube -Ν 1 -R 10000

$(CUBE): $(OBJC)
	$(CC) $(CFLAGS) $(OBJC) -o $(CUBE) -lm -g

run_cube : $(CUBE)
	./$(CUBE) $(ARGSC)

clean:
	rm -f $(OBJL) $(LSH) $(OBJC) $(CUBE) output*