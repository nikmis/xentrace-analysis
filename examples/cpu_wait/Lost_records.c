#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"

int lost_records_init(EventHandler *handler)
{

}

int lost_records_handler(EventHandler *handler, Event *event)
{

}

int lost_records_finalize(EventHandler *handler)
{

}

struct EventHandler lostRecordsHandler = 
{
	.name = "lost_records",
	.event_id = TRC_LOST_RECORDS,
	.init = lost_records_init,
	.data = (void *),
	.process_event = lost_records_handler,
	.finalize = lost_records_finalize,
};
