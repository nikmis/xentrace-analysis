#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Num_Exceptions.h"
#include "Num_Exceptions_in_xen.h"

NumExceptionsInXen numExceptionsInXen;

int num_exceptions_in_xen_init(EventHandler *handler)
{
	memset(&numExceptionsInXen, 0, sizeof(NumExceptionsInXen));

	INIT_LIST_HEAD(&(numExceptionsInXen.numVectors.vectorList));
	
	return SUCCESS;
}


int num_exceptions_in_xen_handler(EventHandler *handler, Event *event)
{
	unsigned int vector = event->data[0];
	
	list_head *head = &(numExceptionsInXen.numVectors.vectorList);
	NumVectors  *tmpNumVectors;

	list_for_each_entry(tmpNumVectors, head, vectorList)
	{
		if(tmpNumVectors->vector == vector)
		{
			tmpNumVectors->totalVectorCount++;
			numExceptionsInXen.totalExceptionCount++;

			return SUCCESS;
		}
	}

	NumVectors *newNumVectors = (NumVectors *) malloc(sizeof(NumVectors));

	newNumVectors->vector = vector;
	newNumVectors->totalVectorCount = 1;	/* Init. first event for given vector */

	numExceptionsInXen.totalExceptionCount++;

	list_add_tail(&(newNumVectors->vectorList), head);

	return SUCCESS;
}

int num_exceptions_in_xen_finalize(EventHandler *handler)
{
	NumVectors  *tmpNumVectors;

	printf("\nExecption in Xen Stats\n");

	/* If no events received for domain then skip the domain */
	if(!numExceptionsInXen.totalExceptionCount)
	{
		printf("\tNo Exceptions in Xen\n");
		return SUCCESS;
	}

	list_head *head = &(numExceptionsInXen.numVectors.vectorList);

	printf("\tTotal Exception in Xen Count = %13llu\n", numExceptionsInXen.totalExceptionCount); 

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

void num_exceptions_in_xen_reset(void)
{
	NumVectors  *tmpNumVectors;
	list_head *head = &(numExceptionsInXen.numVectors.vectorList);

	memset(&numExceptionsInXen, 0, sizeof(NumExceptionsInXen));

	INIT_LIST_HEAD(&(numExceptionsInXen.numVectors.vectorList));

	/* Free memory */
	list_for_each_entry_reverse(tmpNumVectors, head, vectorList)
	{
		free(tmpNumVectors);
	}

}

/* Extern Event handler struct */
struct EventHandler numExceptionsInXenHandler = {
	.name = "exception_in_xen",
	.event_id = TRC_EXCEPTION_IN_XEN,
	.init = num_exceptions_in_xen_init,
	.data = (void *)&numExceptionsInXen,
	.process_event = num_exceptions_in_xen_handler,
	.finalize = num_exceptions_in_xen_finalize,
};
