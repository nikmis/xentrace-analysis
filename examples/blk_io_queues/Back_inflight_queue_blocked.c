#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Back_inflight_queue_blocked.h"

int back_inflight_queue_blocked_init(EventHandler *handler)
{
	return 0;
}

int back_inflight_queue_blocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int back_inflight_queue_blocked_finalize(EventHandler *handler)
{
	return 0;
}

struct EventHandler backInflightQueueBlockedHandler = 
{
	.name = "back_inflight_queue_blocked",
	.event_id = TRC_BLKBACK_INFLIGHT_QUEUE_BLOCKED,
	.init = back_inflight_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = back_inflight_queue_blocked_handler,
	.finalize = back_inflight_queue_blocked_finalize,
};
