#include <stdio.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Exit_to_guest.h"
#include "Exit_to_xen.h"

static unsigned lostRecCount;

int lost_records_init(EventHandler *handler)
{
	lostRecCount = 0;
	return 0;
}

int lost_records_handler(EventHandler *handler, Event *event)
{
	lostRecCount++;
	/* Lost records received. Clear all handler data */
	if((event->ns - event->lastNs) > LOST_REC_MAX_TIME)
	{
		printf("Lost records at %llu. Time lost = %15.3f (ms)\n", event->ns, (float)(event->ns - event->lastNs)/MEGA);
		//exit_to_guest_reset();
		//exit_to_xen_reset();
	}
	return 0;
}

int lost_records_finalize(EventHandler *handler)
{
	printf("\nTotal of %u lost_record events encountered\n\n", lostRecCount);
	return 0;
}

struct EventHandler lostRecordsHandler = 
{
	.name = "lost_records",
	.event_id = TRC_LOST_RECORDS,
	.init = lost_records_init,
	.data = (void *)NULL,
	.process_event = lost_records_handler,
	.finalize = lost_records_finalize,
};
