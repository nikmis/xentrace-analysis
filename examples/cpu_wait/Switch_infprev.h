#ifndef __HDLR_SWITCH_INFPREV
#define __HDLR_SWITCH_INFPREV

typedef struct SwitchPrevData
{
	unsigned int prevDomId;
	unsigned long long prevTsc;
} SwitchPrevData;

/* Event handler functions */
int switch_infprev_init(EventHandler *handler);
int switch_infprev_handler(EventHandler *handler, Event *event);
int switch_infprev_finalize(EventHandler *handler);

/* Functions to provide global access to handler data */
unsigned int get_prev_dom_id(void);
unsigned long long get_prev_tsc(void);

#endif
