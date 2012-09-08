#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_request_queue_blocked.h"
#include "Front_request_queue_unblocked.h"

RQBlockData rqblockData;

int front_request_queue_blocked_init(EventHandler *handler)
{
	memset(&rqblockData, 0, sizeof(RQBlockData));
	return 0;
}

int front_request_queue_blocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long lastRQUnblockNs = get_last_rqunblock_ns();

	rqblockData.requestQueueWaitTime += event->ns - lastRQUnblockNs;
	rqblockData.lastRQBlockNs = event->ns;

	return 0;
}

int front_request_queue_blocked_finalize(EventHandler *handler)
{
	printf("Front Request Queue was unblocked for %15.3f (ms)\n\n", (float)rqblockData.requestQueueWaitTime/MEGA);
	return 0;
}

void front_request_queue_blocked_reset(void)
{
	memset(&rqblockData, 0, sizeof(RQBlockData));
}

unsigned long long get_last_rqblock_ns()
{
	return rqblockData.lastRQBlockNs;
}

struct EventHandler frontRequestQueueBlockedHandler = 
{
	.name = "front_request_queue_blocked",
	.event_id = TRC_BLKFRONT_REQUEST_QUEUE_BLOCKED,
	.init = front_request_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = front_request_queue_blocked_handler,
	.finalize = front_request_queue_blocked_finalize,
};
