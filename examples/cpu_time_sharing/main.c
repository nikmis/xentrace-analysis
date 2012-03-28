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

	evh_register_handler(&reader, &switch_infprev_handler);

	reader_loop(&reader);

	return 0;
}
