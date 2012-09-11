#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
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

static unsigned lostRecCount;
static unsigned long long totalLostRec;
static unsigned long long totalLostTime;

int lost_records_init(EventHandler *handler)
{
	lostRecCount = 0;
	totalLostRec = 0;
	totalLostTime = 0;
	return 0;
}

int lost_records_handler(EventHandler *handler, Event *event)
{
	/* Lost records received. Clear all handler data */
	lostRecCount++;

	if(event->lastNs)
	{
		totalLostTime += (event->ns - event->lastNs);
		totalLostRec += event->data[0];

		if((event->ns - event->lastNs) > LOST_REC_MAX_TIME)
		{
			printf("CPU: %u : Lost records at %llu : Time Lost = %15.3f (ms)\n", event->cpu, event->ns, (float)(event->ns - event->lastNs)/MEGA);
		}
	}
	return 0;
}

int lost_records_finalize(EventHandler *handler)
{
	printf("\nlost_record occurences = %u\n"
		 "Total lost_records     = %llu\n"
		 "Total lost time        = %.3f (ms)\n\n", 
			lostRecCount, totalLostRec, (float)totalLostTime/MEGA);
	return 0;
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
