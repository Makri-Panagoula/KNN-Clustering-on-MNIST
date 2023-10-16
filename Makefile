# Paths
SRC = ./src
HEADERS = ../headers

# Compiler
CC = g++

# Compile Options
CFLAGS = -Wall -Werror -I -g -I$(HEADERS)

LSH = main _lsh

OBJL = main_lsh.o $(SRC)/lsh.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/hashTable.o

ARGSL = –d datasets/input.dat –q datasets/query.dat –k 3 -L 5 -ο output_lsh -Ν 11 -R 10

$(LSH): $(OBJL)
	$(CC) $(CFLAGS) $(OBJL) -o $(LSH) -lm -g

run_lsh : $(LSH)
	./$(LSH) $(ARGSL)

CUBE = main_cube

OBJC = main_cube.o

ARGSC =  –d datasets/input.dat –q datasets/query.dat –k 14 -M 10 -probes 2 -ο output_cube -Ν 1 -R 10000

clean:
	rm -f $(OBJL) $(LSH) $(OBJC) $(CUBE) output*