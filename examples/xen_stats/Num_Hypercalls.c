#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Num_Hypercalls.h"

NumHypercalls numHypercalls[MAX_DOMS];

int num_hypercalls_init(EventHandler *handler)
{
	memset(numHypercalls, 0, sizeof(NumHypercalls)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numHypercalls[i].numTypes.typeList));
	}
	
	return SUCCESS;
}


int num_hypercalls_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];
	unsigned int type = event->data[2];
	
	list_head *head = &(numHypercalls[domId].numTypes.typeList);
	NumTypes  *tmpNumTypes;

	list_for_each_entry(tmpNumTypes, head, typeList)
	{
		if(tmpNumTypes->type == type)
		{
			tmpNumTypes->totalTypeCount++;
			numHypercalls[domId].totalHypercallCount++;

			return SUCCESS;
		}
	}

	NumTypes *newNumTypes = (NumTypes *) malloc(sizeof(NumTypes));

	newNumTypes->type = type;
	newNumTypes->totalTypeCount = 1;	/* Init. first event for given type */

	numHypercalls[domId].totalHypercallCount++;

	list_add_tail(&(newNumTypes->typeList), head);

	return SUCCESS;
}

int num_hypercalls_finalize(EventHandler *handler)
{
	unsigned int flag = 0;
	NumTypes  *tmpNumTypes;

	int i;
	printf("\nHypercall Stats\n");
	for(i = 0; i < MAX_DOMS; i++)
	{
		/* If no events received for domain then skip the domain */
		if(!numHypercalls[i].totalHypercallCount)
		{
			continue;
		}

		flag = 1;
		unsigned int domId = (i == (MAX_DOMS - 1)) ? 0x7fff: i; 
		list_head *head = &(numHypercalls[i].numTypes.typeList);

		printf("  Domain %5u:\n\tTotal Hypercall Count = %13llu\n", domId, numHypercalls[i].totalHypercallCount); 

		list_for_each_entry(tmpNumTypes, head, typeList)
		{
			printf("\t\tNumber %4u : Count = %7u\n", tmpNumTypes->type, tmpNumTypes->totalTypeCount);
		}
	}

	if(!flag)	printf("\tNo Hypercalls found\n");

	for(i = 0; i < MAX_DOMS; i++)
	{
		/* Free memory */
		list_head *head = &(numHypercalls[i].numTypes.typeList);

		list_for_each_entry_reverse(tmpNumTypes, head, typeList)
		{
			free(tmpNumTypes);
		}
	}

	return 0;
}

void num_hypercalls_reset(void)
{
	NumTypes  *tmpNumTypes;

	memset(numHypercalls, 0, sizeof(NumHypercalls)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(numHypercalls[i].numTypes.typeList));

		/* Free memory */
		list_head *head = &(numHypercalls[i].numTypes.typeList);

		list_for_each_entry_reverse(tmpNumTypes, head, typeList)
		{
			free(tmpNumTypes);
		}
	}
}

/* Extern Event handler struct */
struct EventHandler numHypercallsHandler = {
	.name = "hypercall",
	.event_id = TRC_HYPERCALL,
	.init = num_hypercalls_init,
	.data = (void *)numHypercalls,
	.process_event = num_hypercalls_handler,
	.finalize = num_hypercalls_finalize,
};
