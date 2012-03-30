#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_infprev.h"

/* Extern Varaible to be used by Handler */
DomAllTimes dom_all_times;

int start_of_log_flag = 1;

unsigned long long start_of_log, end_of_log;


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
	double distance_in_tsc, distance_in_ns;

	if(start_of_log_flag) {
		start_of_log_flag = 0;
		start_of_log = event->tsc;
		end_of_log = event->tsc;
	};


	/* Compute the distance between two consecutive infprev events, 
	 * we need it to do a sanity check and detect anomalies */

	distance_in_tsc = ((double)event->tsc - (double)end_of_log);
	distance_in_ns = distance_in_tsc /((double)2.4);

	end_of_log = event->tsc;
	
	dat->num_of_doms = add_time_to_list(dat->dt, event);
	dat->total_time += event->data[1];

	/* Sanity check: TSC timestamps can't be two times further apart */
	if((double)event->data[1] < distance_in_ns * 0.5) {
		printf("TSC anomaly, curr TSC:%llu, distance from TSC:%ld, distance from event data:%lld\n",
		       event->tsc, distance_in_ns, event->data[1]);
	};
	
	/* Sanity check 2: TSC timestamps can't be more than 500ms apart */
	if((((double)event->data[1])/1000000 > 500) || 
	    (distance_in_ns / 1000000 > 500)) {
		printf("TSC anomaly, event's can't be > 500ms apart curr TSC:%llu, distance from TSC:%ld, distance from event data:%lld\n",
		       event->tsc, distance_in_ns, event->data[1]);
	};


	//total_tsc_time = ((double)end_of_log - (double)start_of_log) * 0.41;

        //if(total_tsc_time > 1000000000) {
	//	switch_infprev_finalize(handler);
	//	start_of_log_flag = 1;
	//};

	return 0;
}

/* Call finalizers to display processed event data */
int switch_infprev_finalize(EventHandler *handler)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;
	unsigned short i = 0;
	double total_tsc_time_ns, total_tsc_time_cycles;
        
	total_tsc_time_cycles = ((double)end_of_log - (double)start_of_log);

	total_tsc_time_ns = total_tsc_time_cycles * ((double)1/2.4);

	printf("TSC: session start:%llu(cycles), end:%llu(cycles), length:%llu(cycles), %lf (ns)\n", 
			start_of_log, end_of_log, total_tsc_time_cycles, total_tsc_time_ns);


	while((i < dat->num_of_doms) && (i < MAX_DOMS))
	{
		printf("Dom ID = %5d : Cpu_Time = %15.4f(s) : Total_Time = %15.4f(s) : Cpu_Time_Share = %5.2f %%\n", 
				dat->dt[i].dom_id, 
				(float)dat->dt[i].runtime/1000000000, 
				(float)dat->total_time/1000000000, 
				((float)dat->dt[i].runtime/dat->total_time)*100);
                
		
	       		
		printf("TSC: Dom ID = %5d : Cpu_Time = %20lld(ns) : Total_Time = %lf(ns) : Cpu_Time_Share = %3.2f %%\n", 
				dat->dt[i].dom_id, 
				dat->dt[i].runtime, 
			        total_tsc_time_ns,
				((double)dat->dt[i].runtime/total_tsc_time_ns)*100);

                //if(start_of_log_flag) {
		//      dat->dt[i].runtime = 0;
	        //}


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
	.event_id = TRC_SCHED_SWITCH_INFPREV,
	.init = switch_infprev_init,
	.data = (void *)&dom_all_times,
	.process_event = switch_infprev_event,
	.finalize = switch_infprev_finalize,
};
