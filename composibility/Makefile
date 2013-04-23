CC=clang -g -Wall -m32
INC=-I../include

OBJS=	Event.o\
	Parse.o\
	ParseHeap.o\
	Pipe.o\
	Stages.o\

vpath %.h ../include
vpath %.h .
vpath %.c ../src
vpath %.c .

all: pipe or
	rm -f *.o

pipe: pipe_example.c $(OBJS) Macros.h Trace.h
	$(CC) $(INC) $<  *.o -o ../bin/pipe_example.out	

or: or_example.c $(OBJS) Macros.h Trace.h
	$(CC) $(INC) $<  *.o -o ../bin/or_example.out	

Event.o: Event.c Event.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

Parse.o: Parse.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

ParseHeap.o: ParseHeap.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

Pipe.o: Pipe.c Pipe.h uthash.h
	$(CC) $(INC) -c $< -o $@

Stages.o: Stages.c Stages.h
	$(CC) $(INC) -c $< -o $@

clean: 
	rm -rf ../bin/*_example.out
	rm -rf *.o
