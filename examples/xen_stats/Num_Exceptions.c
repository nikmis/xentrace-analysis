#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

NumEvs numExceptions[MAX_DOMS];

int num_exceptions_init(EventHandler *handler)
{
	return num_ev_init(numExceptions);
}


int num_exceptions_handler(EventHandler *handler, Event *event)
{
	return num_ev_handler(numExceptions, event);
}

int num_exceptions_finalize(EventHandler *handler)
{
	return num_ev_finalize(numExceptions, "Exception", "Vector");
}

void num_exceptions_reset(void)
{
	num_ev_reset(numExceptions);
}

/* Extern Event handler struct */
struct EventHandler numExceptionsHandler = {
	.name = "exception",
	.event_id = TRC_EXCEPTION,
	.init = num_exceptions_init,
	.data = (void *)numExceptions,
	.process_event = num_exceptions_handler,
	.finalize = num_exceptions_finalize,
};
