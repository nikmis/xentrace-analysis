#ifndef __PIPE_HEADER
#define __PIPE_HEADER

#include <stdio.h>
#include "uthash.h"
#include "Event.h"

#define SIZE 10

typedef enum 
{
	PIPE, 
	OR, 
	SPLIT, 
	JOIN
} StageType;

struct Stage;
struct DummyStageData;

typedef Event (*StageFunc)(struct Stage *s, Event ev, void *stageData);
typedef void (*FreeStageData)(struct Stage *s);

typedef struct Stage
{
	StageFunc f;
	StageType nextType;
	union
	{
		struct Stage* next;
		struct Stage* list_next[SIZE];
	};
	void *data;
	FreeStageData fsd;
	struct DummyStageData *dst;
	UT_hash_handle hh;
} Stage;

typedef struct DummyStageData
{
	struct JoinArgs
	{
		Stage *s;
		Event ev;
	} JoinArgs;

	struct JoinArgs args[SIZE];
	int joinEdgeCount;
	unsigned int joinEdgeFlags;
} DummyStageData;

void pipe(Stage *s1, Stage *s2);
void or(Stage *s1, Stage *s2);
void split(Stage *s1, Stage *s2);
void join(Stage *s1, Stage *s2);

Stage* create_stage(StageFunc f, FreeStageData fsd,void *data);
void free_stage(Stage *s);
Stage* create_dummy_stage(Stage *joinStage);

Event dummy_func(Stage *dummy, Event ev, void *data);
void free_dummy_func(Stage *dummy);
Event typical_join_func(Stage *s, Event ev, void *data);

void setbit(unsigned int *flag, int count);
void unsetbit(unsigned int *flag, int count);
void set_bit_count(unsigned int *flag, int count);
void init_event(Event *ev);

Event execute_pipe(Stage *s, Event ev, void *data);

#endif
