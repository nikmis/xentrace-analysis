#include <stdio.h>
#include <string.h>
/*
#include "../include/Macros.h"
 */
#include "Macros.h"
#include "Event.h"


/* Description:	Generic event parse function
   Parses event passed as function argument.
   Returns FAIL if event not found.
 */
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

/* This structure represents a single trace buffer record. */
struct t_rec {
    unsigned long long cycles;          /* cycle counter timestamp */
    unsigned long event;           /* event ID                */
    unsigned long data[5];    /* event data items        */
};

/* Description:	Generic event parse function
   Parses the next event in log file. 
   Uses modular parse functions
 */
int parse_next_event(Event *ev, FILE *fp)
{
    int read;
    unsigned int cpu;
    struct t_rec rec;

    read = fread(&cpu, sizeof(cpu), 1, fp);
    if (read == 0) {
        fprintf(stderr, "failed to read cpu id\n");
        return FAIL;
    }

    read = fread(&rec, sizeof(rec), 1, fp);
    if (read == 0) {
        fprintf(stderr, "failed to read rec id\n");
        return FAIL;
    }

    printf("cpu:%u, cycles:%llu, event:%lu, data0:%lu, data1:%lu, data2:%lu, data3:%lu, data4:%lu\n", 
           cpu, rec.cycles, rec.event, rec.data[0], rec.data[1], rec.data[2], rec.data[3], rec.data[4]);

    ev->event_id = rec.event;
    ev->tsc = rec.cycles;
    ev->n_data = 5;
    ev->data[0] = rec.data[0];
    ev->data[1] = rec.data[1];
    ev->data[2] = rec.data[2];
    ev->data[3] = rec.data[3];
    ev->data[4] = rec.data[4];


/*
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
*/
	return SUCCESS;
}

/* Description:	Modular event parse function
   Parses the event id and related metadata.
 */
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

/* Description:	Modular event parse function
   Parses timestamp realted to event.
 */
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

/* Description:	Modular event parse function
   Parses event data 
 */
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

/* Description:	Helper function
   Sets all event data to 0
   Used to init and reset Event struct.
 */
void clear_event(Event *ev)
{
	ev->event_id = 0;
	ev->n_data = 0;
	ev->tsc_in = 0;
	ev->tsc = 0;
	memset(ev->data, 0, MAX_EV_DATA);
}

/* Description:	Helper function
   Checks null pointers.
 */
int check_null(void *ev, void *fp)
{
	if((ev == NULL) || (fp == NULL))
	{
		return FAIL;
	}
	else
		return SUCCESS;
}
