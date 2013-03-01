CC=clang -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	CpuList.o\
	Exit_to_xen.o\
	Exit_to_guest.o\
	Lost_records.o

vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.c $(OBJS) Macros.h Externals.h Exit_to_xen.h Exit_to_guest.h
	$(CC) $(INC) $<  $(OBJS) -o ../../bin/Xen_dom_time.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

CpuList.o: CpuList.c CpuList.h list.h Event.h
	$(CC) $(INC) -c $< -o $@

Exit_to_xen.o: Exit_to_xen.c Exit_to_xen.h Exit_to_guest.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Exit_to_guest.o: Exit_to_guest.c Exit_to_guest.h Exit_to_xen.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Lost_records.o: Lost_records.c Lost_records.h Macros.h Externals.h
	$(CC) $(INC) -c $< -o $@

clean: 
	rm -rf ../../bin/Xen_dom_time.out
	rm -rf *.o
