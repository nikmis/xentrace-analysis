#ifndef __HDLR_DOMAIN_WAKE
#define __HDLR_DOMAIN_WAKE

typedef struct DomainWakeData
{
	short wakeDomFlag;
	unsigned long long wakeTsc;
	unsigned long long wakeNs;
	/* To be used later 
	list_head vcpu_list; 
	*/
} DomainWakeData;

/* Event handler functions */
int domain_wake_init(EventHandler *handler);
int domain_wake_handler(EventHandler *handler, Event *event);
int domain_wake_finalize(EventHandler *handler);
void domain_wake_reset(void);

/* Functions to provide global access to handler data */
unsigned long long get_wake_tsc(unsigned int domId, unsigned int vcpu);
unsigned long long get_wake_ns(unsigned int domId, unsigned int vcpu);

void reset_wake_dom_flag(unsigned int domId, unsigned int vcpu);
short get_wake_dom_flag(unsigned int domId, unsigned int vcpu);

#endif
