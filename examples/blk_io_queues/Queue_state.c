#include <stdio.h>
#include <stdlib.h>

#include "Queue_state.h"
#include "Event.h"
#include "Lost_records.h"

int queue_init_state(QueueState **qst)
{
	(*qst) = (QueueState *) malloc(sizeof(QueueState));

	if(*qst == NULL)
	{
		fprintf(stderr, "%s:%d Failed to alloacte memory\n", __FILE__, __LINE__);
		return -1;
	}

	(*qst)->state = INIT;
	(*qst)->lastNs = 0;
	(*qst)->totalBlockedTime = (*qst)->totalUnblockedTime = (*qst)->totalTime = 0;

	return 0;
}

int queue_update_state(QueueState *qst, Message msg, Event *ev)
{
	if(qst == NULL)
	{
		fprintf(stderr, "%s:%d qst ptr is NULL\n", __FILE__, __LINE__);
		return -1;
	}

	switch(qst->state)
	{
		case INIT:
			queue_update_state_init(qst, msg, ev);
		case BLOCKED:
			queue_update_state_blocked(qst, msg, ev);
		case UNBLOCKED:
			queue_update_state_unblocked(qst, msg, ev);
		case UNKNOWN:
			queue_update_state_unknown(qst, msg, ev);
	}

	return 0;
}

void queue_update_state_init(QueueState *qst, Message msg, Event *ev)
{
	switch(msg)
	{
		case Q_BLOCKED: 
			qst->state = BLOCKED;
			break;
		case Q_UNBLOCKED:
			qst->state = UNBLOCKED;
			break;
		case LOST_REC:
			qst->state = UNKNOWN;
			break;
		default:
			return;
	}

	qst->lastNs = ev->ns;
}

void queue_update_state_blocked(QueueState *qst, Message msg, Event *ev)
{
	switch(msg)
	{
		/* No need to update state */
		case Q_BLOCKED: 
			return;
		case Q_UNBLOCKED:
			qst->state = UNBLOCKED;
			qst->totalBlockedTime += ev->ns - qst->lastNs;
			qst->totalTime += ev->ns - qst->lastNs;
			break;
		/* In case of lost_rec, insert a Q_UNBLOCKED before LOST_REC. 
		 * Then,
		 * 	ns = is the last seen msg before lost records.
		 * 	lastNs = last block msg.
		 * Make sure to pass the right argument in ns for lost rec
		 */
		case LOST_REC:
			qst->state = UNKNOWN;
			Time lastEvNs = get_ev_before_lost_records_ns(ev->cpu);
			qst->totalBlockedTime += lastEvNs - qst->lastNs;
			qst->totalTime += lastEvNs - qst->lastNs;
			break;
		default:
			return;
	}

	qst->lastNs = ev->ns;
}

void queue_update_state_unblocked(QueueState *qst, Message msg, Event *ev)
{
	switch(msg)
	{
		case Q_BLOCKED:
			qst->state = BLOCKED;
			qst->totalUnblockedTime = ev->ns - qst->lastNs;
			qst->totalTime += ev->ns - qst->lastNs;
			break;
		case Q_UNBLOCKED:
			return;
		/* In case of lost_rec, insert a Q_BLOCKED before LOST_REC. 
		 * Then,
		 * 	ns = is the last seen msg before lost records.
		 * 	lastNs = last block msg.
		 * Make sure to pass the right argument in ns for lost rec
		 */
		case LOST_REC:
			qst->state = UNKNOWN;
			Time lastEvNs = get_ev_before_lost_records_ns(ev->cpu);
			qst->totalBlockedTime += lastEvNs - qst->lastNs;
			qst->totalTime += lastEvNs - qst->lastNs;
			break;
		default:
			return;
	}

	qst->lastNs = ev->ns;
}

void queue_update_state_unknown(QueueState *qst, Message msg, Event *ev)
{
	switch(msg)
	{
		case Q_BLOCKED:
			qst->state = BLOCKED;
			qst->totalBlockedTime += ev->ns - qst->lastNs;
			qst->totalTime += ev->ns - qst->lastNs;
			break;
		case Q_UNBLOCKED:
			qst->state = UNBLOCKED;
			qst->totalUnblockedTime = ev->ns - qst->lastNs;
			qst->totalTime += ev->ns - qst->lastNs;
			break;
		case LOST_REC:
			qst->state = UNKNOWN;
			break;
		default:
			return;
	}

	qst->lastNs = ev->ns;
}

void queue_free_state(QueueState **qst)
{
	free(*qst);
	*qst = NULL;
}
