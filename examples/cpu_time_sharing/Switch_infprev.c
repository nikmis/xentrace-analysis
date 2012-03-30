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

	if(start_of_log_flag) {
		start_of_log_flag = 0;
		start_of_log = event->tsc;
		end_of_log = event->tsc;
	};


	dat->num_of_doms = add_time_to_list(dat->dt, event);
	dat->total_time += event->data[1];

	sanity_check(event);

	end_of_log = event->tsc;

	return 0;
}

/* Sanity Check for TSC and runtime */
void sanity_check(Event *ev)
{
	double distance_in_tsc, distance_in_ns;


	/* Compute the distance between two consecutive infprev events, 
	 * we need it to do a sanity check and detect anomalies */

	distance_in_tsc = ((double)ev->tsc - (double)end_of_log);
	distance_in_ns = distance_in_tsc /((double)CPU_FREQ);


	/* Sanity check: TSC timestamps can't be two times further apart */
	if((double)ev->data[1] < distance_in_ns * 0.5) 
	{
		fprintf(stderr, "TSC anomaly: Curr TSC  = %20llu : Difference in TSC = %15.6lf (s) : Difference in runtime = %15.6f (s)\n",
		       ev->tsc, distance_in_ns/1000000000, (float)ev->data[1]/1000000000);
	};
	
	/* Sanity check 2: TSC timestamps can't be more than 500ms apart */
	if((((double)ev->data[1])/1000000 > 500) || 
	    (distance_in_ns / 1000000 > 500)) {
		fprintf(stderr, "TSC anomaly, event's can't be > 500ms apart : Curr TSC = %20llu : Difference in TSC = %15.6lf (s) : Difference in runtime = %15.6f (s)\n",
		       ev->tsc, distance_in_ns/1000000000, (float)ev->data[1]/1000000000);
	};

}
	
/* Call finalizers to display processed event data */
int switch_infprev_finalize(EventHandler *handler)
{
	DomAllTimes *dat = (DomAllTimes *)handler->data;
	unsigned short i = 0;
	double total_tsc_time_ns, total_tsc_time_cycles;
        
	while((i < dat->num_of_doms) && (i < MAX_DOMS))
	{
		printf("RUN: Dom ID = %5d : Cpu_Time = %15.4f(s) : Total_Time = %15.4f(s) : Cpu_Time_Share = %5.2f %%\n", 
				dat->dt[i].dom_id, 
				(float)dat->dt[i].runtime/1000000000, 
				(float)dat->total_time/1000000000, 
				((float)dat->dt[i].runtime/dat->total_time)*100);
		++i;
	}

	i = 0;
	total_tsc_time_cycles = ((double)end_of_log - (double)start_of_log);
	total_tsc_time_ns = total_tsc_time_cycles * ((double)1/2.4);

	printf("TSC: session start:%llu(cycles), end:%llu(cycles), length:%lf(cycles), %lf (ns)\n", 
			start_of_log, end_of_log, total_tsc_time_cycles, total_tsc_time_ns);
	while((i < dat->num_of_doms) && (i < MAX_DOMS))
	{
		printf("TSC: Dom ID = %5d : Cpu_Time = %15.4f(s) : Total_Time = %15.4lf(s) : Cpu_Time_Share = %3.2f %%\n", 
				dat->dt[i].dom_id, 
				(float)dat->dt[i].runtime/1000000000, 
			        total_tsc_time_ns/1000000000,
				((double)dat->dt[i].runtime/total_tsc_time_ns)*100);
	
		++i;
	}
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
