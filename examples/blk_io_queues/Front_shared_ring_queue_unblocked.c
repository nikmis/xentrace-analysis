#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Queue_state.h"
#include "Front_shared_ring_queue_unblocked.h"
#include "Front_shared_ring_queue_blocked.h"

FILE *fsrqFP;
QueueState *FrontSRQueue;

int front_shared_ring_queue_unblocked_init(EventHandler *handler)
{
	if((fsrqFP = fopen("fsrq.dat", "w+")) == NULL)
	{
		fprintf(stderr, "error opening histogram file\n");
	}

	if(FrontSRQueue == NULL)
	{
		queue_init_state(&FrontSRQueue);
	}

	return 0;
}

int front_shared_ring_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long wTime = queue_update_state(FrontSRQueue, Q_UNBLOCKED, event);

	if((wTime) && (fsrqFP) && (fwrite(&wTime, sizeof(unsigned long long), 1, fsrqFP) != 1))
	{
		if(!feof(fsrqFP))
		{
			fprintf(stderr, "error writing to histogram file\n");
		}
	}


	return 0;
}

int front_shared_ring_queue_unblocked_finalize(EventHandler *handler)
{
	printf("Front Shared Ring Queue was blocked for %15.3f (ms)\n\n", 
			(float)queue_blocked_time(FrontSRQueue)/MEGA);

	queue_free_state(&FrontSRQueue);	

	fclose(fsrqFP);
	return 0;
}

void front_shared_ring_queue_unblocked_reset(void)
{
	queue_free_state(&FrontSRQueue);
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
