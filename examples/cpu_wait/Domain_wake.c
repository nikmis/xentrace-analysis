#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Domain_wake.h"
#include "Switch_sched.h"

/* Use list.h instead of fixed len array Hash Table */
DomainWakeData domainWakeData[MAX_DOMS];

int domain_wake_init(EventHandler *handler)
{
	int i = 0;

	for(i = 0; i < MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(domainWakeData[i].vcpu_list));
	}

	return SUCCESS;
}

int domain_wake_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = event->data[0]; 
	unsigned int vcpu = event->data[1]; 

	if(!list_empty(&domainWakeData[domId].vcpu_list))
	{
		list_head *this;

		list_for_each(this, &domainWakeData[domId].vcpu_list)
		{
			DomainWakeData *dwd = list_entry(this, DomainWakeData, vcpu_list);

			/* Check if list object exists for requested vcpu */
			if(dwd->vcpu == vcpu)
			{
				dwd->wakeDomFlag = 1;
				dwd->wakeTsc = event->tsc;
				dwd->wakeNs  = event->ns;
				return SUCCESS;
			}	
		}
	}

	/* If here, it means no list object for this vcpu exists.
	 * else list is empty.
	 * 
	 * Either way create new object for vcpu and add it to the list.
	 */
	DomainWakeData *newDWD = (DomainWakeData *) malloc(sizeof(DomainWakeData));
	newDWD->vcpu = vcpu;
	newDWD->wakeDomFlag = 1;
	newDWD->wakeTsc = event->tsc;
	newDWD->wakeNs  = event->ns;

	list_add_tail(&(newDWD->vcpu_list), &(domainWakeData[domId].vcpu_list));

	return SUCCESS;
}

void domain_wake_reset(void)
{
	int i;

	for(i = 0; i < MAX_DOMS; i++)
	{
		DomainWakeData *dwd;
		list_head *head = &(domainWakeData[i].vcpu_list);

		if(!list_empty(head))
		{
			list_for_each_entry_reverse(dwd, head, vcpu_list)
			{
				free(dwd);
			}

			INIT_LIST_HEAD(head);
		}
	}
	
/*	Why doesn't this thing work ? It's not different from the
 *	above code except for memset. 
 *	Above code works without any issue (so far ?), but try and debug 
 *	when you have time.
 *
	int i, j = 0;

	for(i = 0; i < MAX_DOMS; i++)
	{
		list_head *head = &(domainWakeData[i].vcpu_list);
		DomainWakeData *dwd;

		if(!list_empty(head))
		{
			list_for_each_entry_reverse(dwd, head, vcpu_list)
			{
				printf("%d %p\n", j, &dwd->vcpu_list);
				memset(dwd, 0, sizeof(DomainWakeData));
				j++;
			}

			INIT_LIST_HEAD(head);
		}
	}
*/	
}

void reset_wake_dom_flag(unsigned int domId, unsigned int vcpu)
{
	DomainWakeData *dwd;
	list_head *head = &(domainWakeData[domId].vcpu_list);

	list_for_each_entry(dwd, head, vcpu_list)
	{
		if(dwd->vcpu == vcpu)
		{
			dwd->wakeDomFlag = 0;
			dwd->wakeTsc = 0;
			dwd->wakeNs = 0;
			return;
		}
	}
}

short get_wake_dom_flag(unsigned int domId, unsigned int vcpu)
{
	DomainWakeData *dwd;
	list_head *head = &(domainWakeData[domId].vcpu_list);

	list_for_each_entry(dwd, head, vcpu_list)
	{
		if(dwd->vcpu == vcpu)
		{
			return dwd->wakeDomFlag;
		}
	}

	return SUCCESS;
}

int domain_wake_finalize(EventHandler *handler)
{
	int i;

	for(i = 0; i < MAX_DOMS; i++)
	{
		DomainWakeData *dwd;
		list_head *head = &(domainWakeData[i].vcpu_list);

		if(!list_empty(head))
		{
			list_for_each_entry(dwd, head, vcpu_list)
			{
				free(dwd);
			}

			INIT_LIST_HEAD(head);
		}
	}
	return SUCCESS;
}

unsigned long long get_wake_tsc(unsigned int domId, unsigned int vcpu)
{
	DomainWakeData *dwd;
	list_head *head = &(domainWakeData[domId].vcpu_list);

	list_for_each_entry(dwd, head, vcpu_list)
	{
		if(dwd->vcpu == vcpu)
		{
			return dwd->wakeTsc;
		}
	}

	return SUCCESS;
}

unsigned long long get_wake_ns(unsigned int domId, unsigned int vcpu)
{
	DomainWakeData *dwd;
	list_head *head = &(domainWakeData[domId].vcpu_list);

	list_for_each_entry(dwd, head, vcpu_list)
	{
		if(dwd->vcpu == vcpu)
		{
			return dwd->wakeNs;
		}
	}

	return SUCCESS;
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
