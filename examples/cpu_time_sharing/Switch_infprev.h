#ifndef __HDLR_SWITCH_INFPREV
#define __HDLR_SWITCH_INFPREV

/* Struct for storing SWITCH_INFPREV related data */
typedef struct DomTime
{
	unsigned int 		dom_id;
	unsigned long long 	runtime;
} DomTime;

typedef struct DomAllTimes
{
	unsigned long long	totalTime;
	unsigned short		numOfDoms;
	DomTime			*dt;
} DomAllTimes;

/* Event Handler functions */
int switch_infprev_init(EventHandler *handler);
int switch_infprev_event(EventHandler *handler, Event *event);
int switch_infprev_finalize(EventHandler *handler);
void switch_infprev_reset(void);

/* Event Handler Helper functions */
unsigned short add_time_to_list(DomTime *, Event *);
void sanity_check(Event *ev);

#endif
