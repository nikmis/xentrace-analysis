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

	(*qst)->cpus = -1;
	(*qst)->state = INIT;
	(*qst)->lastNs = 0;
	(*qst)->totalBlockedTime = 0; 
	(*qst)->totalUnblockedTime = 0;
	(*qst)->totalTime = 0;

	return 0;
}

Time queue_update_state(QueueState *qst, Message msg, Event *ev)
{
	Time time = 0;

	if(qst == NULL)
	{
		fprintf(stderr, "%s:%d qst ptr is NULL\n", __FILE__, __LINE__);
		return 0;
	}

	switch(qst->state)
	{
		case INIT:
			time = queue_update_state_init(qst, msg, ev);
			break;
		case BLOCKED:
			time = queue_update_state_blocked(qst, msg, ev);
			break;
		case UNBLOCKED:
			time = queue_update_state_unblocked(qst, msg, ev);
			break;
		case UNKNOWN:
			time = queue_update_state_unknown(qst, msg, ev);
			break;
	}

	return time;
}

Time queue_update_state_init(QueueState *qst, Message msg, Event *ev)
{
	switch(msg)
	{
		case Q_BLOCKED: 
			qst->state = BLOCKED;
			qst->cpus = ev->cpu;
			break;
		case Q_UNBLOCKED:
			qst->state = UNBLOCKED;
			qst->cpus = ev->cpu;
			break;
		case LOST_REC:
			qst->state = UNKNOWN;
			break;
		default:
			return 0;
	}

	qst->lastNs = ev->ns;
	return 0;

}

Time queue_update_state_blocked(QueueState *qst, Message msg, Event *ev)
{
	Time time = 0;

	switch(msg)
	{
		/* No need to update state */
		case Q_BLOCKED: 
			return 0;
		case Q_UNBLOCKED:
			qst->state = UNBLOCKED;

			time = ev->ns - qst->lastNs;
			qst->totalBlockedTime += time;
			qst->totalTime += time;
			break;
		/* In case of lost_rec, insert a Q_UNBLOCKED before LOST_REC. 
		 * Then,
		 * 	ns = is the last seen msg before lost records.
		 * 	lastNs = last block msg.
		 * Make sure to pass the right argument in ns for lost rec
		 */
		case LOST_REC:
			if(ev->cpu == qst->cpus)
			{
				qst->state = UNKNOWN;
				Time lastEvNs = get_ev_before_lost_records_ns(ev->cpu);

				if(lastEvNs)
				{
					time = lastEvNs - qst->lastNs;
					qst->totalBlockedTime += time;
					qst->totalTime += time;
				}
				break;
			}
			else
				return 0;
		default:
			return 0;
	}

	qst->lastNs = ev->ns;
	return time;
}

Time queue_update_state_unblocked(QueueState *qst, Message msg, Event *ev)
{
	Time time = 0;

	switch(msg)
	{
		case Q_BLOCKED:
			qst->state = BLOCKED;

			time = ev->ns - qst->lastNs;
			qst->totalUnblockedTime += time;
			qst->totalTime += time;
			break;
		case Q_UNBLOCKED:
			return 0;
		/* In case of lost_rec, insert a Q_BLOCKED before LOST_REC. 
		 * Then,
		 * 	ns = is the last seen msg before lost records.
		 * 	lastNs = last block msg.
		 * Make sure to pass the right argument in ns for lost rec
		 */
		case LOST_REC:
			if(ev->cpu == qst->cpus)
			{
				qst->state = UNKNOWN;
				Time lastEvNs = get_ev_before_lost_records_ns(ev->cpu);

				if(lastEvNs)
				{
					time = lastEvNs - qst->lastNs;

					qst->totalUnblockedTime += time;
					qst->totalTime += time;
				}
				break;
			}
			else
				return 0;
		default:
			return 0;
	}

	qst->lastNs = ev->ns;
	return time;
}

Time queue_update_state_unknown(QueueState *qst, Message msg, Event *ev)
{
	Time time = 0;

	switch(msg)
	{
		case Q_BLOCKED:
			qst->state = BLOCKED;

			time = ev->ns - qst->lastNs;
			qst->totalUnblockedTime += time;
			qst->totalTime += time;
			break;
		case Q_UNBLOCKED:
			qst->state = UNBLOCKED;

			time = ev->ns - qst->lastNs;
			qst->totalBlockedTime += time;
			qst->totalTime += time;
			break;
		case LOST_REC:
			if(ev->cpu == qst->cpus)
			{
				qst->state = UNKNOWN;
				break;
			}
			else
				return 0;
		default:
			return 0;
	}

	qst->lastNs = ev->ns;
	return time;
}

void queue_free_state(QueueState **qst)
{
	free(*qst);
	*qst = NULL;
}

Time queue_unblocked_time(QueueState *qst)
{
	return qst->totalUnblockedTime;
}

Time queue_blocked_time(QueueState *qst)
{
	return qst->totalBlockedTime;
}

Time queue_total_time(QueueState *qst)
{
	return qst->totalTime;
}
