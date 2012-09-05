CC=clang -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	Back_inflight_queue_blocked.o\
	Back_inflight_queue_unblocked.o\
	Back_request_queue_blocked.o\
	Back_request_queue_unblocked.o\
	Front_grant_queue_blocked.o\
	Front_grant_queue_unblocked.o\
	Front_request_queue_blocked.o\
	Front_request_queue_unblocked.o\
	Front_shared_ring_queue_blocked.o\
	Front_shared_ring_queue_unblocked.o\
	Lost_records.o

vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.c $(OBJS) Macros.h Externals.h Back_inflight_queue_blocked.h Back_inflight_queue_unblocked.h Back_request_queue_blocked.h Back_request_queue_unblocked.h Front_grant_queue_blocked.h Front_grant_queue_unblocked.h Front_request_queue_blocked.h Front_request_queue_unblocked.h Front_shared_ring_queue_blocked.h Front_shared_ring_queue_unblocked.h
	$(CC) $(INC) $<  $(OBJS) -o ../../test/blk_queue_io.out	
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

Back_inflight_queue_blocked.o: Back_inflight_queue_blocked.c Back_inflight_queue_blocked.h Exit_to_guest.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_inflight_queue_unblocked.o: Back_inflight_queue_unblocked.c Back_inflight_queue_unblocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_request_queue_blocked.o: Back_request_queue_blocked.c Back_request_queue_blocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_request_queue_unblocked.o: Back_request_queue_unblocked.c Back_request_queue_unblocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_grant_queue_blocked.o: Front_grant_queue_blocked.c Front_grant_queue_blocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_grant_queue_unblocked.o: Front_grant_queue_unblocked.c Front_grant_queue_unblocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_request_queue_blocked.o: Front_request_queue_blocked.c Front_request_queue_blocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_request_queue_unblocked.o: Front_request_queue_unblocked.c Front_request_queue_unblocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_shared_ring_queue_blocked.o: Front_shared_ring_queue_blocked.c Front_shared_ring_queue_blocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_shared_ring_queue_unblocked.o: Front_shared_ring_queue_unblocked.c Front_shared_ring_queue_unblocked.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Lost_records.o: Lost_records.c Lost_records.h Macros.h Externals.h
	$(CC) $(INC) -c $< -o $@

clean: 
	rm -rf ../../test/blk_queue_io.out
	rm -rf *.o
