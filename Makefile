# Compiler
CC = g++

# Compile Options
CFLAGS = -Wall -Werror -I -g 

LSH = lsh 

OBJL = lsh.o

ARGSL = –d input –q –k 3 -L 5 -ο output -Ν 11 -R 10

$(LSH): $(OBJL)
	$(CC) $(CFLAGS) $(OBJL) -o $(LSH)

run_lsh : $(LSH)
	./$(LSH) $(ARGSL)

clean:
	rm -f $(OBJL) $(LSH)