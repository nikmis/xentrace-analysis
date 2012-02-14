CC=gcc -Wall

all: read.c
	$(CC) read.c -o test/a.out	

clean: 
	rm -rf test/*.out
