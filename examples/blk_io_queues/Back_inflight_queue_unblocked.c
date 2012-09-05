#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Back_inflight_queue_unblocked.h"

int back_inflight_queue_unblocked_init(EventHandler *handler)
{
	return 0;
}

int back_inflight_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int back_inflight_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

struct EventHandler backInflightQueueUnblockedHandler = 
{
	.name = "back_inflight_queue_unblocked",
	.event_id = TRC_BLKBACK_INFLIGHT_QUEUE_UNBLOCKED,
	.init = back_inflight_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = back_inflight_queue_unblocked_handler,
	.finalize = back_inflight_queue_unblocked_finalize,
};
