#ifndef __HDLR_SWITCH_SCHED
#define __HDLR_SWITCH_SCHED

typedef struct SwitchSchedData
{
	unsigned int schedPrevDomId;
	unsigned int schedActiveDomId;
	unsigned long long schedTsc;
} SwitchSchedData;

/* Event handler functions */
int switch_sched_init(EventHandler *handler);
int switch_sched_handler(EventHandler *handler, Event *event);
int switch_sched_finalize(EventHandler *handler);

/* Functions to provide global access to handler data */
unsigned int get_old_dom_id(void);
unsigned int get_active_dom_id(void);
unsigned long long get_switch_tsc(void);

/* Helper functions */
void sanity_check_domId(void);

#endif
