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

	evh_register_handler(&reader, &lostRecordsHandler);

	evh_register_handler(&reader, &backInflightQueueBlockedHandler);
	evh_register_handler(&reader, &backInflightQueueUnblockedHandler);

	evh_register_handler(&reader, &backRequestQueueBlockedHandler);
	evh_register_handler(&reader, &backRequestQueueUnblockedHandler);

	evh_register_handler(&reader, &frontGrantQueueBlockedHandler);
	evh_register_handler(&reader, &frontGrantQueueUnblockedHandler);

	evh_register_handler(&reader, &frontRequestQueueBlockedHandler);
	evh_register_handler(&reader, &frontRequestQueueUnblockedHandler);

	evh_register_handler(&reader, &frontSharedRingQueueBlockedHandler);
	evh_register_handler(&reader, &frontSharedRingQueueUnblockedHandler);

	evh_register_handler(&reader, &frontSharedRingRespQueueBlockedHandler);
	evh_register_handler(&reader, &frontSharedRingRespQueueUnblockedHandler);

	reader_loop(&reader);

	reader_exit(&reader);

	return 0;
}
