#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include "Event.h"

#define SIZE 10

Stage *joinHash = NULL;

typedef enum StageType = {PIPE, OR, SPLIT, JOIN};

typedef Event (*StageFunc)(struct Stage *s, Event ev, void *stageData);

typedef struct Stage
{
	StageFunc f;
	StageType nextType;
	union
	{
		struct Stage *next;
		struct Stage *list_next[SIZE];
	};
	void *data;
	UT_hash_handle hh;
} Stage;

typedef struct DummyStageData
{
	struct JoinArgs
	{
		Stage *s;
		Event ev;
	} JoinArgs;

	JoinArgs args[SIZE];
	int joinEdgeCount;
	unsigned int joinEdgeFlags;
} DummyStageData;

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

void join(Stage *s1, Stage *s2)
{
	s1->nextType = JOIN;

	// Dummy stage to keep track of joins.
	//
	// Exclusive dummy stage for a join operation.
	// Here there is only one dummy stage for s2, 
	// no matter how many other stages join s2.
	Stage *dummy = create_dummy_stage(s2);
	
	s1->next = dummy;

	int i = 0;
	while((i < SIZE) && (dummy->data->args[i].s != NULL))
		i++;

	if(dummy->data->joinEdgeCount < SIZE)
	{
		dummy->data->args[i].s = s1;
		dummy->data->joinEdgeCount++;
		setbit(&dummy->data->joinEdgeFlags, i);
	}
	else
	{
		fprintf(stderr, "Cant Join stages. Exceeded join limit\n");
		exit(0);
	}
}

Stage* create_stage(StageFunc f)
{
	Stage *st = (Stage *)malloc(sizeof(Stage));

	memset(st, 0, sizeof(Stage));

	st->f = f;

	return st;
}

Stage* create_dummy_stage(Stage *joinStage)
{
	Stage *dummy = NULL;

	HASH_FIND_PTR(joinHash, joinStage, dummy);

	if(!dummy) 
	{
		dummy = create_stage(dummy_func);
		dummy->nextType = PIPE;

		// Init list of Stage pointers that need to be joined.
		//dummy->data = (DummyStageData *) malloc(sizeof(DummyStageData) * SIZE);
		memset(dummy->data, 0, sizeof(DummyStageData));

		dummy->next = joinStage;

		HASH_ADD_PTR(joinHash, next, dummy);
	}

	return dummy;
}

Event dummy_func(Stage *dummy, Event ev, void *data)
{
	Stage *prevStage = (Stage *)data;

	int i = 0, ev_count = 0;
	while(i < dummy->data->joinEdgeCount) 
	{
		if(dummy->data[i].s == prevStage)
		{
			dummy->data[i].ev = ev;
			unsetbit(&dummy->data->joinEdgeFlags, i);
			break;
		}
		i++;
	}
	// If dummy->data->joinEdgeFlags == 0 then all events received.
}

void setbit(unsigned int *flag, int count)
{
	unsigned int mask = 1 << (count-1);
	*flag = *flag | mask;
}

void unsetbit(unsigned int *flag, int count)
{
	unsigned int mask = 1 << (c-1);
	mask = ~mask;
	*flag = *flag & mask;
}

void set_bit_count(unsigned int *flag, int count)
{
	unsigned int mask = ~((~0) << count);
	*flag = *flag & mask;
}

Event execute_pipe(Stage *s, Event ev)
{
	Event tmpev;
	event_init(&tmpev);

	if(ev.id != INVALID)
	{

		tmpev = s->f(s, ev, NULL);	

		switch(s->nextType)
		{
			case PIPE   : 	
					tmpev = execute_pipe(s->next, tmpev);
					break;
			case OR     : 
					int i = 0;
					Event ev = NULL;
					do
					{
						// invalid ev.id means bool false.
						// exec next stage.
						ev = execute_pipe(s->next[i], tmpev);

					} while((i < SIZE) && (s->next[i] != NULL) && (ev.id == INVALID));
			case SPLIT  : 
					int i = 0;
					// Doesnt account for logical OR and joins
					while((i < SIZE) && (s->next[i] != NULL))
					{
						tmpev = execute_pipe(s->next[i], tmpev);
					}
					break;
			case JOIN   :  
					Stage *dummy = s->next;	

					tmpev = dummy->f(dummy, tmpev, s);

					if(dummy->data->joinEdgeFlags == 0)	
					{
						tmpev = execute_pipe(dummy->next,);
					}
					break;
		}
	}

	return tmpev;
}
