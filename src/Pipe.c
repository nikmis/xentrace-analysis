#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include "Event.h"
#include "Trace.h"
#include "Pipe.h"

#define SIZE 10

Stage *dummyHash = NULL;

void pipe(Stage *s1, Stage *s2)
{
	s1->nextType = PIPE;

	s1->next = s2;
}

void or(Stage *s1, Stage *s2)
{
	s1->nextType = OR;
	
	int i = 0;
	while((i < SIZE) && (s1->list_next[i] != NULL))
			i++;
	if(i < SIZE)
	{
		s1->list_next[i] = s2;
	}
	else
	{
		fprintf(stderr, "Or: Cannot Pipe s2 to s1. Pipe full\n");
	}
}

void split(Stage *s1, Stage *s2)
{
	s1->nextType = SPLIT;

	int i = 0;
	while((i < SIZE) && (s1->list_next[i] != NULL))
		i++;
	if(i < SIZE)
	{
		s1->list_next[i] = s2;
	}
	else
	{
		fprintf(stderr, "Split: Cannot Pipe s2 to s1. Pipe full\n");
	}
}

void join(Stage *s1, Stage *s2, StageType type)
{
	s1->nextType = type;

	// Dummy stage to keep track of joins.
	//
	// Exclusive dummy stage for a join operation.
	// Here there is only one dummy stage for s2, 
	// no matter how many other stages join s2.
	Stage *dummy = create_dummy_stage(s2);
	
	s1->next = dummy;

	int i = 0;
	while((i < SIZE) && (dummy->dst->args[i].s != NULL))
		i++;

	if(dummy->dst->joinEdgeCount < SIZE)
	{
		dummy->dst->args[i].s = s1;
		dummy->dst->joinEdgeCount++;
		setbit(&dummy->dst->joinEdgeFlags, i);
	}
	else
	{
		fprintf(stderr, "Cant Join stages. Exceeded join limit\n");
		exit(0);
	}
}

Stage* create_stage(StageFunc f, FreeStageData fsd, void *data)
{
	Stage *st = (Stage *)malloc(sizeof(Stage));

	memset(st, 0, sizeof(Stage));

	st->f = f;
	st->fsd = fsd;
	st->data = data;

	return st;
}

void free_stage(Stage *s)
{
	s->fsd(s);
}

Stage* create_dummy_stage(Stage *joinStage)
{
	Stage *dummy = NULL;

	HASH_FIND_PTR(dummyHash, joinStage, dummy);

	if(!dummy) 
	{
		dummy = create_stage(dummy_func, free_dummy_func, NULL);
		dummy->nextType = PIPE;

		// Init list of Stage pointers that need to be joined.
		dummy->dst = (DummyStageData *) malloc(sizeof(DummyStageData) * SIZE);
		memset(dummy->dst, 0, sizeof(DummyStageData));

		dummy->next = joinStage;

		HASH_ADD_PTR(dummyHash, next, dummy);
	}

	return dummy;
}

Event dummy_func(Stage *dummy, Event ev, void *data)
{
	Event tmpev;
	init_event(&tmpev);

	if(ev.event_id == INVALID)
		return tmpev;

	Stage *prevStage = (Stage *)data;

	int i = 0;
	while(i < dummy->dst->joinEdgeCount) 
	{
		if(dummy->dst->args[i].s == prevStage)
		{
			dummy->dst->args[i].ev = ev;
			unsetbit(&dummy->dst->joinEdgeFlags, i);
			break;
		}
		i++;
	}

	return ev;

	// If dummy->dst->joinEdgeFlags == 0 then all events received.
}

void free_dummy_func(Stage *dummy)
{
	free(dummy->dst);
}

/*
Event typical_join_func(Stage *s, Event ev, void *data)
{
	// Wrapper function to parse dummy stage args
	// and call actual function with parsed args.
	int i = 0;
	Event args[5]; // Whatever the number of args join event takes.

	DummyStageData *dst = (DummyStageData *)data;
	while(dst->args[i].s != NULL)
	{
		args[i] = dst->args[i].ev;
	}

	return actual_join_func(args[0], args[1], args[2], args[3], args[4]);
}	

*/
void setbit(unsigned int *flag, int count)
{
	unsigned int mask = 1 << (count-1);
	*flag = *flag | mask;
}

void unsetbit(unsigned int *flag, int count)
{
	unsigned int mask = 1 << (count-1);
	mask = ~mask;
	*flag = *flag & mask;
}

void set_bit_count(unsigned int *flag, int count)
{
	unsigned int mask = ~((~0) << count);
	*flag = *flag & mask;
}

Event execute_pipe(Stage *s, Event ev, void *data)
{
	Event tmpev;
	init_event(&tmpev);

	if(ev.event_id != INVALID) 
	{
		if(s->f != NULL)
		{
			tmpev = s->f(s, ev, data);	
		}
		else
			tmpev = ev;

		if(tmpev.event_id == INVALID)
			return tmpev;

		switch(s->nextType)
		{
			case PIPE   : 	
				tmpev = execute_pipe(s->next, tmpev, data);
				break;

			case OR     : 
				{
					int i = 0;
					Event ev;
					init_event(&ev);
					do
					{
						// invalid ev.event_id means bool false.
						// exec next stage.
						ev = execute_pipe(s->list_next[i], tmpev, data);
						i++;

					} while((i < SIZE) && (s->list_next[i] != NULL) && (ev.event_id == INVALID));
				}
				break;
			
			case JOIN_OR :
				{
					Stage *dummy = s->next;	
					tmpev = dummy->f(dummy, tmpev, s);

					// (1 << 1) - 1 = 1
					// (1 << 2) - 1 = 11
					// (1 << 3) - 1 = 111 etc
					unsigned int exp = (1 << dummy->dst->joinEdgeCount);

					if(dummy->dst->joinEdgeFlags < (exp - 1))
					{
						tmpev = execute_pipe(dummy->next, tmpev, dummy->dst);
						set_bit_count(&dummy->dst->joinEdgeFlags, dummy->dst->joinEdgeCount);
					} 
				}
				break;

			case SPLIT  : 
				{
					int i = 0;
					// Doesnt account for logical OR and joins
					while((i < SIZE) && (s->list_next[i] != NULL))
					{
						tmpev = execute_pipe(s->list_next[i], tmpev, data);
						i++;
					}
				}
				break;

			case JOIN_SPLIT :  
				{
					Stage *dummy = s->next;	
					tmpev = dummy->f(dummy, tmpev, s);

					if(dummy->dst->joinEdgeFlags == 0)	
					{
						tmpev = execute_pipe(dummy->next, tmpev, dummy->dst);
						set_bit_count(&dummy->dst->joinEdgeFlags, dummy->dst->joinEdgeCount);
					} 
				}
				break;
		}
	}

	return tmpev;
}

void init_event(Event *ev)
{
	memset(ev, 0, sizeof(Event));
	ev->event_id = INVALID;
}
