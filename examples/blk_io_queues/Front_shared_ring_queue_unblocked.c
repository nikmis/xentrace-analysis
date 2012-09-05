#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_shared_ring_queue_unblocked.h"

int front_shared_ring_queue_unblocked_init(EventHandler *handler)
{
	return 0;
}

int front_shared_ring_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int front_shared_ring_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

struct EventHandler frontSharedRingQueueUnblockedHandler = 
{
	.name = "front_shared_ring_queue_unblocked",
	.event_id = TRC_BLKFRONT_SHARED_RING_QUEUE_UNBLOCKED,
	.init = front_shared_ring_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = front_shared_ring_queue_unblocked_handler,
	.finalize = front_shared_ring_queue_unblocked_finalize,
};
