#ifndef __HDLR_NUM_HYPERCALLS
#define __HDLR_NUM_HYPERCALLS

typedef struct NumTypes
{
	unsigned int 	type;
	unsigned int 	totalTypeCount;
	list_head 	typeList;
} NumTypes;

typedef struct NumHypercalls
{
	NumTypes numTypes;
	unsigned long long totalHypercallCount;
} NumHypercalls;

int num_hypercalls_init(EventHandler *handler);
int num_hypercalls_handler(EventHandler *handler, Event *event);
int num_hypercalls_finalize(EventHandler *handler);
void num_hypercalls_reset(void);

#endif
