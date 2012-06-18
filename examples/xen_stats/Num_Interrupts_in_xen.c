#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Num_Exceptions.h"
#include "Num_Interrupts_in_xen.h"

NumInterruptsInXen numInterruptsInXen;

int num_interrupts_in_xen_init(EventHandler *handler)
{
	memset(&numInterruptsInXen, 0, sizeof(NumInterruptsInXen));

	INIT_LIST_HEAD(&(numInterruptsInXen.numVectors.vectorList));
	
	return SUCCESS;
}


int num_interrupts_in_xen_handler(EventHandler *handler, Event *event)
{
	unsigned int vector = event->data[0];
	
	list_head *head = &(numInterruptsInXen.numVectors.vectorList);
	NumVectors  *tmpNumVectors;

	list_for_each_entry(tmpNumVectors, head, vectorList)
	{
		if(tmpNumVectors->vector == vector)
		{
			tmpNumVectors->totalVectorCount++;
			numInterruptsInXen.totalInterruptCount++;

			return SUCCESS;
		}
	}

	NumVectors *newNumVectors = (NumVectors *) malloc(sizeof(NumVectors));

	newNumVectors->vector = vector;
	newNumVectors->totalVectorCount = 1;	/* Init. first event for given vector */

	numInterruptsInXen.totalInterruptCount++;

	list_add_tail(&(newNumVectors->vectorList), head);

	return SUCCESS;
}

int num_interrupts_in_xen_finalize(EventHandler *handler)
{
	NumVectors  *tmpNumVectors;

	printf("\nInterrupts in Xen Stats\n");

	/* If no events received for domain then skip the domain */
	if(!numInterruptsInXen.totalInterruptCount)
	{
		printf("\tNo Interrupts in Xen\n");
		return SUCCESS;
	}

	list_head *head = &(numInterruptsInXen.numVectors.vectorList);

	printf("\tTotal Interrupts in Xen Count = %13llu\n", numInterruptsInXen.totalInterruptCount); 

	list_for_each_entry(tmpNumVectors, head, vectorList)
	{
		printf("\t\tVector %4u : Count = %7u\n", tmpNumVectors->vector, tmpNumVectors->totalVectorCount);
	}

	/* Free memory */
	list_for_each_entry_reverse(tmpNumVectors, head, vectorList)
	{
		free(tmpNumVectors);
	}

	return 0;
}

void num_interrupts_in_xen_reset(void)
{
	NumVectors  *tmpNumVectors;
	list_head *head = &(numInterruptsInXen.numVectors.vectorList);

	memset(&numInterruptsInXen, 0, sizeof(NumInterruptsInXen));

	INIT_LIST_HEAD(&(numInterruptsInXen.numVectors.vectorList));

	/* Free memory */
	list_for_each_entry_reverse(tmpNumVectors, head, vectorList)
	{
		free(tmpNumVectors);
	}

}

/* Extern Event handler struct */
struct EventHandler numInterruptsInXenHandler = {
	.name = "interrupt_in_xen",
	.event_id = TRC_INTERRUPT_IN_XEN,
	.init = num_interrupts_in_xen_init,
	.data = (void *)&numInterruptsInXen,
	.process_event = num_interrupts_in_xen_handler,
	.finalize = num_interrupts_in_xen_finalize,
};
