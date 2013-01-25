#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

/* Common Headers */
#include "Macros.h"
#include "Trace.h"
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

	evh_register_handler(&reader, &switchInfprevHandler);
	evh_register_handler(&reader, &switchSchedHandler);
	evh_register_handler(&reader, &lostRecordsHandler);

	reader_loop(&reader);

	reader_exit(&reader);

	return 0;
}
