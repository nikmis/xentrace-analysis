#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include "EventHandler.h"
#include "Macros.h"
#include "Reader.h"
#include "CpuList.h"

CpuList *cpus;

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
	cpus = (CpuList *) malloc(sizeof(CpuList));
	init_cpulist(cpus);

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
	free_cpulist(cpus);

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

		update_cpulist(cpus, event.cpu);

	} while(!feof(reader->fp)); /* Update condition to check for sorted array not fp */

	evh_call_finalizers(reader);

	printf("\n\nTotal time spent collecting records is %15.3f (ms) \n", (float)get_total_time(cpus)/MEGA);
	printf("Total event records collected = %llu\n", get_total_event_records());

	return SUCCESS;
}
