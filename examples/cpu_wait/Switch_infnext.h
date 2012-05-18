#ifndef __HDLR_SWITCH_INFNEXT
#define __HDLR_SWITCH_INFNEXT

typedef struct SwitchNextData
{
	unsigned int nextDomId;
	unsigned long long nextTsc;
	unsigned long long nextNs;
} SwitchNextData;

/* Event handler functions */
int switch_infnext_init(EventHandler *handler);
int switch_infnext_handler(EventHandler *handler, Event *event);
int switch_infnext_finalize(EventHandler *handler);

/* Functions to provide global access to handler data */
unsigned int get_next_dom_id(void);
unsigned long long get_next_tsc(void);
unsigned long long get_next_ns(void);

#endif
