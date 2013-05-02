CC=clang -g -Wall -m32 -O3
CPP=clang++ -Wall -m32
INC=-I../../include

OBJS=	Event.o\
	EventHandler.o\
	Reader.o\
	Parse.o\
	ParseHeap.o\
	CpuList.o\
	Queue_state.o\
	Wait_state.o\
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
	Front_shared_ring_resp_queue_blocked.o\
	Front_shared_ring_resp_queue_unblocked.o\
	Back_request_queue_interrupt_send.o\
	Back_request_queue_interrupt_receive.o\
	Back_response_queue_interrupt_send.o\
	Back_response_queue_interrupt_receive.o\
	Lost_records.o
	
vpath %.h ../../include
vpath %.h .
vpath %.c ../../src
vpath %.c .

all: main.o Histogram.o

main.o: main.c $(OBJS) Macros.h Externals.h Queue_state.h Wait_state.h Back_inflight_queue_blocked.h Back_inflight_queue_unblocked.h Back_request_queue_blocked.h Back_request_queue_unblocked.h Front_grant_queue_blocked.h Front_grant_queue_unblocked.h Front_request_queue_blocked.h Front_request_queue_unblocked.h Front_shared_ring_queue_blocked.h Front_shared_ring_queue_unblocked.h Front_shared_ring_resp_queue_blocked.h Front_shared_ring_resp_queue_unblocked.h Back_request_queue_interrupt_send.h Back_request_queue_interrupt_receive.h
	$(CC) $(INC) $<  $(OBJS) -o ../../bin/blk_queue_io
	rm -f *.o

Histogram.o: Histogram.cpp Histogram.h histo.cpp
	$(CPP) -c Histogram.cpp -o $@
	$(CPP) Histogram.o histo.cpp -o ../../bin/histogram
	rm -f *.o

Event.o: Event.c Event.h Macros.h
	$(CC) $(INC) -c $< -o $@

EventHandler.o: EventHandler.c EventHandler.h Macros.h list.h
	$(CC) $(INC) -c $< -o $@

Reader.o: Reader.c Reader.h Macros.h
	$(CC) $(INC) -c $< -o $@

Parse.o: Parse.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

ParseHeap.o: ParseHeap.c Parse.h Macros.h Trace.h
	$(CC) $(INC) -c $< -o $@

CpuList.o: CpuList.c CpuList.h list.h Event.h
	$(CC) $(INC) -c $< -o $@

Queue_state.o: Queue_state.c Queue_state.h Lost_records.h Event.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Wait_state.o: Wait_state.c Wait_state.h Event.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_inflight_queue_blocked.o: Back_inflight_queue_blocked.c Back_inflight_queue_blocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_inflight_queue_unblocked.o: Back_inflight_queue_unblocked.c Back_inflight_queue_unblocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_request_queue_blocked.o: Back_request_queue_blocked.c Back_request_queue_blocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_request_queue_unblocked.o: Back_request_queue_unblocked.c Back_request_queue_unblocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_grant_queue_blocked.o: Front_grant_queue_blocked.c Front_grant_queue_blocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_grant_queue_unblocked.o: Front_grant_queue_unblocked.c Front_grant_queue_unblocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_request_queue_blocked.o: Front_request_queue_blocked.c Front_request_queue_blocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_request_queue_unblocked.o: Front_request_queue_unblocked.c Front_request_queue_unblocked.h Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_shared_ring_queue_blocked.o: Front_shared_ring_queue_blocked.c Front_shared_ring_queue_blocked.h Queue_state.c Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_shared_ring_queue_unblocked.o: Front_shared_ring_queue_unblocked.c Front_shared_ring_queue_unblocked.h Queue_state.c Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_shared_ring_resp_queue_blocked.o: Front_shared_ring_resp_queue_blocked.c Front_shared_ring_resp_queue_blocked.h Queue_state.c Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Front_shared_ring_resp_queue_unblocked.o: Front_shared_ring_resp_queue_unblocked.c Front_shared_ring_resp_queue_unblocked.h Queue_state.c Queue_state.h Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_request_queue_interrupt_send.o: Back_request_queue_interrupt_send.c Back_request_queue_interrupt_send.h Wait_state.h Wait_state.c Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_request_queue_interrupt_receive.o: Back_request_queue_interrupt_receive.c Back_request_queue_interrupt_receive.h Wait_state.h Wait_state.c Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_response_queue_interrupt_send.o: Back_response_queue_interrupt_send.c Back_response_queue_interrupt_send.h Wait_state.h Wait_state.c Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Back_response_queue_interrupt_receive.o: Back_response_queue_interrupt_receive.c Back_response_queue_interrupt_receive.h Wait_state.h Wait_state.c Macros.h list.h Trace.h
	$(CC) $(INC) -c $< -o $@

Lost_records.o: Lost_records.c Lost_records.h Front_shared_ring_queue_unblocked.c Queue_state.h Macros.h Externals.h
	$(CC) $(INC) -c $< -o $@

clean: 
	rm -rf ../../bin/blk_queue_io.out
	rm -rf *.o
