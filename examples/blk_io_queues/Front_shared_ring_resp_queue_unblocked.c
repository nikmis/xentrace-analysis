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

FILE *fsrrqFP;
QueueState *FrontSRRespQueue;

int front_shared_ring_resp_queue_unblocked_init(EventHandler *handler)
{
	if((fsrrqFP = fopen("fsrrq.dat", "w+")) == NULL)
	{
		fprintf(stderr, "error opening histogram file\n");
	}

	if(FrontSRRespQueue == NULL)
	{
		queue_init_state(&FrontSRRespQueue);
	}

	return 0;
}

int front_shared_ring_resp_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long wTime = queue_update_state(FrontSRRespQueue, Q_UNBLOCKED, event);

	if((wTime) && (fsrrqFP) && (fwrite(&wTime, sizeof(unsigned long long), 1, fsrrqFP) != 1))
	{
		if(!feof(fsrrqFP))
		{
			fprintf(stderr, "error writing to histogram file\n");
		}
	}


	return 0;
}

int front_shared_ring_resp_queue_unblocked_finalize(EventHandler *handler)
{
	printf("Front Shared Ring Resp Queue Unblocked : %15.3f (ms), %5.2f %% ; Blocked : %15.3f (ms), %5.2f %%\n\n", 
			(float)queue_unblocked_time(FrontSRRespQueue)/MEGA,
			(float)queue_unblocked_time(FrontSRRespQueue)/get_max_total_time()*100,
			(float)queue_blocked_time(FrontSRRespQueue)/MEGA,
			(float)queue_blocked_time(FrontSRRespQueue)/get_max_total_time()*100);

	queue_free_state(&FrontSRRespQueue);	

	fclose(fsrrqFP);
	return 0;
}

void front_shared_ring_resp_queue_unblocked_reset(void)
{
	queue_free_state(&FrontSRRespQueue);
}

struct EventHandler frontSharedRingRespQueueUnblockedHandler = 
{
	.name = "front_shared_ring_resp_queue_unblocked",
	.event_id = TRC_BLKFRONT_SHARED_RING_RESP_QUEUE_UNBLOCKED,
	.init = front_shared_ring_resp_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = front_shared_ring_resp_queue_unblocked_handler,
	.finalize = front_shared_ring_resp_queue_unblocked_finalize,
};
