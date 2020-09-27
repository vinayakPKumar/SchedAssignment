#Declarations
CC=gcc
CFlags= -Wall
all:
	$(CC) $(CFlags) main.c -o output.out
clean:
	rm -rf *o *out
