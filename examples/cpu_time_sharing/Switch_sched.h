#ifndef __HDLR_SWITCH_SCHED
#define __HDLR_SWITCH_SCHED

typedef struct VcpuTimes
{
	unsigned int 		vcpuId;
	unsigned long long 	totalVcpuTime;
	list_head 		vcpuList;
} VcpuTimes;

typedef struct DomVcpuTimes
{
	unsigned int 		domId;
	unsigned long long 	totalDomTime;
	VcpuTimes 		vcpuTimes;
	unsigned int		numVcpus;
} DomVcpuTimes;

typedef struct CpuTimes
{
	DomVcpuTimes 		domVcpuTimes[MAX_DOMS];
	unsigned long long	totalCpuTime;
	unsigned long long 	prevNs;
	unsigned int 		prevDomId;
	unsigned int 		prevVcpuId;
	unsigned int 		cpuId;
	list_head 		cpuList;	
} CpuTimes;

/* Event Handler functions */
int switch_sched_init(EventHandler *handler);
int switch_sched_handler(EventHandler *handler, Event *event);
int switch_sched_finalize(EventHandler *handler);
void switch_sched_reset(void);

/* Helper functions */
unsigned int calc_max_num_vcpus(list_head *headCpuList);
void free_list_mallocs(list_head *headCpuList);

#endif
