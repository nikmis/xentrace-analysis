#include <stdio.h>
#include <stdlib.h>

#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Queue_state.h"
#include "Front_shared_ring_queue_unblocked.h"

extern QueueState *FrontSRQueue;
extern QueueState *FrontRQueue;
extern QueueState *BackRQueue;

unsigned lostRecCount;
LostRecTime lostRecTime;

int lost_records_init(EventHandler *handler)
{

	lostRecCount = 0;
	lostRecTime.totalLostRec = 0;
	lostRecTime.totalLostTime = 0;
	lostRecTime.cpu = 0;
	lostRecTime.lastNs = 0;
	lostRecTime.lastNs2 = 0;

	INIT_LIST_HEAD(&(lostRecTime.cpuList));

	return 0;
}

int lost_records_handler(EventHandler *handler, Event *event)
{
	/* Queue specific handling */
	queue_update_state(FrontSRQueue, LOST_REC, event); 
	queue_update_state(FrontRQueue, LOST_REC, event); 
	queue_update_state(BackRQueue, LOST_REC, event); 

	/* Lost records received. Clear all handler data */
	lostRecCount++;
	update_lrt_cpulist(event->cpu);

	if(event->lastNs)
	{
		LostRecTime *tmpLrt;
		list_head *head = &(lostRecTime.cpuList);

		list_for_each_entry(tmpLrt, head, cpuList)
		{
			if(tmpLrt->cpu == event->cpu)
			{
				tmpLrt->totalLostTime += (event->ns - event->lastNs);
				tmpLrt->totalLostRec += event->data[0];
				tmpLrt->lastNs = event->ns;
				tmpLrt->lastNs2 = event->lastNs;

				if((event->ns - event->lastNs) > LOST_REC_MAX_TIME)
				{
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
	printf("Lost records data\n");
	print_line();
	printf("\nTotal lost_record occurences = %u\n", lostRecCount);

	LostRecTime *tmpLrt;

	list_head *head = &(lostRecTime.cpuList);

	unsigned long long avgTime = get_avg_total_time();

	unsigned long long totalCPULostTime = 0;
	list_for_each_entry(tmpLrt, head, cpuList)
	{
		totalCPULostTime += tmpLrt->totalLostTime;
		printf("CPU %u : Total lost_records = %llu : Total lost time = %10.3f(ms)",
			tmpLrt->cpu, tmpLrt->totalLostRec, (float)tmpLrt->totalLostTime/MEGA);

		if(avgTime)
		{
			printf(" or %3.2f %%", (float)tmpLrt->totalLostTime/avgTime * 100);
		}
		printf("\n");
	}
			
	printf("Total lost time  = %10.3f (ms)\n\n", (float)totalCPULostTime/MEGA);
	print_line();

	free_lrt_cpulist();

	return 0;
}

unsigned long long get_ev_before_lost_records_ns(unsigned int cpu)
{
	LostRecTime *tmpLrt;

	list_head *head = &(lostRecTime.cpuList);

	if(!list_empty(head))
	{
		list_for_each_entry(tmpLrt, head, cpuList)
		{
			if(tmpLrt->cpu == cpu)
			{
				return tmpLrt->lastNs2;
			}
		}
	}

	return 0;
}

unsigned long long get_last_lost_records_ns(unsigned int cpu)
{
	LostRecTime *tmpLrt;

	list_head *head = &(lostRecTime.cpuList);

	if(!list_empty(head))
	{
		list_for_each_entry(tmpLrt, head, cpuList)
		{
			if(tmpLrt->cpu == cpu)
			{
				return tmpLrt->lastNs;
			}
		}
	}

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
	newLrt->lastNs = 0;

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
