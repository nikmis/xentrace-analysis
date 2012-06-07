#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Exit_to_guest.h"


int exit_to_guest_init(EventHandler *handler)
{

}

int exit_to_guest_handler(EventHandler *handler, Event *event)
{

}

int exit_to_guest_finalize(EventHandler *handler)
{

}

void exit_to_guest_reset(void)
{

}

struct EventHandler exitToGuestHandler =
{
	.name = "exit_to_guest",
	.event_id = TRC_EXIT_TO_GUEST,
	.init = exit_to_guest_init,
	.data = (void*)&,
	.process_event = exit_to_guest_handler,
	.finalize = exit_to_guest_reset,
};

