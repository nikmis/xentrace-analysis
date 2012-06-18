#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Num_Exceptions.h"

NumExceptions numExceptions[MAX_DOMS];

int num_exceptions_init(EventHandler *handler)
{
	memset(numExceptions, 0, sizeof(NumExceptions)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numExceptions[i].numVectors.vectorList));
	}
	
	return SUCCESS;
}


int num_exceptions_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];
	unsigned int vector = event->data[2];
	
	list_head *head = &(numExceptions[domId].numVectors.vectorList);
	NumVectors  *tmpNumVectors;

	list_for_each_entry(tmpNumVectors, head, vectorList)
	{
		if(tmpNumVectors->vector == vector)
		{
			tmpNumVectors->totalVectorCount++;
			numExceptions[domId].totalExceptionCount++;

			return SUCCESS;
		}
	}

	NumVectors *newNumVectors = (NumVectors *) malloc(sizeof(NumVectors));

	newNumVectors->vector = vector;
	newNumVectors->totalVectorCount = 1;	/* Init. first event for given vector */

	numExceptions[domId].totalExceptionCount++;

	list_add_tail(&(newNumVectors->vectorList), head);

	return SUCCESS;
}

int num_exceptions_finalize(EventHandler *handler)
{
	NumVectors  *tmpNumVectors;

	int i;
	printf("\nExecption Stats\n");
	for(i = 0; i < MAX_DOMS; i++)
	{
		/* If no events received for domain then skip the domain */
		if(!numExceptions[i].totalExceptionCount)
		{
			continue;
		}

		unsigned int domId = (i == (MAX_DOMS - 1)) ? 0x7fff: i; 
		list_head *head = &(numExceptions[i].numVectors.vectorList);

		printf("Domain %5u:\n\tTotal Exception Count = %13llu\n", domId, numExceptions[i].totalExceptionCount); 

		list_for_each_entry(tmpNumVectors, head, vectorList)
		{
			printf("\t\tVector %4u : Count = %7u\n", tmpNumVectors->vector, tmpNumVectors->totalVectorCount);
		}
	}

	return 0;
}

void num_exceptions_reset(void)
{
	memset(numExceptions, 0, sizeof(NumExceptions)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numExceptions[i].numVectors.vectorList));
	}
}

/* Extern Event handler struct */
struct EventHandler numExceptionsHandler = {
	.name = "exception",
	.event_id = TRC_EXCEPTION,
	.init = num_exceptions_init,
	.data = (void *)numExceptions,
	.process_event = num_exceptions_handler,
	.finalize = num_exceptions_finalize,
};
