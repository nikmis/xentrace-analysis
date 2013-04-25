#include <stdio.h>
#include "Pipe.h"
#include "Event.h"
#include "Trace.h"
#include "Stages.h"

Event event_id(Stage *s, Event ev, void *data)
{
	Event tmpev;
	init_event(&tmpev);

	//printf("stage %p evid %u %llu\n", s, ev.event_id, ev.ns);

	unsigned int id = *(unsigned int *)s->data;
	if(ev.event_id == id)
	{
		return ev;
	}
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
/*
Event typical_join_func(Stage s, Event ev, void *data)
{
	// Wrapper function to parse dummy stage args
	// and call actual function with parsed args.
	int i = 0;
	Event args[5]; // Whatever the number of args join event takes.

	DummyStageData *dst = (DummyStageData )data;
	while(dst->args[i].s != NULL)
	{
		args[i] = dst->args[i].ev;
	}

	return actual_join_func(args[0], args[1], args[2], args[3], args[4]);
}       
*/

Event wait_time(Stage *s, Event ev, void *data)
{
	int i = 0;
	Event args[2]; // Whatever the number of args join event takes.

	DummyStageData *dst = (DummyStageData *)data;
	while((i < 2) && (dst->args[i].s != NULL))
	{
		args[i] = dst->args[i].ev;
		i++;
	}

	return wait_time_t(s, args[0], args[1]);
}

Event wait_time_t(Stage *s, Event ev1, Event ev2)
{
	Event tmpev;
	init_event(&tmpev);


	if(s->data == NULL)
	{
		s->data = (unsigned long long *)malloc(sizeof(unsigned long long));
		*(unsigned long long *)s->data = 0;

	}	

	long long diff = ev1.ns - ev2.ns;

	diff = (diff < 0) ? (-1)*diff : diff;

	*(unsigned long long *)s->data += diff;

	tmpev.data[0] = *(unsigned long long *)s->data;

	return tmpev;
}

void free_wait_time(Stage *s)
{
	if(s->data == NULL)
		return;

	unsigned long long temp = *(unsigned long long *)s->data; 
	//printf("Wait time = %20.3lf\n", ((double)temp)/MEGA);
	printf("Wait time = %20.3lf\n", ((double)temp));
	free(s->data);
}
