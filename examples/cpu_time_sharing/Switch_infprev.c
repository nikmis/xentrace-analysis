#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_infprev.h"

/* Extern Varaible to be used by Handler */
DomAllTimes domAllTimes;
static unsigned short tmpNumDoms = 0;

/* Init handler */
int switch_infprev_init(EventHandler *handler)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;
	dat->totalTime = 0;
	dat->numOfDoms = 0;

	dat->dt = (DomTime *)malloc(sizeof(struct DomTime) * MAX_DOMS);

	memset(dat->dt, 0, sizeof(struct DomTime) * MAX_DOMS);

	return 0;
}

void switch_infprev_reset(void)
{
	DomAllTimes *dat = &domAllTimes;
	dat->totalTime = 0;
	dat->numOfDoms = 0;
	tmpNumDoms = 0;

	memset(dat->dt, 0, sizeof(struct DomTime) * MAX_DOMS);
}

/* Event handler */
int switch_infprev_event(EventHandler *handler, Event *event)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;

	dat->numOfDoms = add_time_to_list(dat->dt, event);
	dat->totalTime += event->data[1];

	return 0;
}

/* Sanity Check for TSC and runtime */
//void sanity_check(Event *ev)
//{
//}
	
/* Call finalizers to display processed event data */
int switch_infprev_finalize(EventHandler *handler)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;
	unsigned short i = 0;
        
	while((i < dat->numOfDoms) && (i < MAX_DOMS))
	{
		printf("RUN: Dom ID = %5d : Cpu_Time = %15.4f(s) : Total_Time = %15.4f(s) : Cpu_Time_Share = %5.2f %%\n", 
				dat->dt[i].dom_id, 
				(float)dat->dt[i].runtime/1000000000, 
				(float)dat->totalTime/1000000000, 
				((float)dat->dt[i].runtime/dat->totalTime)*100);
		++i;
	}

	free(dat->dt);

	return 0;
}

/* Description:	Helper function for switch_infprev_event()
   Searches for existing domain id data.
   if dom exists
   	update runtime
   else
   	add new dom and its runtime
 */
unsigned short add_time_to_list(DomTime *dt, Event *ev)
{
	unsigned short i = 0;

	/* Search if domain already exists. 

	   Hash Table is the ideal abstraction for
	   looking up dom ids. Instead I am using
	   Linear Search on the list/array, since list 
	   size will be small and HT will be overkill.
	 */
	while(i < tmpNumDoms)
	{
		if(dt[i].dom_id == ev->data[0])
		{
			dt[i].runtime += ev->data[1];

			/* No new doms added */
			return tmpNumDoms;
		}
		
		++i;
	}

	if (tmpNumDoms == MAX_DOMS)
	{
		/* No new doms can be added */
		return tmpNumDoms;
	}

	/* If dom not found add new dom and its time.
	   Update num.
	 */
	dt[i].dom_id = ev->data[0];
	dt[i].runtime = ev->data[1];

	tmpNumDoms = i + 1; /* +1 since array index starts at 0 */

	return tmpNumDoms;
}


/* Extern Event handler struct */
struct EventHandler switchInfprevHandler = {
	.name = "switch_infprev",
	.event_id = TRC_SCHED_SWITCH_INFPREV,
	.init = switch_infprev_init,
	.data = (void *)&domAllTimes,
	.process_event = switch_infprev_event,
	.finalize = switch_infprev_finalize,
};
