#ifndef __HDLR_SWITCH_INFPREV
#define __HDLR_SWITCH_INFPREV

/* Struct for storing SWITCH_INFPREV related data */
typedef struct CpuLastRuntime
{
	unsigned long long 	lastRuntime[MAX_DOMS];
	unsigned int		cpuId;
	list_head		cpuList;
} CpuLastRuntime;

/* Event Handler functions */
int switch_infprev_init(EventHandler *handler);
int switch_infprev_handler(EventHandler *handler, Event *event);
int switch_infprev_finalize(EventHandler *handler);
void switch_infprev_reset(void);

/* Accessor functions */
unsigned long long switch_infprev_last_runtime(unsigned int cpuId, unsigned int domId);

#endif
