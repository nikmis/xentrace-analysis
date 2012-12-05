#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Wait_state.h"
#include "Back_response_queue_interrupt_send.h"
#include "Back_response_queue_interrupt_receive.h"
#include "CpuList.h"

FILE *brspqwaitFP;
WaitData *BackRspQWait;

int back_response_interrupt_receive_init(EventHandler *handler)
{
	if((brspqwaitFP = fopen("brspqwait.dat", "w+")) == NULL)
	{
		fprintf(stderr, "error opening histogram file\n");
	}

	if(BackRspQWait == NULL)
	{
		wait_init_state(&BackRspQWait);
	}

	return 0;
}

int back_response_interrupt_receive_handler(EventHandler *handler, Event *event)
{
	unsigned long long wTime = wait_update_state(BackRspQWait, MSG_RECEIVED, event);

	if((wTime) && (brspqwaitFP) && (fwrite(&wTime, sizeof(unsigned long long), 1, brspqwaitFP) != 1))
	{
		if(!feof(brspqwaitFP))
		{
			fprintf(stderr, "error writing to histogram file\n");
		}
	}

	return 0;
}

int back_response_interrupt_receive_finalize(EventHandler *handler)
{

	printf(	"\nBack Response Queue Wait Time : %15.3f (ms), %5.2f %% \n\n",
			(float)wait_total_time(BackRspQWait)/MEGA,
			(float)wait_total_time(BackRspQWait)/get_max_total_time()*100);

	wait_free_state(&BackRspQWait);

	fclose(brspqwaitFP);
	return 0;
}

void back_response_interrupt_receive_reset(void)
{
	wait_free_state(&BackRspQWait);
}

struct EventHandler backResponseInterruptReceiveHandler = 
{
	.name = "back_response_interrupt_receive_handler",
	.event_id = TRC_BLKFRONT_INT,
	.init = back_response_interrupt_receive_init,
	.data = (void *)NULL,
	.process_event = back_response_interrupt_receive_handler,
	.finalize = back_response_interrupt_receive_finalize,
};
