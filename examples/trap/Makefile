CC=clang -pg -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Parse.o\
	ParseHeap.o\
	CpuList.o\
	Reader.o\
	Trap.o

vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.c $(OBJS) Macros.h Trace.h Externals.h
	$(CC) $(INC) $<  *.o -o ../../bin/Trap.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h Trace.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

Parse.o: Parse.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

ParseHeap.o: ParseHeap.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

CpuList.o: CpuList.c CpuList.h list.h Event.h
	$(CC) $(INC) -c $< -o $@

Trap.o: Trap.h Trap.c Macros.h Trace.h Externals.h
	$(CC) $(INC) -c Trap.c

clean: 
	rm -rf ../../bin/Trap.out
	rm -rf *.o
