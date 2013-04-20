CC=clang -Wall -m32 -O3
INC=-I../../include

OBJS=	Event.o\
	Parse.o\
	ParseHeap.o\
	Pipe.o\
	Stages.o\
	Trap.o

vpath %.h ../include
vpath %.h .
vpath %.c ../src
vpath %.c .

all: main.c $(OBJS) Macros.h Trace.h Externals.h
	$(CC) $(INC) $<  *.o -o ../bin/Compo-Trap.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

Parse.o: Parse.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

ParseHeap.o: ParseHeap.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

Pipe.o: Pipe.c Pipe.h 
	$(CC) $(INC) -c $< -o $@

Stages.o: Stages.c Stages.h
	$(CC) $(INC) -c $< -o $@

Trap.o: Trap.h Trap.c Macros.h Trace.h Externals.h
	$(CC) $(INC) -c Trap.c

clean: 
	rm -rf ../bin/Compo-Trap.out
	rm -rf *.o
