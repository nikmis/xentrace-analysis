#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_shared_ring_queue_blocked.h"

int front_shared_ring_queue_blocked_init(EventHandler *handler)
{
	return 0;
}

int front_shared_ring_queue_blocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int front_shared_ring_queue_blocked_finalize(EventHandler *handler)
{
	return 0;
}

void front_shared_ring_queue_blocked_reset(void)
{

}

struct EventHandler frontSharedRingQueueBlockedHandler = 
{
	.name = "front_shared_ring_queue_blocked",
	.event_id = TRC_BLKFRONT_SHARED_RING_QUEUE_BLOCKED,
	.init = front_shared_ring_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = front_shared_ring_queue_blocked_handler,
	.finalize = front_shared_ring_queue_blocked_finalize,
};
