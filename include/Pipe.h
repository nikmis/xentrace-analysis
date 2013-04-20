#include <stdio.h>
#include "uthash.h"
#include "Event.h"

#define SIZE 10

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
	DummyStageData *dst;
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

Stage* create_stage(StageFunc f);
Stage* create_dummy_stage(Stage *joinStage);

Event dummy_func(Stage *dummy, Event ev, void *data);
Event typical_join_func(Stage *s, Event ev, void *data);

void setbit(unsigned int *flag, int count);
void unsetbit(unsigned int *flag, int count);
void set_bit_count(unsigned int *flag, int count);
void init_event(Event *ev);

Event execute_pipe(Stage *s, Event ev, void *data);

