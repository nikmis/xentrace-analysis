#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "list.h"
#include "Switch_infprev.h"
#include "Switch_sched.h"

static CpuTimes cpuTimes;

int switch_sched_init(EventHandler *handler)
{
	int i;

	INIT_LIST_HEAD(&(cpuTimes.cpuList));
	cpuTimes.prevNs = 0;
	cpuTimes.cpuId = 0;
	cpuTimes.prevDomId = 0;
	cpuTimes.prevVcpuId = 0;
	cpuTimes.totalCpuTime = 0;	
	memset(cpuTimes.domVcpuTimes, 0, sizeof(DomVcpuTimes)*MAX_DOMS);

	for(i = 0; i < MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(cpuTimes.domVcpuTimes[i].vcpuTimes.vcpuList));
	}

	return SUCCESS;
}

int switch_sched_handler(EventHandler *handler, Event *event)
{
	unsigned int domId = event->data[0]; 
	unsigned int domIdIndex = event->data[0]; 
	unsigned int vcpuIdIndex = event->data[1]; 
	
	unsigned long long lastRuntime = 0;

	if(domIdIndex == 0x7fff)	
		domIdIndex = MAX_DOMS - 1;

	list_head *headCpuList = &(cpuTimes.cpuList);

	if(!list_empty(headCpuList))
	{
		list_head *this;

		list_for_each(this, headCpuList)
		{
			CpuTimes *tmpCpuTimes = list_entry(this, CpuTimes, cpuList);

			/* Chk if physical cpu exists on list */
			if(tmpCpuTimes->cpuId == event->cpu)
			{
				/* cpu exists and matches 
				 * add runtime to the domain and its vcpu
				 */
				unsigned int flag = 0;
				unsigned long long runtime = event->ns - tmpCpuTimes->prevNs;

				/* Add runtime */
				if(	(tmpCpuTimes->prevDomId == event->data[0]) && 
					(tmpCpuTimes->prevVcpuId == event->data[1]))
				{
					
					list_head *headVcpuList = &(tmpCpuTimes->domVcpuTimes[domIdIndex].vcpuTimes.vcpuList);
					VcpuTimes *tmpVcpuTimes;

					if(!list_empty(headVcpuList))
					{
						list_for_each_entry(tmpVcpuTimes, headVcpuList, vcpuList)
						{
							if(tmpVcpuTimes->vcpuId == vcpuIdIndex)
							{
								tmpVcpuTimes->totalVcpuTime += runtime;
								flag = 1;
							}
						}
					}

					if(!flag)
					{
						/* Couldn't find vcpu, or vcpu doesn't exist.
						 * Malloc new vcpu and add it to vcpu list.
						 */
						flag = 0;

						VcpuTimes *newVcpuTimes = (VcpuTimes *) malloc(sizeof(VcpuTimes));

						newVcpuTimes->vcpuId = event->data[1];
						newVcpuTimes->totalVcpuTime = lastRuntime;

						list_add_tail(&(newVcpuTimes->vcpuList), headVcpuList);

						tmpCpuTimes->domVcpuTimes[domIdIndex].numVcpus++;
					}

					tmpCpuTimes->domVcpuTimes[domIdIndex].totalDomTime += runtime;
					tmpCpuTimes->totalCpuTime += runtime;
					tmpCpuTimes->prevNs = event->ns;
					
					tmpCpuTimes->prevDomId = event->data[2];
					tmpCpuTimes->prevVcpuId = event->data[3];

					return SUCCESS;
				}
				else
				{
					fprintf(stderr, "Anomaly: Prev switched dom and vcpu don't match\n");
					return FAIL;
				}
			}
		}
	}

	/* If here, means either list is empty or 
	 * list doesn't have cpu.
	 *
	 * Malloc new obj and add it to list.
	 */

	lastRuntime = switch_infprev_last_runtime(event->cpu, domId);

	CpuTimes *newCpuTimes = (CpuTimes *) malloc(sizeof(CpuTimes));
	VcpuTimes *newVcpuTimes = (VcpuTimes *) malloc(sizeof(VcpuTimes));

	/* Init array to 0 */
	memset(newCpuTimes->domVcpuTimes, 0, sizeof(DomVcpuTimes)*MAX_DOMS);

	newCpuTimes->prevNs = event->ns;
	newCpuTimes->prevDomId = event->data[2];
	newCpuTimes->prevVcpuId = event->data[3];

	newCpuTimes->cpuId = event->cpu;
	newCpuTimes->domVcpuTimes[domIdIndex].domId = event->data[0];	/* DomId is the prev/switched domain */

	newCpuTimes->totalCpuTime = lastRuntime;
	newCpuTimes->domVcpuTimes[domIdIndex].totalDomTime = lastRuntime;

	int i;
	for(i = 0; i < MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(newCpuTimes->domVcpuTimes[i].vcpuTimes.vcpuList));
	}

	newVcpuTimes->vcpuId = event->data[1];
	newVcpuTimes->totalVcpuTime = lastRuntime;
	newCpuTimes->domVcpuTimes[domIdIndex].numVcpus++;

	list_add_tail(&(newCpuTimes->cpuList), &(cpuTimes.cpuList));

	list_add_tail(&(newVcpuTimes->vcpuList), &(newCpuTimes->domVcpuTimes[domIdIndex].vcpuTimes.vcpuList));

	return SUCCESS;
}


int switch_sched_finalize(EventHandler *handler)
{
	CpuTimes *tmpCpuTimes;
	list_head *headCpuList = &(cpuTimes.cpuList);

	if(!list_empty(headCpuList))
	{
		/* Using a 2d array simplifies the code greatly.
		 * Hence use the max numVcpus in the cpu list
		 * as the 2nd dimension of array.
		 */

		unsigned int maxNumVcpus = calc_max_num_vcpus(headCpuList);

		tmpCpuTimes = NULL;

		int j;
		unsigned long long totalRuntime = 0;
		unsigned long long totalDomRuntime[MAX_DOMS][maxNumVcpus+1];

		memset(totalDomRuntime, 0, sizeof(unsigned long long)*MAX_DOMS*(maxNumVcpus+1));

		/* Iterate through all cpus.
		 * Calculate and store total runtimes for CPUs, domains and VCPUs.
		 */
		list_for_each_entry(tmpCpuTimes, headCpuList, cpuList)
		{
			int i;

			totalRuntime += tmpCpuTimes->totalCpuTime;

			for(i = 0; i < MAX_DOMS; i++)
			{
				/* index 0 has total dom time */
				/* index 1 and up has total vcpu time across all cpus */
				totalDomRuntime[i][0] += tmpCpuTimes->domVcpuTimes[i].totalDomTime;

				VcpuTimes *tmpVcpuTimes;
				list_head *headVcpuList = &(tmpCpuTimes->domVcpuTimes[i].vcpuTimes.vcpuList);
				
				list_for_each_entry(tmpVcpuTimes, headVcpuList, vcpuList)
				{
					totalDomRuntime[i][tmpVcpuTimes->vcpuId + 1] += tmpVcpuTimes->totalVcpuTime;
				}
			}
		}

		tmpCpuTimes = NULL;

		/* Produce util by Cpus */
		list_for_each_entry(tmpCpuTimes, headCpuList, cpuList)
		{
			unsigned long long cpuUtil = tmpCpuTimes->totalCpuTime - tmpCpuTimes->domVcpuTimes[MAX_DOMS - 1].totalDomTime;
			printf("Physcial CPU Utilization:\tCPU %d = %5.2f %%\n", 
					tmpCpuTimes->cpuId,
					(float)cpuUtil/totalRuntime * 100);
		}

		printf("\n");

		/* Produce util by Domains */
		for(j = 0; j < MAX_DOMS; j++)
		{
			int k;

			if(totalDomRuntime[j][0])
			{
				printf("\nDomain CPU Utilization:\t\tDom %d = %5.2f %%\n",
						(j == (MAX_DOMS - 1)) ? 0x7fff : j,
						(float)totalDomRuntime[j][0]/totalRuntime * 100);

				/* Produce util by Vcpus */
				for(k = 0; k < maxNumVcpus; k++)
				{
					if(totalDomRuntime[j][k+1])
					{
						printf("\tVCPU Utilization:\tVCPU %d = %5.2f %%\n",
								k,
								(float)totalDomRuntime[j][k+1]/totalDomRuntime[j][0] * 100);
					}
				}
			}
		}

		tmpCpuTimes = NULL;
		
		free_list_mallocs(headCpuList);
	}
	
	return SUCCESS;
}

void switch_sched_reset(void)
{
	list_head *headCpuList = &(cpuTimes.cpuList);
	
	free_list_mallocs(headCpuList);

	INIT_LIST_HEAD(&(cpuTimes.cpuList));
	cpuTimes.prevNs = 0;
	cpuTimes.cpuId = 0;
	cpuTimes.prevDomId = 0;
	cpuTimes.prevVcpuId = 0;
	cpuTimes.totalCpuTime = 0;	
	memset(cpuTimes.domVcpuTimes, 0, sizeof(DomVcpuTimes)*MAX_DOMS);

}

/* Calculate max vcpus used by a domain */
unsigned int calc_max_num_vcpus(list_head *headCpuList)
{
	int i;
	unsigned int maxNumVcpus = 0;
	unsigned int numVcpus[MAX_DOMS];
	CpuTimes *tmpCpuTimes;

	memset(numVcpus, 0, sizeof(unsigned int)*MAX_DOMS);

	list_for_each_entry(tmpCpuTimes, headCpuList, cpuList)
	{
		for(i = 0; i < MAX_DOMS; i++)
		{
			numVcpus[i] += tmpCpuTimes->domVcpuTimes[i].numVcpus;
		}
	}

	for(i = 0; i < MAX_DOMS; i++)
	{
		if(numVcpus[i] > maxNumVcpus)
			maxNumVcpus = numVcpus[i];
	}

	return maxNumVcpus;
}

/* Free all malloc'd memory used in lists.
 * Verified in valgrind.
 * 	- Benign "Still reachable memory" warning.
 * 	- TODO: Read in freed memory in below func.
 * 		No impact on correctness or perfromance.
 */
void free_list_mallocs(list_head *headCpuList)
{
	CpuTimes *tmpCpuTimes;

	/* Free malloc'd memory */
	list_for_each_entry_reverse(tmpCpuTimes, headCpuList, cpuList)
	{
		int i;
		for(i = 0; i < MAX_DOMS; i++)
		{
			VcpuTimes *tmpVcpuTimes;
			list_head *headVcpuList = &(tmpCpuTimes->domVcpuTimes[i].vcpuTimes.vcpuList);

			list_for_each_entry_reverse(tmpVcpuTimes, headVcpuList, vcpuList)
			{
				free(tmpVcpuTimes);
			}
		}

		free(tmpCpuTimes);
	}
}

struct EventHandler switchSchedHandler =
{
	.name = "__enter_scheduler",
	.event_id =TRC_SCHED_SWITCH,
	.init = switch_sched_init,
	.data = (void*)&cpuTimes,
	.process_event = switch_sched_handler,
	.finalize = switch_sched_finalize,
};
