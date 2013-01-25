#include <stdio.h>
#include <stdlib.h>

#include "Wait_state.h"
#include "Event.h"
#include "Lost_records.h"

int wait_init_state(WaitData **wdat)
{
	(*wdat) = (WaitData *) malloc(sizeof(WaitData));

	if(*wdat == NULL)
	{
		fprintf(stderr, "%s:%d Failed to alloacte memory\n", __FILE__, __LINE__);
		return -1;
	}

	(*wdat)->state = INIT;
	(*wdat)->totalWaitTime = 0;

	return 0;
}

Time wait_update_state(WaitData *wdat, WMessage msg, Event *ev)
{
	Time time = 0;

	if(wdat == NULL)
	{
		fprintf(stderr, "%s:%d wdat ptr is NULL\n", __FILE__, __LINE__);
		return 0;
	}

	switch(wdat->state)
	{
		case INIT:
			time = wait_update_state_init(wdat, msg, ev);
			break;
		case SENT:
			time = wait_update_state_sent(wdat, msg, ev);
			break;
		case RECEIVED:
			time = wait_update_state_received(wdat, msg, ev);
			break;
		case UNKNOWN:
			time = wait_update_state_unknown(wdat, msg, ev);
			break;
	}

	return time;
}

Time wait_update_state_init(WaitData *wdat, WMessage msg, Event *ev)
{
	switch(msg)
	{
		case MSG_SENT: 
			wdat->state = SENT;
			break;
		case MSG_RECEIVED:
			wdat->state = RECEIVED;
			break;
		case MSG_UNKNOWN:
			wdat->state = UNKNOWN;
			break;
		default:
			return 0;
	}

	wdat->lastNs = ev->ns;
	return 0;

}

Time wait_update_state_sent(WaitData *wdat, WMessage msg, Event *ev)
{
	Time time = 0;

	switch(msg)
	{
		/* No need to update state */
		case MSG_SENT: 
			return 0;
		case MSG_RECEIVED:
			wdat->state = RECEIVED;

			time = ev->ns - wdat->lastNs;
			wdat->totalWaitTime += time;
			break;
		case MSG_UNKNOWN:
			/*Left blank for use if required */
		default:
			return 0;
	}

	wdat->lastNs = ev->ns;
	return time;
}

Time wait_update_state_received(WaitData *wdat, WMessage msg, Event *ev)
{
	Time time = 0;

	switch(msg)
	{
		case MSG_SENT:
			wdat->state = SENT;
			break;
		case MSG_RECEIVED:
		case MSG_UNKNOWN:
			/*Left blank for use if required */
		default:
			return 0;
	}

	wdat->lastNs = ev->ns;
	return time;
}

Time wait_update_state_unknown(WaitData *wdat, WMessage msg, Event *ev)
{
	Time time = 0;

	switch(msg)
	{
		case MSG_SENT:
		case MSG_RECEIVED:
		case MSG_UNKNOWN:
		default:
			return 0;
	}

	return time;
}

void wait_free_state(WaitData **wdat)
{
	free(*wdat);
	*wdat = NULL;
}

Time wait_total_time(WaitData *wdat)
{
	return wdat->totalWaitTime;
}
