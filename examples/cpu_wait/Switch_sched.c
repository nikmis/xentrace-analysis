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

	sanity_check_domId();

	/* Flag set and active domain is also requested wake domain */
	if(get_wake_dom_flag() && (dat->schedActiveDomId == get_wake_dom_id())) 
	{
		dat->numDoms = add_dom_wait_time(dat);	
	}

	reset_wake_dom_flag();

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
			dat->d[i].domIdWaitTime += dat->schedTsc - get_wake_tsc();
			dat->totalWaitTime += dat->d[i].domIdWaitTime;
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
		dat->d[i].domIdWaitTime = dat->schedTsc - get_wake_tsc();
		dat->totalWaitTime += dat->d[i].domIdWaitTime;
		
		numDoms = i + 1;
	}


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
				(double)dat->d[i].domIdWaitTime/(1000000 * CPU_FREQ));
	}

	printf("Total CPU Wait time for all domains: %lf (ms)\n", (double)dat->totalWaitTime/(1000000 * CPU_FREQ));

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

void sanity_check_domId(void)
{
	if(get_old_dom_id() != get_prev_dom_id())
	{
		fprintf(stderr, "%s:%d: Prev domId mismatch at TSC %llu. infprev_domId = %u, switch_prev_domId = %u \n", 
				__FILE__, 
				__LINE__, 
				get_switch_tsc(), 
				get_prev_dom_id(),
				get_old_dom_id());
	}

	if(get_active_dom_id() != get_next_dom_id())
	{
		fprintf(stderr, "%s:%d: Next domId mismatch at TSC %llu. infnext_domId = %u, switch_active_domId = %u \n", 
				__FILE__, 
				__LINE__, 
				get_switch_tsc(), 
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

