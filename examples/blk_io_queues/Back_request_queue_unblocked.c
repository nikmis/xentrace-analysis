#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Back_request_queue_unblocked.h"

int back_request_queue_unblocked_init(EventHandler *handler)
{
	return 0;
}

int back_request_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int back_request_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

struct EventHandler backRequestQueueUnblockedHandler = 
{
	.name = "back_request_queue_unblocked",
	.event_id = TRC_BLKBACK_REQUEST_QUEUE_UNBLOCKED,
	.init = back_request_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = back_request_queue_unblocked_handler,
	.finalize = back_request_queue_unblocked_finalize,
};
