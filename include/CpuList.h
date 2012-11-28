#ifndef __CPULIST
#define __CPULIST

#include "list.h"
#include "Event.h"

typedef struct CpuList
{
	int cpu;
	list_head cpuList;
	unsigned lostRecFlag;
	unsigned long long firstNs;
	unsigned long long lastNs;
	unsigned long long totalTime;
} CpuList;


void init_cpulist(CpuList *cpus);
void update_cpulist(CpuList *cpus, Event *event);

unsigned long long get_total_time(CpuList *cpus);
unsigned long long get_avg_total_time(void);
unsigned long long get_total_time_cpu(CpuList *cpus, unsigned int cpu);

unsigned long long get_first_ns(CpuList* tmpCpuList);
unsigned long long get_last_ns(CpuList* tmpCpuList);

void set_last_record_flag();
void set_last_record_flag_list(CpuList *cpus);

void clear_last_record_flag();
unsigned is_last_record_flag_set();

void free_cpulist(CpuList *cpus);

#endif
