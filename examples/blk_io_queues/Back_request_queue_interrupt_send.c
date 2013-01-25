#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Wait_state.h"
#include "Back_request_queue_interrupt_send.h"
#include "Back_request_queue_interrupt_receive.h"

extern WaitData *BackRQWait;

int back_request_interrupt_send_init(EventHandler *handler)
{
	if(BackRQWait == NULL)
	{
		wait_init_state(&BackRQWait);
	}

	return 0;
}

int back_request_interrupt_send_handler(EventHandler *handler, Event *event)
{
	wait_update_state(BackRQWait, MSG_SENT, event);

	return 0;
}

int back_request_interrupt_send_finalize(EventHandler *handler)
{
	printf("QUEUE WAIT TIMES\n");
	print_line();
	return 0;
}

void back_request_interrupt_send_reset(void)
{
}

struct EventHandler backRequestInterruptSendHandler = 
{
	.name = "back_request_interrupt_send_handler",
	.event_id = TRC_BLKBACK_INT_SEND,
	.init = back_request_interrupt_send_init,
	.data = (void *)NULL,
	.process_event = back_request_interrupt_send_handler,
	.finalize = back_request_interrupt_send_finalize,
};
