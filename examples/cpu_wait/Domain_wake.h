#ifndef __HDLR_DOMAIN_WAKE
#define __HDLR_DOMAIN_WAKE

typedef struct DomainWakeData
{
	short wakeDomFlag;
	unsigned int wakeDomId;
	unsigned long long wakeTsc;
} DomainWakeData;

/* Event handler functions */
int domain_wake_init(EventHandler *handler);
int domain_wake_handler(EventHandler *handler, Event *event);
int domain_wake_finalize(EventHandler *handler);

/* Functions to provide global access to handler data */
unsigned int get_wake_dom_id(void);
unsigned long long get_wake_tsc(void);

void set_wake_dom_flag(void);
void reset_wake_dom_flag(void);
short get_wake_dom_flag(void);

#endif
