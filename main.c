#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

#include "Macros.h"
#include "Event.h"
#include "EventHandler.h"

int trap_counter;

int trap_init(EventHandler *handler) 
{
   int *counter = (int*)handler->data;
   
   (*counter) = 0;

   return 0;
}

int trap_finalize(EventHandler *handler) 
{
   int *counter = (int*)handler->data;
   
   printf("Overall we've seen %d traps\n", *counter);
   return 0;
}

int trap_event(EventHandler *handler, Event *event) 
{
   int *counter = (int*)handler->data;

   (*counter)++;

   return 0;
}

static EventHandler trap_handler = {
    .name = "trap",
    .event_id = TRAP,
    .init = trap_init,
    .data = (void *)&trap_counter,
    .process_event = trap_event,
    .finalize = trap_finalize,
};

int main(int argc, char *argv[])
{
	Event event; 
	Reader reader;

	/* Init struct */
	clear_event(&event);

	reader_init(&reader, argv[1]);

	evh_register_handler(&reader, &trap_handler);

	reader_loop(&reader);

	return 0;
}

