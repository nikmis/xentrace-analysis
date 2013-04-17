#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"
#include "Event.h"

#define SIZE 10

Stage *joinHash = NULL;

typedef enum StageType = {PIPE, OR, SPLIT, JOIN};

typedef Event (*StageFunc)(struct Stage *s, Event ev);

typedef struct Stage
{
	StageFunc f;
	StageType nextType;
	union
	{
		struct Stage *next;
		struct Stage *list_next[SIZE];
	};
	void **data;
	UT_hash_handle hh;
} Stage;

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
	
	int i = 0;
	while((i < SIZE) && (dummy->data[i] != NULL))
		i++;
	s1->next = dummy;
	dummy->data[i] = s1;
}

Stage* create_stage(void)
{
	Stage *st = (Stage *)malloc(sizeof(Stage));

	memset(st, 0, sizeof(Stage));
	return st;
}

Stage* create_dummy_stage(Stage *s1, Stage *s2)
{
	Stage *dummy = NULL;

	HASH_FIND_PTR(joinHash, s2, dummy);

	if(!dummy) 
	{
		dummy = create_stage();
		dummy->nextType = PIPE;

		// Init list of Stage pointers that need to be joined.
		dummy->data = (Stage **) malloc(sizeof(Stage *) * SIZE);
		memset(dummy->data, 0, sizeof(Stage *) * SIZE);

		dummy->f = dummy_func;
		
		dummy->next = s2;

		HASH_ADD_PTR(joinHash, next, dummy);
	}

	return dummy;
}

Event create_dummy_event(Stage *s)
{
	
}

Event dummy_func(struct Stage *s, Event ev)
{
	
}

Event execute_pipe(Stage *s, Event ev)
{
	Event tmpev;
	event_init(&tmpev);

	if(ev.id != INVALID)
	{
		tmpev = s->f(s, ev);	

		switch(s->nextType)
		{
			case PIPE   : tmpev = execute_pipe(&s->next, tmpev);
				      break;
			case OR     : 
				    {
					    int i = 0;
					    Event ev = NULL;
					    do
					    {
						    // invalid ev.id means bool false.
						    // exec next stage.
						    ev = execute_pipe(s->next[i], tmpev);

					    } while((i < SIZE) && (s->next[i] != NULL) && (ev.id == INVALID));
			case SPLIT  : 
				    {
					    int i = 0;
					    // Doesnt account for logical OR and joins
					    while((i < SIZE) && (s->next[i] != NULL))
					    {
						    tmpev = execute_pipe(s->next[i], tmpev);
					    }
				    }
				    break;
			case JOIN   : // 
				    break;
		}
	}

	return tmpev;
}
