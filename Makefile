CC=gcc -Wall
INC=-I./include

vpath %.h ./include
vpath %.c src

all: main.c Event.o EventHandler.o Reader.o Macros.h
	$(CC) $(INC) $< Event.o EventHandler.o Reader.o -o test/a.out	
	rm -f Event.o EventHandler.o Reader.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

clean: 
	rm -rf test/*.out
	rm -rf *.o
