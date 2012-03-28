CC=gcc -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	Trap.o

vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.c $(OBJS) Macros.h Externals.h
	$(CC) $(INC) $<  *.o -o ../../test/Trap.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

Trap.o: Trap.h Trap.c Macros.h Externals.h
	$(CC) $(INC) -c Trap.c

clean: 
	rm -rf ../../test/trap.out
	rm -rf *.o
