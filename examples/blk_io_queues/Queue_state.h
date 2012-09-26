#ifndef __QUEUE_STATE_H
#define __QUEUE_STATE_H

#include "Event.h"

typedef unsigned long long Time;

typedef enum 
{
	INIT,
	BLOCKED,
	UNBLOCKED,
	UNKNOWN
} State;

typedef enum 
{
	Q_BLOCKED,
	Q_UNBLOCKED,
	LOST_REC	
} Message;


typedef struct QueueState
{
	State state;
	Time lastNs;
	Time totalBlockedTime;
	Time totalUnblockedTime;
	Time totalTime;
} QueueState;

/* Public functions */
int queue_init_state(QueueState **qst);
int queue_update_state(QueueState *qst, Message msg, Event *ev);
void queue_free_state(QueueState **qst);

/* Internally used only*/
void queue_update_state_init(QueueState *qst, Message msg, Event *ev);
void queue_update_state_blocked(QueueState *qst, Message msg, Event *ev);
void queue_update_state_unblocked(QueueState *qst, Message msg, Event *ev);
void queue_update_state_unknown(QueueState *qst, Message msg, Event *ev);
#endif
