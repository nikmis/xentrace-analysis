#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_request_queue_unblocked.h"
#include "Front_request_queue_blocked.h"

unsigned long long lastRQUnblockNs;

int front_request_queue_unblocked_init(EventHandler *handler)
{
	lastRQUnblockNs = 0;
	return 0;
}

int front_request_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long lastRQBlockNs = get_last_rqblock_ns();

	if(lastRQUnblockNs > lastRQBlockNs)
	{
		/* Successive Unblock msgs seen, w/o a block msg.
		 * Ignore latest unblock msg.
		 */
		return 0;
	}

	/* 1st RQ Unblock msg after a Block msg */
	lastRQUnblockNs = event->ns;

	return 0;
}

int front_request_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

void front_request_queue_unblocked_reset(void)
{
	lastRQUnblockNs = 0;
}

unsigned long long get_last_rqunblock_ns()
{
	return lastRQUnblockNs;
}

struct EventHandler frontRequestQueueUnblockedHandler = 
{
	.name = "front_request_queue_unblocked",
	.event_id = TRC_BLKFRONT_REQUEST_QUEUE_UNBLOCKED,
	.init = front_request_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = front_request_queue_unblocked_handler,
	.finalize = front_request_queue_unblocked_finalize,
};
