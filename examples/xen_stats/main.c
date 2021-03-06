#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

/* Common Headers */
#include "Macros.h"
#include "Event.h"
#include "EventHandler.h"
#include "Reader.h"

/* Handler specific headers */
#include "Externals.h"

int main(int argc, char *argv[])
{
	Event event; 
	Reader reader;

	/* Init struct */
	clear_event(&event);

	reader_init(&reader, argv[1]);

	evh_register_handler(&reader, &numEventsHandler);
	evh_register_handler(&reader, &numExceptionsHandler);
	evh_register_handler(&reader, &numExceptionsInXenHandler);
	evh_register_handler(&reader, &numInterruptsHandler);
	evh_register_handler(&reader, &numInterruptsInXenHandler);
	evh_register_handler(&reader, &numHypercallsHandler);
	evh_register_handler(&reader, &lostRecordsHandler);

	reader_loop(&reader);

	reader_exit(&reader);

	return 0;
}
