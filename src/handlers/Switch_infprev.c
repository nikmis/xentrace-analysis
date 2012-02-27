#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Switch_infprev.h"

/* Extern Varaible to be used by Handler */
DomAllTimes dom_all_times;

/* Init handler */
int switch_infprev_init(EventHandler *handler)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;
	dat->total_time = 0;
	dat->num_of_doms = 0;

	dat->dt = (DomTime *)malloc(sizeof(struct DomTime) * MAX_DOMS);

	memset(dat->dt, 0, sizeof(struct DomTime) * MAX_DOMS);

	return 0;
}

/* Event handler */
int switch_infprev_event(EventHandler *handler, Event *event)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;

	dat->num_of_doms = add_time_to_list(dat->dt, event);
	dat->total_time += event->data[1];

	return 0;
}

/* Call finalizers to display processed event data */
int switch_infprev_finalize(EventHandler *handler)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;
	unsigned short i = 0;

	while((i < dat->num_of_doms) && (i < MAX_DOMS))
	{
		printf("Dom ID = %d Cpu_Time = %lld Total_Time = %lld Cpu_Time_Share = %.2f %%\n", 
				dat->dt[i].dom_id, 
				dat->dt[i].runtime, 
				dat->total_time, 
				((float)dat->dt[i].runtime/dat->total_time)*100);
		++i;
	}

	return 0;
}

/* Description:	Helper function for generate_cpu_share_stats()
   Searches for existing domain id data.
   if dom exists
   	update runtime
   else
   	add new dom and its runtime
 */
unsigned short add_time_to_list(DomTime *dt, Event *ev)
{
	static unsigned short num = 0;
	unsigned short i = 0;

	/* Search if domain already exists. 

	   Hash Table is the ideal abstraction for
	   looking up dom ids. Instead I am using
	   Linear Search on the list/array, since list 
	   size will be small and HT will be overkill.
	 */
	while(i < num)
	{
		if(dt[i].dom_id == ev->data[0])
		{
			dt[i].runtime += ev->data[1];

			/* No new doms added */
			return num;
		}
		
		++i;
	}

	if (num == MAX_DOMS)
	{
		/* No new doms can be added */
		return num;
	}

	/* If dom not found add new dom and its time.
	   Update num.
	 */
	dt[i].dom_id = ev->data[0];
	dt[i].runtime = ev->data[1];

	num = i + 1; /* +1 since array index starts at 0 */

	return num;
}
		
/* Extern Event handler struct */
struct EventHandler switch_infprev_handler = {
	.name = "switch_infprev",
	.event_id = SWITCH_INFPREV,
	.init = switch_infprev_init,
	.data = (void *)&dom_all_times,
	.process_event = switch_infprev_event,
	.finalize = switch_infprev_finalize,
};
