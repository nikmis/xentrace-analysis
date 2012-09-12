#ifndef __CPULIST
#define __CPULIST

#include "list.h"

typedef struct CpuList
{
	int cpu;
	list_head cpuList;
	unsigned long long totalTime;
} CpuList;


void init_cpulist(CpuList *cpus);
void update_cpulist(CpuList *cpus, unsigned int cpu);

unsigned long long get_total_time(CpuList *cpus);
unsigned long long get_avg_total_time(void);
unsigned long long get_total_time_cpu(CpuList *cpus, unsigned int cpu);

void free_cpulist(CpuList *cpus);

#endif
