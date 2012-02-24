#include <stdlib.h>
#include "Macros.h"
#include "Reader.h"


void reader_init(Reader *reader, const char *filename)
{
	if((reader->fp = fopen(filename, "rb")) == NULL)
	{
		strerror(errno);
		printf("Usage: ./a.out <filename>\n");
		exit(0);
	}

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
	int i = 0;
	fclose(reader->fp);

	free(reader->handler_array);
	return;
}

int reader_loop(Reader *reader)
{

