#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

#define FAIL 		-1
#define SUCCESS 	0
#define MAX_EV_DATA 	7

#define TRAP 		0x0020f003
#define SWITCH_INFPREV	0x0002800e
#define SWITCH_INFNEXT	0x0002800f

typedef struct 
{
	unsigned int event_id;
	unsigned int n_data;
	unsigned int tsc_in;
	unsigned long long tsc;
	unsigned int data[MAX_EV_DATA];
} Event;

/* Helper functions */
int check_null(void *, void *);

/* Generic event parse */
int parse_event(Event *, FILE *, unsigned int);
int parse_next_event(Event *, FILE *);
void clear_event(Event *);

/* Modular event parse */
int parse_event_id(Event *, FILE *);
int parse_timestamp(Event *, FILE *);
int parse_event_data(Event *, FILE *);

/* Event specfic parse */
int parse_trap(Event *, FILE *);

int main(int argc, char *argv[])
{
	FILE *fp;
	Event event; 

	/* Init struct */
	clear_event(&event);

	if((fp = fopen(argv[1], "rb")) == NULL)
	{
		strerror(errno);
		printf("Usage: ./a.out <filename>\n");
		exit(0);
	}

	while(!feof(fp))
	{
		/* 
		Example: Using granular parse handlers  
		
		if(parse_event_id(&event, fp) == FAIL)
		{
			fprintf(stderr, "%s:%d: Parsing of event failed\n", __FILE__, __LINE__); 
			exit(0);
		}
		
		if(parse_timestamp(&event, fp) == FAIL)
		{
			fprintf(stderr, "%s:%d: Parsing of timestamp failed\n", __FILE__, __LINE__); 
			exit(0);
		}

		if(parse_event_data(&event, fp) == FAIL)
		{
			fprintf(stderr, "%s:%d: Parsing of event data failed\n", __FILE__, __LINE__); 
			exit(0);
		}
		*/

		/* 
		Example: Using generic parse handlers  

		if(parse_next_event(&event, fp) == FAIL)
		{
			fprintf(stderr, "%s:%d: Parsing of event failed\n", __FILE__, __LINE__); 
			exit(0);
		}

		if(event.event_id == TRAP)
		{
			printf("event = %d, tsc = %lld, eip = %d, error = %d\n", event.event_id, event.tsc, event.data[0], event.data[1]);
		}
		*/

		if(parse_event(&event, fp, TRAP) != FAIL)
		{
			printf("event = %d, tsc = %lld, eip = %d, error = %d\n", event.event_id, event.tsc, event.data[0], event.data[1]);
		}

		clear_event(&event);
	}

	return 0;
}

int parse_event(Event *ev, FILE *fp, unsigned int ev_id)
{
	Event tmp_ev;

	/* Init struct */
	clear_event(&tmp_ev);

	while(!feof(fp))
	{
		if(parse_next_event(&tmp_ev, fp) == FAIL)
		{
			fprintf(stderr, "%s:%d: parse_next_event returned FAIL\n", __FILE__, __LINE__);
			return FAIL;
		}
		
		if(tmp_ev.event_id == ev_id)
		{
			memcpy(ev, &tmp_ev, sizeof(tmp_ev));
			return SUCCESS;
		}
	}

	/* fprintf(stdout, "%s:%d:parse_event: EOF file reached", __FILE__, __LINE__); */
	return FAIL;
}

int parse_next_event(Event *ev, FILE *fp)
{
	if(parse_event_id(ev, fp) == FAIL)
	{
		fprintf(stderr, "%s:%d: parse_event_id returned FAIL\n", __FILE__, __LINE__);
		return FAIL;
	}

	if(parse_timestamp(ev, fp) == FAIL)
	{
		fprintf(stderr, "%s:%d: parse_timestamp returned FAIL\n", __FILE__, __LINE__);
		return FAIL;
	}

	if(parse_event_data(ev, fp) == FAIL)
	{
		fprintf(stderr, "%s:%d: parse_event_data returned FAIL\n", __FILE__, __LINE__);
		return FAIL;
	}

	return SUCCESS;
}

int parse_event_id(Event *ev, FILE *fp)
{
	unsigned int x; 

	if(check_null(ev, fp))
	{
		fprintf(stderr, "%s:%d: Null pointers passed\n", __FILE__, __LINE__);
		return FAIL;
	}

	if(fread(&x, sizeof(unsigned int), 1, fp) == 0)
	{
		if(!feof(fp))
		{
			fprintf(stderr, "%s:%d: File read failed\n", __FILE__, __LINE__);
			return FAIL;
		}
	}

	ev->n_data = x >> 28 & 0x7;
	ev->tsc_in = x >> 31;
	
	ev->event_id = x & 0x0fffffff;

	return SUCCESS;
}

int parse_timestamp(Event *ev, FILE *fp)
{
	unsigned long long x;

	if(check_null(ev, fp))
	{
		fprintf(stderr, "%s:%d: Null pointers passed\n", __FILE__, __LINE__);
		return FAIL;
	}

	/* No timestamp information */
	if(ev->tsc_in != 1)
	{
		ev->tsc = 0;
		return SUCCESS;
	}

	if(fread(&x, sizeof(unsigned long long), 1, fp) == 0)
	{
		if(!feof(fp))
		{
			fprintf(stderr, "%s:%d: File read failed\n", __FILE__, __LINE__);
			return FAIL;
		}
	}
	
	ev->tsc = x;

	return SUCCESS;
}

int parse_event_data(Event *ev, FILE *fp)
{
	unsigned int i = 0;
	unsigned int x;

	if(check_null(ev, fp))
	{
		fprintf(stderr, "%s:%d: Null pointers passed\n", __FILE__, __LINE__);
		return FAIL;
	}

	for(i = 0; i < ev->n_data; i++)
	{
		if(fread(&x, sizeof(unsigned int), 1, fp) == 0)
		{
			if(!feof(fp))
			{	
				fprintf(stderr, "%s:%d: File read failed\n", __FILE__, __LINE__);
				return FAIL;
			}
		}
		
		ev->data[i] = x;
		x = 0;
	}

	return SUCCESS;
}

void clear_event(Event *ev)
{
	ev->event_id = 0;
	ev->n_data = 0;
	ev->tsc_in = 0;
	ev->tsc = 0;
	memset(ev->data, 0, MAX_EV_DATA);
}
		
int check_null(void *ev, void *fp)
{
	if((ev == NULL) || (fp == NULL))
	{
		return FAIL;
	}
	else
		return SUCCESS;
}
