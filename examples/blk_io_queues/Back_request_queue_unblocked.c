#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Back_request_queue_unblocked.h"
#include "Back_request_queue_blocked.h"

unsigned long long lastBackRQUnblockNs;

int back_request_queue_unblocked_init(EventHandler *handler)
{
	lastBackRQUnblockNs = 0;
	return 0;
}

int back_request_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long lastBackRQBlockNs = get_last_back_rqblock_ns();
	unsigned long long lastLostRecordNs = get_last_lost_records_ns(event->cpu);

	/* 		SCENARIOS
	 * last_block	| last_block	| last_unblock
	 * 	.	|	.	|	.
	 * last_unblock	| lost_rec	| lost_rec
	 * 	.	|	.	|	.
	 * 	.	|	.	|	.
	 * lost_rec	| last_unblock	| last_block
	 * 	.	|	.	|	.
	 * unblock	| unblock	| unblock
	 */
	if( !((lastLostRecordNs > lastBackRQUnblockNs) && (lastLostRecordNs > lastBackRQBlockNs)) ) 
	{
		if(lastBackRQUnblockNs > lastBackRQBlockNs)
		{
			/* Successive Unblock events, w/o a block event.
			 * Ignore latest Unblock event.
			 */
			return 0;
		}
	}
	
	lastBackRQUnblockNs = event->ns;

	return 0;
}

int back_request_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

void back_request_queue_unblocked_reset(void)
{
	lastBackRQUnblockNs = 0;
}

unsigned long long get_last_back_rqunblock_ns()
{
	return lastBackRQUnblockNs;
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
