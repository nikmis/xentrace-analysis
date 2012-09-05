#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_grant_queue_blocked.h"

int front_grant_queue_blocked_init(EventHandler *handler)
{
	return 0;
}

int front_grant_queue_blocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int front_grant_queue_blocked_finalize(EventHandler *handler)
{
	return 0;
}

struct EventHandler frontGrantQueueBlockedHandler = 
{
	.name = "front_grant_queue_blocked",
	.event_id = TRC_BLKFRONT_GRANT_QUEUE_BLOCKED,
	.init = front_grant_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = front_grant_queue_blocked_handler,
	.finalize = front_grant_queue_blocked_finalize,
};
