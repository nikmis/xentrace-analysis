#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Queue_state.h"
#include "Front_shared_ring_resp_queue_blocked.h"
#include "Front_shared_ring_resp_queue_unblocked.h"

extern QueueState *FrontSRRespQueue;

int front_shared_ring_resp_queue_blocked_init(EventHandler *handler)
{
	if(FrontSRRespQueue == NULL)
	{
		queue_init_state(&FrontSRRespQueue);
	}

	return 0;
}

int front_shared_ring_resp_queue_blocked_handler(EventHandler *handler, Event *event)
{
	queue_update_state(FrontSRRespQueue, Q_BLOCKED, event);

	return 0;
}

int front_shared_ring_resp_queue_blocked_finalize(EventHandler *handler)
{
	return 0;
}

void front_shared_ring_resp_queue_blocked_reset(void)
{
	queue_free_state(&FrontSRRespQueue);
}

struct EventHandler frontSharedRingRespQueueBlockedHandler = 
{
	.name = "front_shared_ring_resp_queue_blocked",
	.event_id = TRC_BLKFRONT_SHARED_RING_RESP_QUEUE_BLOCKED,
	.init = front_shared_ring_resp_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = front_shared_ring_resp_queue_blocked_handler,
	.finalize = front_shared_ring_resp_queue_blocked_finalize,
};
