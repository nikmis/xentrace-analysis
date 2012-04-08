#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_sched.h"
#include "Switch_infprev.h"
#include "Switch_infnext.h"

SwitchSchedData switchSchedData;

int switch_sched_init(EventHandler *handler)
{
	SwitchSchedData *dat = (SwitchSchedData *) handler->data;

	dat->schedPrevDomId = 0x7fff; /* idle domain */
	dat->schedActiveDomId = 0x7fff; /* idle domain */
	dat->schedTsc = 0;

	return 0;
}

int switch_sched_handler(EventHandler *handler, Event *event)
{
	SwitchSchedData *dat = (SwitchSchedData *) handler->data;

	dat->schedPrevDomId = event->data[0];
	dat->schedActiveDomId = event->data[2];
	dat->schedTsc = event->tsc;

	sanity_check_domId();

	return 0;
}

int switch_sched_finalize(EventHandler *handler)
{
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

