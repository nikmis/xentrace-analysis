#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include "EventHandler.h"
#include "Macros.h"
#include "Reader.h"
#include "CpuList.h"

void reader_init(Reader *reader, const char *filename)
{
	int i = 0;
	if((reader->fp = fopen(filename, "rb")) == NULL)
	{
		strerror(errno);
		printf("Usage: ./a.out <filename>\n");
		exit(0);
	}

	/* Init Cpu List */
	reader->cpus = (CpuList *) malloc(sizeof(CpuList));
	init_cpulist(reader->cpus);

	/* Init and sort logs */
	sort_events_by_ns(reader->fp);

	/* Init Hash table */
	reader->handler_array = (EventHandler *)malloc(sizeof(struct EventHandler) * MAX_EVENTS); 

	for(i = 0; i < MAX_EVENTS; i++)
	{
		/* Init evh struct */
		reader->handler_array[i].event_id = 0;
		reader->handler_array[i].data = NULL;
		/* INIT_LIST_HEAD called during handler registartion */
		reader->handler_array[i].init = NULL;
		reader->handler_array[i].process_event = NULL;
		reader->handler_array[i].finalize = NULL;
	}
	/* Need to free memory later */
}

void reader_exit(Reader *reader)
{
	fclose(reader->fp);

	free(reader->handler_array);
	free_events();
	free_cpulist(reader->cpus);

	return;
}

int reader_loop(Reader *reader)
{
	int ret = 0;
	Event event;

	clear_event(&event);

	evh_call_initializers(reader);

	do
	{
		ret = return_next_event(&event);
		if (ret == FAIL)
			break;
		evh_call_handlers(reader, &event);

		update_cpulist(reader->cpus, event.cpu);

	} while(!feof(reader->fp)); /* Update condition to check for sorted array not fp */

	/* Make sure to call get_total_time() before calling finalizers as this
	 * data maybe used by some finalizers */
	float totalTime = get_total_time(reader->cpus)/MEGA;

	evh_call_finalizers(reader);

	printf("Log Collection data\n");
	print_line();

	print_cpu_times(reader->cpus);

	printf("\nAverage time spent collecting records is %15.3f (ms)\n", (float)get_avg_total_time()/MEGA);
	printf("Total CPU time spent collecting records is %15.3f (ms) \n", totalTime);
	printf("\nTotal event records collected = %llu\n", get_total_event_records());

	return SUCCESS;
}

void print_cpu_times(CpuList *cpus)
{
	CpuList *tmpCpuList;
	list_head *head = &(cpus->cpuList);

	list_for_each_entry(tmpCpuList, head, cpuList)
	{
		printf("Total time spent on CPU %u is %15.3f (ms)\n", 
				tmpCpuList->cpu, (float)get_total_time_cpu(cpus, tmpCpuList->cpu)/MEGA);
	}
}
