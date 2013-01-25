#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Wait_state.h"
#include "Back_response_queue_interrupt_send.h"
#include "Back_response_queue_interrupt_receive.h"

extern WaitData *BackRspQWait;

int back_response_interrupt_send_init(EventHandler *handler)
{
	if(BackRspQWait == NULL)
	{
		wait_init_state(&BackRspQWait);
	}

	return 0;
}

int back_response_interrupt_send_handler(EventHandler *handler, Event *event)
{
	wait_update_state(BackRspQWait, MSG_SENT, event);

	return 0;
}

int back_response_interrupt_send_finalize(EventHandler *handler)
{
	return 0;
}

void back_response_interrupt_send_reset(void)
{
}

struct EventHandler backResponseInterruptSendHandler = 
{
	.name = "back_response_interrupt_send_handler",
	.event_id = TRC_BLKFRONT_INT_SEND,
	.init = back_response_interrupt_send_init,
	.data = (void *)NULL,
	.process_event = back_response_interrupt_send_handler,
	.finalize = back_response_interrupt_send_finalize,
};
