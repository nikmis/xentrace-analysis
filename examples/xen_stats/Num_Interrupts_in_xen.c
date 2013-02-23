#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

NumEvs numInterruptsInXen;

int num_interrupts_in_xen_init(EventHandler *handler)
{
	return	num_ev_in_xen_init(&numInterruptsInXen);
}


int num_interrupts_in_xen_handler(EventHandler *handler, Event *event)
{
	return num_ev_in_xen_handler(&numInterruptsInXen, event);
}

int num_interrupts_in_xen_finalize(EventHandler *handler)
{
	return num_ev_in_xen_finalize(&numInterruptsInXen, "Interrupt", "Vector");
}

void num_interrupts_in_xen_reset(void)
{
	num_ev_in_xen_reset(&numInterruptsInXen);
}

/* Extern Event handler struct */
struct EventHandler numInterruptsInXenHandler = {
	.name = "interrupt_in_xen",
	.event_id = TRC_INTERRUPT_IN_XEN,
	.init = num_interrupts_in_xen_init,
	.data = (void *)&numInterruptsInXen,
	.process_event = num_interrupts_in_xen_handler,
	.finalize = num_interrupts_in_xen_finalize,
};
