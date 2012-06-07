#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Exit_to_xen.h"


int exit_to_xen_init(EventHandler *handler)
{

}

int exit_to_xen_handler(EventHandler *handler, Event *event)
{

}

int exit_to_xen_finalize(EventHandler *handler)
{

}

void exit_to_xen_reset(void)
{

}

struct EventHandler exitToXenHandler =
{
	.name = "exit_to_xen",
	.event_id = TRC_EXIT_TO_XEN,
	.init = exit_to_xen_init,
	.data = (void*)&,
	.process_event = exit_to_xen_handler,
	.finalize = exit_to_xen_reset,
};

