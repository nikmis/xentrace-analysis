#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Num_events.h"

NumEvents numEvents[MAX_DOMS];

int num_events_init(EventHandler *handler)
{
	memset(numEvents, 0, sizeof(NumEvents)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numEvents[i].numPorts.portList));
	}
	
	return SUCCESS;
}


int num_events_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];
	unsigned int port = event->data[2];
	
	list_head *head = &(numEvents[domId].numPorts.portList);
	NumPorts  *tmpNumPorts;

	list_for_each_entry(tmpNumPorts, head, portList)
	{
		if(tmpNumPorts->port == port)
		{
			tmpNumPorts->totalPortCount++;
			numEvents[domId].totalEventCount++;

			return SUCCESS;
		}
	}

	NumPorts *newNumPorts = (NumPorts *) malloc(sizeof(NumPorts));

	newNumPorts->port = port;
	newNumPorts->totalPortCount = 1;	/* Init. first event for given port */

	numEvents[domId].totalEventCount++;

	list_add_tail(&(newNumPorts->portList), head);

	return SUCCESS;
}

int num_events_finalize(EventHandler *handler)
{
	NumPorts  *tmpNumPorts;

	int i;
	for(i = 0; i < MAX_DOMS; i++)
	{
		/* If no events received for domain then skip the domain */
		if(!numEvents[i].totalEventCount)
		{
			continue;
		}

		unsigned int domId = (i == (MAX_DOMS - 1)) ? 0x7fff: i; 
		list_head *head = &(numEvents[i].numPorts.portList);

		printf("Domain %u:\n\tTotal Event Count = %llu\n", domId, numEvents[i].totalEventCount); 

		list_for_each_entry(tmpNumPorts, head, portList)
		{
			printf("\t\tPort %u : Count = %u\n", tmpNumPorts->port, tmpNumPorts->totalPortCount);
		}
	}

	return 0;
}

void num_events_reset(void)
{
	memset(numEvents, 0, sizeof(NumEvents)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numEvents[i].numPorts.portList));
	}
}

/* Extern Event handler struct */
struct EventHandler numEventsHandler = {
	.name = "event",
	.event_id = TRC_EVENT,
	.init = num_events_init,
	.data = (void *)numEvents,
	.process_event = num_events_handler,
	.finalize = num_events_finalize,
};
