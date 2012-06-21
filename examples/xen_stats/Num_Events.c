#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

NumEvs numEvents[MAX_DOMS];

int num_events_init(EventHandler *handler)
{
	return num_ev_init(numEvents);
}


int num_events_handler(EventHandler *handler, Event *event)
{
	return num_ev_handler(numEvents, event);
}

int num_events_finalize(EventHandler *handler)
{
	return num_ev_finalize(numEvents, "Event", "Port");
}

void num_events_reset(void)
{
	num_ev_reset(numEvents);
}

/* Extern Event handler struct */
struct EventHandler numEventsHandler = {
	.name = "event",
	.event_id = TRC_EVENT,
	.init = num_events_init,
	.data = (void *)numEvents,
	.process_event = num_events_handler,
	.finalize = num_events_finalize,
};
