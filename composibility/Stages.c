#include <stdio.h>
#include "Pipe.h"
#include "Event.h"
#include "Trace.h"
#include "Stages.h"

Event event_id(Stage *s, Event ev, void *data)
{
	Event tmpev;
	init_event(&tmpev);

	unsigned int id = *(unsigned int *)s->data;
	if(ev.event_id == id)
		return ev;
	else
		return tmpev;
}

void free_event_id(Stage *s)
{
	
}

Event count(Stage *s, Event ev, void *data)
{
	Event tmpev;
	init_event(&tmpev);

	if(ev.event_id == INVALID)
	{
		return ev;
	}
	else
	{
		if(s->data == NULL)
		{
			s->data = (unsigned int *)malloc(sizeof(unsigned int));
			*((unsigned int *)(s->data)) = 0;
		}
		*(unsigned int *)s->data += 1;
	}

	return tmpev;
}

void free_count(Stage *s)
{
	printf("Count = %u\n", *((unsigned int *)s->data));
	free(s->data);
}
