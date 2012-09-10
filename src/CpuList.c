#include <stdio.h>
#include <stdlib.h>

#include "CpuList.h"
#include "Event.h"
#include "Macros.h"
#include "list.h"

void init_cpulist(CpuList *cpus)
{
	INIT_LIST_HEAD(&(cpus->cpuList));
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

	list_add_tail(&(newCpuList->cpuList), head);
}

unsigned long long get_total_time(CpuList *cpus)
{
	unsigned long long totalTime = 0;

	CpuList *tmpCpuList;

	list_head *head = &(cpus->cpuList);

	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		unsigned long long tmpTime = get_last_ns(tmpCpuList->cpu) - get_first_ns(tmpCpuList->cpu);

		//printf("\nTotal Time on CPU %u is %15.3f (ms)\n", tmpCpuList->cpu, (float)tmpTime/MEGA);

		if(totalTime < tmpTime)
		{
			totalTime = tmpTime;
		}
	}

	return totalTime;
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

