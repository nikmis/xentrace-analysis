#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Queue_state.h"
#include "Back_request_queue_unblocked.h"
#include "Back_request_queue_blocked.h"

QueueState *BackRQueue;

int back_request_queue_unblocked_init(EventHandler *handler)
{
	if(BackRQueue == NULL)
	{
		queue_init_state(&BackRQueue);
	}

	return 0;
}

int back_request_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	queue_update_state(BackRQueue, Q_UNBLOCKED, event);

	return 0;
}

int back_request_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

void back_request_queue_unblocked_reset(void)
{
	queue_free_state(&BackRQueue);
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
