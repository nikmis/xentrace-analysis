#ifndef __WAIT_STATE_H
#define __WAIT_STATE_H

#include "Event.h"

typedef unsigned long long Time;

typedef enum 
{
	INIT,
	SENT,
	RECEIVED,
	UNKNOWN
} WaitState;

typedef enum 
{
	MSG_SENT,
	MSG_RECEIVED,
	MSG_UNKNOWN
} WMessage;


typedef struct WaitData
{
	WaitState state;
	Time lastNs;
	Time totalWaitTime;
} WaitData;

/* Public functions */
int wait_init_state(WaitData **wdat);
Time wait_update_state(WaitData *wdat, WMessage msg, Event *ev);
void wait_free_state(WaitData **wdat);

Time wait_total_time(WaitData *wdat);

/* Internally used only*/
Time wait_update_state_init(WaitData *wdat, WMessage msg, Event *ev);
Time wait_update_state_sent(WaitData *wdat, WMessage msg, Event *ev);
Time wait_update_state_received(WaitData *wdat, WMessage msg, Event *ev);
Time wait_update_state_unknown(WaitData *wdat, WMessage msg, Event *ev);

#endif
