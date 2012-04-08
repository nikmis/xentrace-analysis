CC=gcc -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	Switch_infprev.o\
	Switch_infnext.o

vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.c $(OBJS) Macros.h Externals.h
	$(CC) $(INC) $<  *.o -o ../../test/Cpu_wait.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

Switch_infprev.o: Switch_infprev.h Switch_infprev.c Macros.h Externals.h
	$(CC) $(INC) -c Switch_infprev.c

Switch_infnext.o: Switch_infnext.h Switch_infnext.c Macros.h Externals.h
	$(CC) $(INC) -c Switch_infnext.c

clean: 
	rm -rf ../../test/Cpu_wait.out
	rm -rf *.o
