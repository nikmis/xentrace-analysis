#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Queue_state.h"
#include "Front_request_queue_blocked.h"
#include "Front_request_queue_unblocked.h"

QueueState *FrontRQueue;

int front_request_queue_blocked_init(EventHandler *handler)
{
	if(FrontRQueue == NULL)
	{
		queue_init_state(&FrontRQueue);
	}

	return 0;
}

int front_request_queue_blocked_handler(EventHandler *handler, Event *event)
{
	queue_update_state(FrontRQueue, Q_BLOCKED, event);

	return 0;
}

int front_request_queue_blocked_finalize(EventHandler *handler)
{
	printf("Front Request Queue was unblocked for %15.3f (ms)\n\n", 
			(float)queue_unblocked_time(FrontRQueue)/MEGA);
	return 0;
}

void front_request_queue_blocked_reset(void)
{
	queue_free_state(&FrontRQueue);
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
