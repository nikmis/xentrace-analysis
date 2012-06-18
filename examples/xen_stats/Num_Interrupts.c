#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Num_Exceptions.h"
#include "Num_Interrupts.h"

NumInterrupts numInterrupts[MAX_DOMS];

int num_interrupts_init(EventHandler *handler)
{
	memset(numInterrupts, 0, sizeof(NumInterrupts)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numInterrupts[i].numVectors.vectorList));
	}
	
	return SUCCESS;
}


int num_interrupts_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];
	unsigned int vector = event->data[2];
	
	list_head *head = &(numInterrupts[domId].numVectors.vectorList);
	NumVectors  *tmpNumVectors;

	list_for_each_entry(tmpNumVectors, head, vectorList)
	{
		if(tmpNumVectors->vector == vector)
		{
			tmpNumVectors->totalVectorCount++;
			numInterrupts[domId].totalInterruptCount++;

			return SUCCESS;
		}
	}

	NumVectors *newNumVectors = (NumVectors *) malloc(sizeof(NumVectors));

	newNumVectors->vector = vector;
	newNumVectors->totalVectorCount = 1;	/* Init. first event for given vector */

	numInterrupts[domId].totalInterruptCount++;

	list_add_tail(&(newNumVectors->vectorList), head);

	return SUCCESS;
}

int num_interrupts_finalize(EventHandler *handler)
{
	NumVectors  *tmpNumVectors;

	int i;
	printf("\nInterrupt Stats\n");
	for(i = 0; i < MAX_DOMS; i++)
	{
		/* If no events received for domain then skip the domain */
		if(!numInterrupts[i].totalInterruptCount)
		{
			continue;
		}

		unsigned int domId = (i == (MAX_DOMS - 1)) ? 0x7fff: i; 
		list_head *head = &(numInterrupts[i].numVectors.vectorList);

		printf("  Domain %5u:\n\tTotal Interrupt Count = %13llu\n", domId, numInterrupts[i].totalInterruptCount); 

		list_for_each_entry(tmpNumVectors, head, vectorList)
		{
			printf("\t\tVector %4u : Count = %7u\n", tmpNumVectors->vector, tmpNumVectors->totalVectorCount);
		}
	}

	for(i = 0; i < MAX_DOMS; i++)
	{
		list_head *head = &(numInterrupts[i].numVectors.vectorList);
		/* Free memory */
		list_for_each_entry_reverse(tmpNumVectors, head, vectorList)
		{
			free(tmpNumVectors);
		}
	}

	return 0;
}

void num_interrupts_reset(void)
{
	NumVectors  *tmpNumVectors;

	memset(numInterrupts, 0, sizeof(NumInterrupts)*MAX_DOMS);

	int i;
	for(i = 0; i < MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numInterrupts[i].numVectors.vectorList));

		/* Free memory */
		list_head *head = &(numInterrupts[i].numVectors.vectorList);
		
		list_for_each_entry_reverse(tmpNumVectors, head, vectorList)
		{
			free(tmpNumVectors);
		}
	}
}

/* Extern Event handler struct */
struct EventHandler numInterruptsHandler = {
	.name = "interrupt",
	.event_id = TRC_INTERRUPT,
	.init = num_interrupts_init,
	.data = (void *)numInterrupts,
	.process_event = num_interrupts_handler,
	.finalize = num_interrupts_finalize,
};
