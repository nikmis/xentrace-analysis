#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

NumEvs numHypercalls[MAX_DOMS];

int num_hypercalls_init(EventHandler *handler)
{
	return num_ev_init(numHypercalls);
}

int num_hypercalls_handler(EventHandler *handler, Event *event)
{
	return num_ev_handler(numHypercalls, event);
}

int num_hypercalls_finalize(EventHandler *handler)
{
	return num_ev_finalize(numHypercalls, "Hypercall", "Number");
}

void num_hypercalls_reset(void)
{
	num_ev_reset(numHypercalls);
}

/* Extern Event handler struct */
struct EventHandler numHypercallsHandler = {
	.name = "hypercall",
	.event_id = TRC_HYPERCALL,
	.init = num_hypercalls_init,
	.data = (void *)numHypercalls,
	.process_event = num_hypercalls_handler,
	.finalize = num_hypercalls_finalize,
};
