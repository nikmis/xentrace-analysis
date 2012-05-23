#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Domain_wake.h"
#include "Switch_sched.h"

/* Use list.h instead of fixed len array */
DomainWakeData domainWakeData[MAX_DOMS][MAX_VCPUS];

int domain_wake_init(EventHandler *handler)
{
	return 0;
}

int domain_wake_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = event->data[0]; 
	unsigned int vcpu = event->data[1]; 
	
	domainWakeData[domId][vcpu].wakeDomFlag = 1;
	domainWakeData[domId][vcpu].wakeTsc = event->tsc;
	domainWakeData[domId][vcpu].wakeNs = event->ns;

	return 0;
}

void domain_wake_reset(void)
{
	memset(domainWakeData, 0, sizeof(DomainWakeData)*MAX_DOMS*MAX_VCPUS);
}

void reset_wake_dom_flag(unsigned int domId, unsigned int vcpu)
{
	domainWakeData[domId][vcpu].wakeDomFlag = 0;
	domainWakeData[domId][vcpu].wakeTsc = 0;
	domainWakeData[domId][vcpu].wakeNs = 0;
}

short get_wake_dom_flag(unsigned int domId, unsigned int vcpu)
{
	return domainWakeData[domId][vcpu].wakeDomFlag;
}

int domain_wake_finalize(EventHandler *handler)
{
	return 0;
}

unsigned long long get_wake_tsc(unsigned int domId, unsigned int vcpu)
{
	return domainWakeData[domId][vcpu].wakeTsc;
}

unsigned long long get_wake_ns(unsigned int domId, unsigned int vcpu)
{
	return domainWakeData[domId][vcpu].wakeNs;
}

struct EventHandler domainWakeHandler =
{
	.name = "domain_wake",
	.event_id =TRC_SCHED_WAKE,
	.init = domain_wake_init,
	.data = (void*)domainWakeData,
	.process_event = domain_wake_handler,
	.finalize = domain_wake_finalize,
};
