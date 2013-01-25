#ifndef __HDLR_SWITCH_SCHED
#define __HDLR_SWITCH_SCHED

typedef struct DomIdWaitTime
{
	unsigned int domId;
	unsigned long long domIdWaitTime;
} DomIdWaitTime;

typedef struct SwitchSchedData
{
	/* Event specific members */
	unsigned int schedPrevDomId;
	unsigned int schedPrevVcpu;
	unsigned int schedActiveDomId;
	unsigned int schedActiveVcpu;
	unsigned long long schedTsc;
	unsigned long long schedNs;
	unsigned int schedCpu;
	/* Dom Wait time members */
	DomIdWaitTime d[MAX_DOMS];
	unsigned long long totalWaitTime;
	unsigned short numDoms;
	/* Histogram data */
	unsigned int *buckets;
} SwitchSchedData;

/* Event handler functions */
int switch_sched_init(EventHandler *handler);
int switch_sched_handler(EventHandler *handler, Event *event);
int switch_sched_finalize(EventHandler *handler);
void switch_sched_reset(void);

/* Functions to provide global access to handler data */
unsigned int get_old_dom_id(void);
unsigned int get_active_dom_id(void);
unsigned long long get_switch_tsc(void);
unsigned long long get_switch_ns(void);

/* Helper functions */
void sanity_check_domId(void);
unsigned short add_dom_wait_time(SwitchSchedData *d);

void add_to_histogram(long long diff, unsigned long long wakeNs, SwitchSchedData *dat);
void storeHistogram(SwitchSchedData *dat);
void drawConsoleHistogram(SwitchSchedData *dat);

#endif
