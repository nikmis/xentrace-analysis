#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Queue_state.h"
#include "Back_request_queue_blocked.h"
#include "Back_request_queue_unblocked.h"

extern QueueState *BackRQueue;

int back_request_queue_blocked_init(EventHandler *handler)
{
	if(BackRQueue == NULL)
	{
		queue_init_state(&BackRQueue);
	}

	return 0;
}

int back_request_queue_blocked_handler(EventHandler *handler, Event *event)
{
	queue_update_state(BackRQueue, Q_BLOCKED, event);

	return 0;
}

int back_request_queue_blocked_finalize(EventHandler *handler)
{
	printf("\nBack Request Queue is unblocked for %15.3f (ms) \n\n", 
			(float)queue_unblocked_time(BackRQueue)/MEGA);
	return 0;
}

void back_request_queue_blocked_reset(void)
{
	queue_free_state(&BackRQueue);
}

struct EventHandler backRequestQueueBlockedHandler = 
{
	.name = "back_request_queue_blocked",
	.event_id = TRC_BLKBACK_REQUEST_QUEUE_BLOCKED,
	.init = back_request_queue_blocked_init,
	.data = (void *)NULL,
	.process_event = back_request_queue_blocked_handler,
	.finalize = back_request_queue_blocked_finalize,
};
