#ifndef __HDLR_NUM_EVENTS
#define __HDLR_NUM_EVENTS

typedef struct NumPorts
{
	unsigned int 	port;
	unsigned int 	totalPortCount;
	list_head 	portList;
} NumPorts;

typedef struct NumEvents
{
	NumPorts numPorts;
	unsigned long long totalEventCount;
} NumEvents;

int num_events_init(EventHandler *handler);
int num_events_handler(EventHandler *handler, Event *event);
int num_events_finalize(EventHandler *handler);
void num_events_reset(void);

#endif
