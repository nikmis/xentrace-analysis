#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_shared_ring_queue_unblocked.h"
#include "Front_shared_ring_queue_blocked.h"

SRUnblockData srunblockData;

int front_shared_ring_queue_unblocked_init(EventHandler *handler)
{
	memset(&srunblockData, 0, sizeof(SRUnblockData));
	return 0;
}

int front_shared_ring_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long lastSRBlockNs = get_last_srblock_ns();
	unsigned long long lastLostRecordNs = get_last_lost_records_ns(event->cpu);

	/* Ignore successive unblock msgs.
	 * Can also be seen during lost records.
	 */
	if(	(lastLostRecordNs > lastSRBlockNs) ||
		(srunblockData.lastSRUnblockNs > lastSRBlockNs))
	{
		fprintf(stderr, "ignoring unblock msg %llu\n", event->ns);
		return 0;
	}

	//fprintf(stderr, "wait time %15.5f ns %llu\n", (float)(event->ns - lastSRBlockNs)/MEGA, event->ns);

	if(lastSRBlockNs) /* Edge case: beginning of log, no block msg seen */
		srunblockData.sharedRingWaitTime += event->ns - lastSRBlockNs;

	srunblockData.lastSRUnblockNs = event->ns;

	return 0;
}

int front_shared_ring_queue_unblocked_finalize(EventHandler *handler)
{
	printf("Front Shared Ring Queue was blocked for %15.3f (ms)\n\n", (float)srunblockData.sharedRingWaitTime/MEGA);
	return 0;
}

void front_shared_ring_queue_unblocked_reset(void)
{
	memset(&srunblockData, 0, sizeof(SRUnblockData));
}

unsigned long long get_last_srunblock_ns()
{
	return srunblockData.lastSRUnblockNs;
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
