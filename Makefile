#Declarations
CC=gcc
CFlags= -Wall
all:
	$(CC) $(CFlags) main.c utils.c -o output.out
clean:
	rm -rf *o *out
