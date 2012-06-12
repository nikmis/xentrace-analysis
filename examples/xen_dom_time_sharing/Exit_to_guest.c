#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "list.h"
#include "Exit_to_xen.h"
#include "Exit_to_guest.h"

XenRuntime xenRuntime;

int exit_to_guest_init(EventHandler *handler)
{
	xenRuntime.runtime = 0;
	xenRuntime.cpuId = 0;
	
	memset(xenRuntime.exitToGuest, 0, sizeof(unsigned long long)*MAX_DOMS);

	INIT_LIST_HEAD(&(xenRuntime.cpuList));

	return SUCCESS;
}

int exit_to_guest_handler(EventHandler *handler, Event *event)
{
	XenRuntime *tmpXenRuntime;
	list_head *head = &(xenRuntime.cpuList);
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];

	list_for_each_entry(tmpXenRuntime, head, cpuList)
	{
		if(tmpXenRuntime->cpuId == event->cpu)
		{
			tmpXenRuntime->runtime += event->ns - get_last_exit_to_xen(event->cpu);
			tmpXenRuntime->exitToGuest[domId] = event->ns;
			return SUCCESS;
		}
	}

	/* If here, cpu object doesn't exist or list empty.
	 * Malloc new object and add to list.
	 */
	unsigned long long lastExitToXen = get_last_exit_to_xen(event->cpu);

	if(!lastExitToXen) lastExitToXen = event->ns;

	XenRuntime *newXenRuntime = (XenRuntime *) malloc(sizeof(XenRuntime));	

	newXenRuntime->runtime = event->ns - lastExitToXen;
	newXenRuntime->cpuId = event->cpu;
	
	memset(newXenRuntime->exitToGuest, 0, sizeof(unsigned long long)*MAX_DOMS);

	newXenRuntime->exitToGuest[domId] = event->ns;

	list_add_tail(&(newXenRuntime->cpuList), head);

	return SUCCESS;
}

int exit_to_guest_finalize(EventHandler *handler)
{
	XenRuntime *tmpXenRuntime;
	list_head *head = &(xenRuntime.cpuList);
	unsigned long long totalXenRuntime = 0;

	list_for_each_entry(tmpXenRuntime, head, cpuList)
	{
		totalXenRuntime += tmpXenRuntime->runtime;
	}

	printf("\nTotal time spent in Xen: %15.3f (ms)\n", (float)totalXenRuntime/MEGA);

	list_for_each_entry_reverse(tmpXenRuntime, head, cpuList)
	{
		free(tmpXenRuntime);
	}

	return SUCCESS;
}

void exit_to_guest_reset(void)
{
	XenRuntime *tmpXenRuntime;
	list_head *head = &(xenRuntime.cpuList);

	list_for_each_entry_reverse(tmpXenRuntime, head, cpuList)
	{
		free(tmpXenRuntime);
	}

	xenRuntime.runtime = 0;
	xenRuntime.cpuId = 0;
	
	memset(xenRuntime.exitToGuest, 0, sizeof(unsigned long long)*MAX_DOMS);

	INIT_LIST_HEAD(&(xenRuntime.cpuList));
}

unsigned long long get_last_exit_to_guest(unsigned int cpuId, unsigned int domId)
{
	XenRuntime *tmpXenRuntime;
	list_head *head = &(xenRuntime.cpuList);

	list_for_each_entry(tmpXenRuntime, head, cpuList)
	{
		if(tmpXenRuntime->cpuId == cpuId)
		{
			return tmpXenRuntime->exitToGuest[domId];
		}
	}

	return 0;
}

struct EventHandler exitToGuestHandler =
{
	.name = "exit_to_guest",
	.event_id = TRC_EXIT_TO_GUEST,
	.init = exit_to_guest_init,
	.data = (void*)&xenRuntime,
	.process_event = exit_to_guest_handler,
	.finalize = exit_to_guest_finalize,
};
