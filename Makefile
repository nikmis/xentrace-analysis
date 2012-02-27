CC=clang -Wall
INC=-I./include -I./include/handlers

HDLR_SRC=$(wildcard src/handlers/*.c)
HDLR_INC=$(wildcard include/handlers/*.h)

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	Handlers.o

vpath %.h ./include
vpath %.h ./include/handlers
vpath %.c ./src
vpath %.c ./src/handlers

all: main.c $(OBJS) Macros.h Externals.h
	$(CC) $(INC) $<  *.o -o test/a.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

Handlers.o: $(HDLR_SRC) $(HDLR_INC) Macros.h Externals.h
	$(CC) $(INC) -c $(filter %.c,$^)

clean: 
	rm -rf test/*.out
	rm -rf *.o
