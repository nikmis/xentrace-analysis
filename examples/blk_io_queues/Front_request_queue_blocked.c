#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_request_queue_blocked.h"

int front_request_queue_blocked_init(EventHandler *handler)
{
	return 0;
}

int front_request_queue_blocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int front_request_queue_blocked_finalize(EventHandler *handler)
{
	return 0;
}

void front_request_queue_blocked_reset(void)
{

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
