# Paths
SRC = ./src
HEADERS = ../headers

# Compiler
CC = g++

# Compile Options
CFLAGS = -Wall -Werror -I -g -I$(HEADERS)

LSH = main 

OBJL = main.o $(SRC)/lsh.o $(SRC)/img.o $(SRC)/hFunc.o $(SRC)/hashTable.o

ARGSL = –d datasets/input.dat –q datasets/query.dat –k 3 -L 5 -ο output -Ν 11 -R 10

$(LSH): $(OBJL)
	$(CC) $(CFLAGS) $(OBJL) -o $(LSH) -lm -g

run_lsh : $(LSH)
	./$(LSH) $(ARGSL)

clean:
	rm -f $(OBJL) $(LSH)