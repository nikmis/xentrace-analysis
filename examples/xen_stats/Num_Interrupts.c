#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

NumEvs numInterrupts[MAX_DOMS];

int num_interrupts_init(EventHandler *handler)
{
	return num_ev_init(numInterrupts);
}


int num_interrupts_handler(EventHandler *handler, Event *event)
{
	return num_ev_handler(numInterrupts, event);
}

int num_interrupts_finalize(EventHandler *handler)
{
	return num_ev_finalize(numInterrupts, "Interrupt", "Vector");
}

void num_interrupts_reset(void)
{
	num_ev_reset(numInterrupts);
}

/* Extern Event handler struct */
struct EventHandler numInterruptsHandler = {
	.name = "interrupt",
	.event_id = TRC_INTERRUPT,
	.init = num_interrupts_init,
	.data = (void *)numInterrupts,
	.process_event = num_interrupts_handler,
	.finalize = num_interrupts_finalize,
};
