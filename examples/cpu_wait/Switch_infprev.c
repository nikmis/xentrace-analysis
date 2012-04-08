#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_infprev.h"

SwitchPrevData switchPrevData;

int switch_infprev_init(EventHandler *handler)
{
	SwitchPrevData *dat = (SwitchPrevData *) handler->data;

	dat->prevDomId = 0x7fff; /* idle domain */
	dat->prevTsc = 0;

	return 0;
}

int switch_infprev_handler(EventHandler *handler, Event *event)
{
	SwitchPrevData *dat = (SwitchPrevData *) handler->data;

	dat->prevDomId = event->data[0];
	dat->prevTsc = event->tsc;

	return 0;
}

int switch_infprev_finalize(EventHandler *handler)
{
	return 0;
}

unsigned int get_prev_dom_id(void)
{
	return switchPrevData.prevDomId;
}

unsigned long long get_prev_tsc(void)
{
	return switchPrevData.prevTsc;
}

struct EventHandler switchInfprevHandler =
{
	.name = "switch_infprev",
	.event_id = TRC_SCHED_SWITCH_INFPREV,
	.init = switch_infprev_init,
	.data = (void*)&switchPrevData,
	.process_event = switch_infprev_handler,
	.finalize = switch_infprev_finalize,
};

