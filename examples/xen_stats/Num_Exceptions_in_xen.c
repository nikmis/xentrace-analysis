#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

NumEvs numExceptionsInXen;

int num_exceptions_in_xen_init(EventHandler *handler)
{
	return num_ev_init(&numExceptionsInXen);	
}


int num_exceptions_in_xen_handler(EventHandler *handler, Event *event)
{
	return num_ev_in_xen_handler(&numExceptionsInXen, event);	
}

int num_exceptions_in_xen_finalize(EventHandler *handler)
{
	return num_ev_in_xen_finalize(&numExceptionsInXen, "Exception", "Vector");
}

void num_exceptions_in_xen_reset(void)
{
	num_ev_in_xen_reset(&numExceptionsInXen);	
}

/* Extern Event handler struct */
struct EventHandler numExceptionsInXenHandler = {
	.name = "exception_in_xen",
	.event_id = TRC_EXCEPTION_IN_XEN,
	.init = num_exceptions_in_xen_init,
	.data = (void *)&numExceptionsInXen,
	.process_event = num_exceptions_in_xen_handler,
	.finalize = num_exceptions_in_xen_finalize,
};
