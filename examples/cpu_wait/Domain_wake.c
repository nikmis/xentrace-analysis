#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Domain_wake.h"
#include "Switch_sched.h"

DomainWakeData domainWakeData;

int domain_wake_init(EventHandler *handler)
{
	DomainWakeData *dat = (DomainWakeData *) handler->data;

	dat->wakeDomFlag = 0;
	dat->wakeDomId = 0x7fff; /* idle domain */
	dat->wakeTsc = 0;

	return 0;
}

int domain_wake_handler(EventHandler *handler, Event *event)
{
	DomainWakeData *dat = (DomainWakeData *) handler->data;

	dat->wakeDomId = event->data[0];
	dat->wakeTsc = event->tsc;

	if(get_active_dom_id() != get_wake_dom_id()) /* Need to wake domain */
	{
		set_wake_dom_flag();
	}
	else /* Domain is running. No wait time */
	{
		reset_wake_dom_flag();
	}

	return 0;
}

void set_wake_dom_flag(void)
{
	domainWakeData.wakeDomFlag = 1;
}

void reset_wake_dom_flag(void)
{
	domainWakeData.wakeDomFlag = 0;
}

short get_wake_dom_flag(void)
{
	return domainWakeData.wakeDomFlag;
}

int domain_wake_finalize(EventHandler *handler)
{
	return 0;
}

unsigned int get_wake_dom_id(void)
{
	return domainWakeData.wakeDomId;
}

unsigned long long get_wake_tsc(void)
{
	return domainWakeData.wakeTsc;
}

struct EventHandler domainWakeHandler =
{
	.name = "domain_wake",
	.event_id =TRC_SCHED_WAKE,
	.init = domain_wake_init,
	.data = (void*)&domainWakeData,
	.process_event = domain_wake_handler,
	.finalize = domain_wake_finalize,
};

