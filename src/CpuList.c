#include <stdio.h>
#include <stdlib.h>

#include "Event.h"
#include "Macros.h"
#include "Event.h"
#include "list.h"
#include "CpuList.h"

unsigned lostRecFlagGlobal = 0;
unsigned long long avgTotalTime = 0;
unsigned long long maxCpuTotalTime = 0;

void init_cpulist(CpuList *cpus)
{
	INIT_LIST_HEAD(&(cpus->cpuList));
	cpus->firstNs = 0;
	cpus->lastNs = 0;
	cpus->totalTime = 0;
}

void update_cpulist(CpuList *cpus, Event *ev)
{
	int cpu = ev->cpu;

	
	if(is_last_record_flag_set())
	{
		set_last_record_flag_list(cpus);
		clear_last_record_flag();
	}

	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		if(tmpCpuList->cpu == cpu)
		{
			if(tmpCpuList->lostRecFlag)
			{
				tmpCpuList->firstNs = ev->ns;
				tmpCpuList->lostRecFlag = 0;
			}
			return;
		}
	}

	/* cpu not found. update list */
	CpuList *newCpuList = (CpuList *) malloc(sizeof(CpuList));

	init_cpulist(newCpuList);
	newCpuList->cpu = cpu;
	newCpuList->firstNs = ev->ns;
	newCpuList->lastNs = 0;
	newCpuList->lostRecFlag = 0;
	newCpuList->totalTime = 0;

	list_add_tail(&(newCpuList->cpuList), head);
}

unsigned long long get_total_time(CpuList *cpus)
{
	unsigned int cpuCount = 0;
	unsigned long long totalTime = 0;

	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	printf("\n");
	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		cpuCount++;
		/*
		printf("cpu: %d lastNs: %lld firstNs: %lld \n", 
				tmpCpuList->cpu, get_last_ns(tmpCpuList), get_first_ns(tmpCpuList)); 
		*/
		unsigned long long tmpTime = get_last_ns(tmpCpuList) - get_first_ns(tmpCpuList);

		tmpCpuList->totalTime = tmpTime;
		totalTime += tmpTime;

		if(maxCpuTotalTime < tmpTime)
			maxCpuTotalTime = tmpTime;
	}

	avgTotalTime = totalTime/cpuCount;

	return totalTime;
}

unsigned long long get_max_total_time()
{
	return maxCpuTotalTime;
}

unsigned long long get_avg_total_time(void)
{
	return avgTotalTime;
}

unsigned long long get_total_time_cpu(CpuList *cpus, unsigned int cpu)
{
	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		if(tmpCpuList->cpu == cpu)
		{
			if(tmpCpuList->totalTime)
				return tmpCpuList->totalTime;
			else
				return (get_last_ns(tmpCpuList) - get_first_ns(tmpCpuList));
		}
	}

	return 0;
}

void free_cpulist(CpuList *cpus)
{
	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	list_for_each_entry_reverse(tmpCpuList, head, cpuList)
	{
		free(tmpCpuList);
	}
}

void update_first_ns(CpuList* tmpCpuList, unsigned long long firstNs)
{
	tmpCpuList->firstNs = firstNs;
}

void update_last_ns(CpuList* tmpCpuList, unsigned long long lastNs)
{
	tmpCpuList->lastNs = lastNs;
}

unsigned long long get_first_ns(CpuList* tmpCpuList)
{
	if(tmpCpuList->firstNs)	return tmpCpuList->firstNs;

	return get_first_ns_ev_list(tmpCpuList->cpu);
}

unsigned long long get_last_ns(CpuList* tmpCpuList)
{
	if(tmpCpuList->lastNs)	return tmpCpuList->lastNs;

	return get_last_ns_ev_list(tmpCpuList->cpu);
}

void set_last_record_flag()
{
	lostRecFlagGlobal = 1;
}

void set_last_record_flag_list(CpuList *cpus)
{
	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		tmpCpuList->lostRecFlag = 1;	
	}
}

void clear_last_record_flag()
{
	lostRecFlagGlobal = 0;
}

unsigned is_last_record_flag_set()
{
	return lostRecFlagGlobal;
}
