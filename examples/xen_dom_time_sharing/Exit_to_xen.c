#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Exit_to_guest.h"
#include "Exit_to_xen.h"

DomRuntime domRuntime;

int exit_to_xen_init(EventHandler *handler)
{
	domRuntime.exitToXen = 0;
	domRuntime.cpuId = 0;

	memset(domRuntime.runtime, 0, sizeof(unsigned long long)*MAX_DOMS);

	INIT_LIST_HEAD(&(domRuntime.cpuList));

	return SUCCESS;
}

int exit_to_xen_handler(EventHandler *handler, Event *event)
{
	DomRuntime *tmpDomRuntime;
	list_head *head = &(domRuntime.cpuList);
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];

	list_for_each_entry(tmpDomRuntime, head, cpuList)
	{
		if(tmpDomRuntime->cpuId == event->cpu)
		{
			tmpDomRuntime->exitToXen = event->ns;
			tmpDomRuntime->runtime[domId] += event->ns - get_last_exit_to_guest(event->cpu, domId);
			return SUCCESS;
		}
	}

	/* cpu doesn't exist or empty list.
	 * Malloc new cpu obj and add to list.
	 */
	unsigned long long lastExitToDom = get_last_exit_to_guest(event->cpu, domId);

	if(!lastExitToDom) lastExitToDom = event->ns;

	DomRuntime *newDomRuntime = (DomRuntime *) malloc(sizeof(DomRuntime));
	
	newDomRuntime->runtime[domId] = event->ns - lastExitToDom;
	newDomRuntime->cpuId = event->cpu;
	newDomRuntime->exitToXen = event->ns;

	list_add_tail(&(newDomRuntime->cpuList), head);

	return SUCCESS;
}

int exit_to_xen_finalize(EventHandler *handler)
{
	DomRuntime *tmpDomRuntime;
	list_head *head = &(domRuntime.cpuList);
	unsigned long long totalDomRuntime[MAX_DOMS];

	memset(totalDomRuntime, 0, sizeof(unsigned long long)*MAX_DOMS);

	list_for_each_entry(tmpDomRuntime, head, cpuList)
	{	
		int i;
		for(i = 0; i < MAX_DOMS; i++)
		{
			totalDomRuntime[i] += tmpDomRuntime->runtime[i];
		}
	}

	int i;
	for(i = 0; i < MAX_DOMS; i++)
	{
		if(totalDomRuntime[i])
		{
			unsigned int j = (i == MAX_DOMS - 1) ? 0x7fff : i;
			printf("Total time spen in Domain %u = %5.3f (ms)\n", j, (float)totalDomRuntime[i]/MEGA);
		}
	}

	tmpDomRuntime = NULL;

	list_for_each_entry_reverse(tmpDomRuntime, head, cpuList)
	{
		free(tmpDomRuntime);
	}

	return SUCCESS;
}

void exit_to_xen_reset(void)
{
	DomRuntime *tmpDomRuntime;
	list_head *head = &(domRuntime.cpuList);

	list_for_each_entry_reverse(tmpDomRuntime, head, cpuList)
	{
		free(tmpDomRuntime);
	}

	domRuntime.exitToXen = 0;
	domRuntime.cpuId = 0;

	memset(domRuntime.runtime, 0, sizeof(unsigned long long)*MAX_DOMS);

	INIT_LIST_HEAD(&(domRuntime.cpuList));
}

unsigned long long get_last_exit_to_xen(unsigned int cpuId)
{
	DomRuntime *tmpDomRuntime;
	list_head *head = &(domRuntime.cpuList);

	list_for_each_entry(tmpDomRuntime, head, cpuList)
	{
		if(tmpDomRuntime->cpuId == cpuId)
		{
			return tmpDomRuntime->exitToXen;
		}
	}

	return 0;
}

struct EventHandler exitToXenHandler =
{
	.name = "exit_to_xen",
	.event_id = TRC_EXIT_TO_XEN,
	.init = exit_to_xen_init,
	.data = (void*)&domRuntime,
	.process_event = exit_to_xen_handler,
	.finalize = exit_to_xen_finalize,
};
