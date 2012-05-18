#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_sched.h"
#include "Switch_infprev.h"
#include "Switch_infnext.h"
#include "Domain_wake.h"

SwitchSchedData switchSchedData;

int switch_sched_init(EventHandler *handler)
{
	SwitchSchedData *dat = (SwitchSchedData *) handler->data;

	dat->schedPrevDomId = 0x7fff; /* idle domain */
	dat->schedActiveDomId = 0x7fff; /* idle domain */
	dat->schedTsc = 0;
	dat->schedNs = 0;
	dat->totalWaitTime = 0;

	memset(dat->d, 0, sizeof(struct DomIdWaitTime) * MAX_DOMS);

	return 0;
}

int switch_sched_handler(EventHandler *handler, Event *event)
{
	SwitchSchedData *dat = (SwitchSchedData *) handler->data;

	dat->schedPrevDomId = event->data[0];
	dat->schedActiveDomId = event->data[2];
	dat->schedTsc = event->tsc;
	dat->schedNs = event->ns;

	sanity_check_domId();

	/* Proceed only if domain wake flag is set and active domain is also the requested wake domain */
	if(get_wake_dom_flag() && (dat->schedActiveDomId == get_wake_dom_id())) 
	{
		dat->numDoms = add_dom_wait_time(dat);	
		reset_wake_dom_flag();
	}


	return 0;
}

unsigned short add_dom_wait_time(SwitchSchedData *dat)
{
	/* Retain value of num of Doms observed so far */
	static unsigned short numDoms = 0;
	short i = 0;
	
	for(i = 0; i < numDoms; i++)
	{
		/* Check if dom id already exists */
		if(dat->schedActiveDomId == dat->d[i].domId)
		{
			/* 
			dat->d[i].domIdWaitTime += dat->schedTsc - get_wake_tsc();
			dat->totalWaitTime += dat->schedTsc - get_wake_tsc();
			*/
			/* Use ns instead for computing wait times */

			dat->d[i].domIdWaitTime += dat->schedNs - get_wake_ns();
			dat->totalWaitTime += dat->schedNs - get_wake_ns();

			/* Debug Msg
			printf("dom_id = %5u | delta = %lld | wait_time = %llu | total_wait_time = %llu | sched_TSC = %llu | wake_TSC = %llu\n", dat->d[i].domId, dat->schedTsc - get_wake_tsc(), dat->d[i].domIdWaitTime, dat->totalWaitTime, dat->schedTsc, get_wake_tsc());
			*/
			return numDoms;
		}
	}

	if(numDoms == MAX_DOMS)
	{
		/* No new doms can be added */
		return numDoms;
	}

	/* Do NOT add wait times for idle domain */
	if(dat->schedActiveDomId != 0x7fff)
	{
		dat->d[i].domId = dat->schedActiveDomId;
		dat->d[i].domIdWaitTime = dat->schedNs - get_wake_ns();
		dat->totalWaitTime += dat->schedNs - get_wake_ns();
		
		numDoms = i + 1;
	}

	/*
	printf("Outside: dom_id = %5u | wait_time = %llu | total_wait_time = %llu\n", dat->d[i].domId, dat->d[i].domIdWaitTime, dat->totalWaitTime);
	*/
	return numDoms;
}

int switch_sched_finalize(EventHandler *handler)
{
	short i = 0;
	SwitchSchedData *dat = (SwitchSchedData *)handler->data;

	for(i = 0; i < dat->numDoms; i++)
	{
		printf("domId: %u : CPU Wait Time: %lf (ms)\n",
				dat->d[i].domId,
				/* tsc
				(double)dat->d[i].domIdWaitTime/(1000000 * CPU_FREQ));
				*/
				(double)dat->d[i].domIdWaitTime/1000000);
	}
				
	printf("Total CPU Wait time for all domains: %lf (ms)\n", 
			/* tsc
			(double)dat->totalWaitTime/(1000000 * CPU_FREQ));
			*/
			(double)dat->totalWaitTime/1000000);
	return 0;
}

unsigned int get_old_dom_id(void)
{
	return switchSchedData.schedPrevDomId;
}

unsigned int get_active_dom_id(void)
{
	return switchSchedData.schedActiveDomId;
}

unsigned long long get_switch_tsc(void)
{
	return switchSchedData.schedTsc;
}

unsigned long long get_switch_ns(void)
{
	return switchSchedData.schedNs;
}

void sanity_check_domId(void)
{
	if(get_old_dom_id() != get_prev_dom_id())
	{
		fprintf(stderr, "%s:%d: Prev domId mismatch at TSC %llu. infprev_domId = %u, switch_prev_domId = %u \n", 
				__FILE__, 
				__LINE__, 
				get_switch_ns(), 
				get_prev_dom_id(),
				get_old_dom_id());
	}

	if(get_active_dom_id() != get_next_dom_id())
	{
		fprintf(stderr, "%s:%d: Next domId mismatch at TSC %llu. infnext_domId = %u, switch_active_domId = %u \n", 
				__FILE__, 
				__LINE__, 
				get_switch_ns(), 
				get_next_dom_id(),
				get_active_dom_id());
	}

}

struct EventHandler switchSchedHandler =
{
	.name = "__enter_scheduler",
	.event_id = TRC_SCHED_SWITCH,
	.init = switch_sched_init,
	.data = (void*)&switchSchedData,
	.process_event = switch_sched_handler,
	.finalize = switch_sched_finalize,
};

