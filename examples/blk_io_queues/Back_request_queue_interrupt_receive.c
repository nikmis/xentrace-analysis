#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Wait_state.h"
#include "Back_request_queue_interrupt_send.h"
#include "Back_request_queue_interrupt_receive.h"
#include "CpuList.h"

FILE *brqwaitFP;
WaitData *BackRQWait;

int back_request_interrupt_receive_init(EventHandler *handler)
{
	if((brqwaitFP = fopen("brqwait.dat", "w+")) == NULL)
	{
		fprintf(stderr, "error opening histogram file\n");
	}

	if(BackRQWait == NULL)
	{
		wait_init_state(&BackRQWait);
	}

	return 0;
}

int back_request_interrupt_receive_handler(EventHandler *handler, Event *event)
{
	unsigned long long wTime = wait_update_state(BackRQWait, MSG_RECEIVED, event);

	if((wTime) && (brqwaitFP) && (fwrite(&wTime, sizeof(unsigned long long), 1, brqwaitFP) != 1))
	{
		if(!feof(brqwaitFP))
		{
			fprintf(stderr, "error writing to histogram file\n");
		}
	}

	return 0;
}

int back_request_interrupt_receive_finalize(EventHandler *handler)
{

	printf(	"\nBack Request Queue Wait Time  : %15.3f (ms), %5.2f %% \n",
			(float)wait_total_time(BackRQWait)/MEGA,
			(float)wait_total_time(BackRQWait)/get_max_total_time()*100);

	wait_free_state(&BackRQWait);

	fclose(brqwaitFP);
	return 0;
}

void back_request_interrupt_receive_reset(void)
{
	wait_free_state(&BackRQWait);
}

struct EventHandler backRequestInterruptReceiveHandler = 
{
	.name = "back_request_interrupt_receive_handler",
	.event_id = TRC_BLKBACK_INT,
	.init = back_request_interrupt_receive_init,
	.data = (void *)NULL,
	.process_event = back_request_interrupt_receive_handler,
	.finalize = back_request_interrupt_receive_finalize,
};
