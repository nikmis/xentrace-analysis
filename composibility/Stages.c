#include <stdio.h>
#include "Pipe.h"
#include "Event.h"
#include "Stages.h"

Event event_id(Stage *s, Event ev, void *data)
{
	Event tmpev;
	init_event(&tmpev);

	unsigned int id = *(unsigned int *)data;
	if(ev.id == id)
		return ev;
	else
		return tmpev;
}

Event count(Stage *s, Event ev, void *data)
{
	if(ev.id == INVALID)
	{
		printf("Count = %u\n", (unsigned int *)s->data);
		return ev;
	}
	else
	{
		if(s->data == NULL)
		{
			s->data = (unsigned int *)malloc(sizeof(unsigned int));
			*(s->data) = 0;
		}
		*((unsigned int *)s->data++);
	}
}
