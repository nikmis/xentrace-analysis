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

FILE *frqFP;
QueueState *FrontRQueue;

int front_request_queue_blocked_init(EventHandler *handler)
{
	if((frqFP = fopen("frq.dat", "w+")) == NULL)
	{
		fprintf(stderr, "error opening histogram file\n");
	}

	if(FrontRQueue == NULL)
	{
		queue_init_state(&FrontRQueue);
	}

	return 0;
}

int front_request_queue_blocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long wTime = queue_update_state(FrontRQueue, Q_BLOCKED, event);

	if((wTime) && (frqFP) && (fwrite(&wTime, sizeof(unsigned long long), 1, frqFP) != 1))
	{
		if(!feof(frqFP))
		{
			fprintf(stderr, "error writing to histogram file\n");
		}
	}

	return 0;
}

int front_request_queue_blocked_finalize(EventHandler *handler)
{
	printf("Front Request Queue Unblocked     : %15.3f (ms), %5.2f %% ; Blocked : %15.3f (ms), %5.2f %%\n\n", 
			(float)queue_unblocked_time(FrontRQueue)/MEGA,
			(float)queue_unblocked_time(FrontRQueue)/get_max_total_time()*100,
			(float)queue_blocked_time(FrontRQueue)/MEGA,
			(float)queue_blocked_time(FrontRQueue)/get_max_total_time());

	queue_free_state(&FrontRQueue);	

	fclose(frqFP);
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
