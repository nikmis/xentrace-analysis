#include <stdio.h>
#include <stdlib.h>

#include "CpuList.h"
#include "Event.h"
#include "Macros.h"
#include "list.h"

unsigned long long avgTotalTime = 0;

void init_cpulist(CpuList *cpus)
{
	INIT_LIST_HEAD(&(cpus->cpuList));
	cpus->totalTime = 0;
}

void update_cpulist(CpuList *cpus, unsigned int cpu)
{
	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		if(tmpCpuList->cpu == cpu)
			return;
	}

	/* cpu not found. update list */
	CpuList *newCpuList = (CpuList *) malloc(sizeof(CpuList));

	init_cpulist(newCpuList);
	newCpuList->cpu = cpu;
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
		unsigned long long tmpTime = get_last_ns(tmpCpuList->cpu) - get_first_ns(tmpCpuList->cpu);

		tmpCpuList->totalTime = tmpTime;
		totalTime += tmpTime;
	}

	avgTotalTime = totalTime/cpuCount;

	return totalTime;
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
				return (get_last_ns(tmpCpuList->cpu) - get_first_ns(tmpCpuList->cpu));
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
