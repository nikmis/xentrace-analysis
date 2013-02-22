#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_infprev.h"

/* Extern Varaible to be used by Handler */
static CpuLastRuntime cpuLastRuntime;

/* Init handler */
int switch_infprev_init(EventHandler *handler)
{
	CpuLastRuntime *dat = (CpuLastRuntime *)handler->data;

	INIT_LIST_HEAD(&(cpuLastRuntime.cpuList));

	memset(dat->lastRuntime, 0, sizeof(unsigned long long) * MAX_DOMS);

	return 0;
}

void switch_infprev_reset(void)
{
	CpuLastRuntime *tmpCLR;
	CpuLastRuntime *t;
	list_head *head = &(cpuLastRuntime.cpuList);

	list_for_each_entry_safe(tmpCLR, t, head, cpuList)
	{
		free(tmpCLR);
	}

	memset(tmpCLR->lastRuntime, 0, sizeof(unsigned long long)*MAX_DOMS);
	tmpCLR->cpuId = 0;

	INIT_LIST_HEAD(&(cpuLastRuntime.cpuList));
}

/* Event handler */
int switch_infprev_handler(EventHandler *handler, Event *event)
{
	CpuLastRuntime *tmpCLR;
	CpuLastRuntime *dat = (CpuLastRuntime *)handler->data;
	list_head *head = &(dat->cpuList);
	unsigned int index = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];

	if(!list_empty(head))
	{
		list_for_each_entry(tmpCLR, head, cpuList)
		{
			if(tmpCLR->cpuId == event->cpu)
			{
				tmpCLR->lastRuntime[index] = event->data[1];

				return 0;
			}
		}
	}

	/* CPU doesnt exist.
	 * Malloc a new object and add it to list.
	 */

	CpuLastRuntime *newCLR = (CpuLastRuntime *) malloc(sizeof(CpuLastRuntime));

	memset(newCLR->lastRuntime, 0, sizeof(unsigned long long)*MAX_DOMS);

	newCLR->cpuId = event->cpu;
	newCLR->lastRuntime[index] = event->data[1];

	list_add_tail(&(newCLR->cpuList), &(dat->cpuList));

	return 0;
}

/* Call finalizer to clear event data */
int switch_infprev_finalize(EventHandler *handler)
{
	CpuLastRuntime *tmpCLR;
	CpuLastRuntime *t;
	list_head *head = &(cpuLastRuntime.cpuList);

	list_for_each_entry_safe(tmpCLR, t, head, cpuList)
	{
		free(tmpCLR);
	}

	return 0;
}

/* Return last recorded runtime for switch infprev event */
unsigned long long switch_infprev_last_runtime(unsigned int cpuId, unsigned int domId)
{
	CpuLastRuntime *tmpCLR;
	list_head *head = &(cpuLastRuntime.cpuList);
	unsigned int index = (domId == 0x7fff) ? MAX_DOMS - 1: domId;

	list_for_each_entry_reverse(tmpCLR, head, cpuList)
	{
		if(tmpCLR->cpuId == cpuId)
		{
			return tmpCLR->lastRuntime[index];
		}
	}

	return 0;
}

/* Extern Event handler struct */
struct EventHandler switchInfprevHandler = {
	.name = "switch_infprev",
	.event_id = TRC_SCHED_SWITCH_INFPREV,
	.init = switch_infprev_init,
	.data = (void *)&cpuLastRuntime,
	.process_event = switch_infprev_handler,
	.finalize = switch_infprev_finalize,
};
