#include <stdio.h>
#include <stdlib.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
/* For reset methods */
#include "Back_inflight_queue_blocked.h"
#include "Back_inflight_queue_unblocked.h"
#include "Back_request_queue_blocked.h"
#include "Back_request_queue_unblocked.h"
#include "Front_grant_queue_blocked.h"
#include "Front_grant_queue_unblocked.h"
#include "Front_request_queue_blocked.h"
#include "Front_request_queue_unblocked.h"
#include "Front_shared_ring_queue_blocked.h"
#include "Front_shared_ring_queue_unblocked.h"

unsigned lostRecCount;
LostRecTime lostRecTime;

int lost_records_init(EventHandler *handler)
{

	lostRecCount = 0;
	lostRecTime.totalLostRec = 0;
	lostRecTime.totalLostTime = 0;
	lostRecTime.cpu = 0;

	INIT_LIST_HEAD(&(lostRecTime.cpuList));

	return 0;
}

int lost_records_handler(EventHandler *handler, Event *event)
{
	/* Lost records received. Clear all handler data */
	lostRecCount++;
	update_lrt_cpulist(event->cpu);

	if(event->lastNs)
	{
		if((event->ns - event->lastNs) > LOST_REC_MAX_TIME)
		{
			LostRecTime *tmpLrt;
			list_head *head = &(lostRecTime.cpuList);

			list_for_each_entry(tmpLrt, head, cpuList)
			{
				if(tmpLrt->cpu == event->cpu)
				{
					tmpLrt->totalLostTime += (event->ns - event->lastNs);
					tmpLrt->totalLostRec += event->data[0];
					printf("CPU: %u : Lost records at %llu : Time Lost = %15.3f (ms)\n", 
							event->cpu, event->ns, 
							(float)(event->ns - event->lastNs)/MEGA);

				}
				
			}
		}
	}
	return 0;
}

int lost_records_finalize(EventHandler *handler)
{
	printf("\nlost_record occurences = %u\n", lostRecCount);

	LostRecTime *tmpLrt;

	list_head *head = &(lostRecTime.cpuList);

	unsigned long long totalCPULostTime = 0;
	list_for_each_entry(tmpLrt, head, cpuList)
	{
		totalCPULostTime += tmpLrt->totalLostTime;
		 printf("CPU %u : Total lost_records = %llu : Total lost time = %10.3f(ms)\n",
			tmpLrt->cpu, tmpLrt->totalLostRec, (float)tmpLrt->totalLostTime/MEGA);
	}
			
	printf("Total lost time  = %10.3f (ms)\n\n", (float)totalCPULostTime/MEGA);

	free_lrt_cpulist();

	return 0;
}

void update_lrt_cpulist(unsigned int cpu)
{
	LostRecTime *tmpLrt;

	list_head *head = &(lostRecTime.cpuList);

	if(!list_empty(head))
	{
		list_for_each_entry(tmpLrt, head, cpuList)
		{
			if(tmpLrt->cpu == cpu)
			{
				return;
			}
		}
	}

	/* cpu not found. update list */
	LostRecTime *newLrt = (LostRecTime *) malloc(sizeof(LostRecTime));

	newLrt->cpu = cpu;
	
	newLrt->totalLostRec = 0;
	newLrt->totalLostTime = 0;

	list_add_tail(&(newLrt->cpuList), head);
}

void free_lrt_cpulist()
{
	LostRecTime *tmpLostRecTime;

	list_head *head = &(lostRecTime.cpuList);

	list_for_each_entry_reverse(tmpLostRecTime, head, cpuList)
	{
		free(tmpLostRecTime);
	}
}

struct EventHandler lostRecordsHandler = 
{
	.name = "lost_records",
	.event_id = TRC_LOST_RECORDS,
	.init = lost_records_init,
	.data = (void *)NULL,
	.process_event = lost_records_handler,
	.finalize = lost_records_finalize,
};
