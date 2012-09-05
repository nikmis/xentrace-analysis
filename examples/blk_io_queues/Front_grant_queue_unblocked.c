#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_grant_queue_unblocked.h"

int front_grant_queue_unblocked_init(EventHandler *handler)
{
	return 0;
}

int front_grant_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	return 0;
}

int front_grant_queue_unblocked_finalize(EventHandler *handler)
{
	return 0;
}

void front_grant_queue_unblocked_reset(void)
{

}

struct EventHandler frontGrantQueueUnblockedHandler = 
{
	.name = "front_grant_queue_unblocked",
	.event_id = TRC_BLKFRONT_GRANT_QUEUE_UNBLOCKED,
	.init = front_grant_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = front_grant_queue_unblocked_handler,
	.finalize = front_grant_queue_unblocked_finalize,
};
