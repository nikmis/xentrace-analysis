#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Back_request_queue_blocked.h"
#include "Back_request_queue_unblocked.h"

BackRQBlockData backRQBlockData;

int back_request_queue_blocked_init(EventHandler *handler)
{
	memset(&backRQBlockData, 0, sizeof(BackRQBlockData));
	return 0;
}

int back_request_queue_blocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long lastRQUnblockNs = get_last_back_rqunblock_ns();

	backRQBlockData.backRQBlockWaitTime += event->ns - lastRQUnblockNs;
	backRQBlockData.lastRQBlockNs = event->ns;

	return 0;
}

int back_request_queue_blocked_finalize(EventHandler *handler)
{
	printf("\nBack Request Queue is unblocked for %15.3f (ms) \n\n", (float)backRQBlockData.backRQBlockWaitTime/MEGA);
	return 0;
}

void back_request_queue_blocked_reset(void)
{
	memset(&backRQBlockData, 0, sizeof(BackRQBlockData));
}

unsigned long long get_last_back_rqblock_ns()
{
	return backRQBlockData.lastRQBlockNs;
}

struct EventHandler backRequestQueueBlockedHandler = 
{
	.name = "back_request_queue_blocked",
	.event_id = TRC_BLKBACK_REQUEST_QUEUE_BLOCKED,
	.init = back_request_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = back_request_queue_blocked_handler,
	.finalize = back_request_queue_blocked_finalize,
};
