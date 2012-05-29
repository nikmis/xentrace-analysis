CC=clang -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	Switch_infprev.o\
	Lost_records.o

vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.c $(OBJS) Macros.h Externals.h
	$(CC) $(INC) $<  *.o -o ../../test/Cpu_util.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

Switch_infprev.o: Switch_infprev.h Switch_infprev.c Macros.h Externals.h
	$(CC) $(INC) -c Switch_infprev.c

Lost_records.o: Lost_records.c Lost_records.h Macros.h Externals.h Switch_infprev.h
	$(CC) $(INC) -c Lost_records.c
clean: 
	rm -rf ../../test/Cpu_util.out
	rm -rf *.o
