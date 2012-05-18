#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Switch_infnext.h"

SwitchNextData switchNextData;

int switch_infnext_init(EventHandler *handler)
{
	SwitchNextData *dat = (SwitchNextData *) handler->data;

	dat->nextDomId = 0x7fff; /* idle domain */
	dat->nextTsc = 0;
	dat->nextNs = 0;

	return 0;
}

int switch_infnext_handler(EventHandler *handler, Event *event)
{
	SwitchNextData *dat = (SwitchNextData *) handler->data;

	dat->nextDomId = event->data[0];
	dat->nextTsc = event->tsc;
	dat->nextNs = event->ns;

	return 0;
}

int switch_infnext_finalize(EventHandler *handler)
{
	return 0;
}

unsigned int get_next_dom_id(void)
{
	return switchNextData.nextDomId;
}

unsigned long long get_next_tsc(void)
{
	return switchNextData.nextTsc;
}

unsigned long long get_next_ns(void)
{
	return switchNextData.nextNs;
}

struct EventHandler switchInfnextHandler =
{
	.name = "switch_infnext",
	.event_id = TRC_SCHED_SWITCH_INFNEXT,
	.init = switch_infnext_init,
	.data = (void*)&switchNextData,
	.process_event = switch_infnext_handler,
	.finalize = switch_infnext_finalize,
};

