#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

#define FAIL 		-1
#define SUCCESS 	0
#define MAX_EV_DATA 	7
#define MAX_DOMS	25

#define TRAP 		0x0020f003
#define SWITCH_INFPREV	0x0002800e
#define SWITCH_INFNEXT	0x0002800f

/* Struct for storing Events */
typedef struct 
{
	unsigned int event_id;
	unsigned int n_data;
	unsigned int tsc_in;
	unsigned long long tsc;
	unsigned int data[MAX_EV_DATA];
} Event;

/* Struct for storing SWITCH_INFPREV related data */
typedef struct
{
	unsigned int dom_id;
	unsigned long long runtime;
} DomTime;

/* Helper functions */
int check_null(void *, void *);
unsigned short add_time_to_list(DomTime *, Event *);

/* Generic event parse functions */
int parse_event(Event *, FILE *, unsigned int);
int parse_next_event(Event *, FILE *);
void clear_event(Event *);

/* Modular event parse functions */
int parse_event_id(Event *, FILE *);
int parse_timestamp(Event *, FILE *);
int parse_event_data(Event *, FILE *);

/* Event specfic parse functions */
int parse_trap(Event *, FILE *);
void generate_cpu_share_stats(FILE *, unsigned int);


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

	fclose(fp);

	return 0;
}

/* Description:	Event specfic parse function
   Parses SWITCH_INFPREV events and prints the cpu time-sharing stats 

   To-Do: 
   - Use Linked list instead of fixed len array for DomTime. 
 */
void generate_cpu_share_stats(FILE *fp, unsigned int ev_id)
{
	unsigned long long	total_time = 0;
	unsigned short		num_of_doms = 0;
	unsigned short		i = 0;
	Event ev;
	DomTime dt[MAX_DOMS];

	/* Init Struct */
	clear_event(&ev);

	while(!feof(fp))
	{
		if(parse_event(&ev, fp, SWITCH_INFPREV) != FAIL)
		{
			num_of_doms = add_time_to_list(dt, &ev);
			total_time += ev.data[1];
		}
	} /* EOF reached. */
	
	while((i < num_of_doms) && (i < MAX_DOMS))
	{
		printf("Dom ID = %d Cpu_Time = %lld Total_Time = %lld Cpu_Time_Share = %.2f %%\n", 
				dt[i].dom_id, 
				dt[i].runtime, 
				total_time, 
				((float)dt[i].runtime/total_time)*100);
		++i;
	}
}

/* Description:	Helper function for generate_cpu_share_stats()
   Searches for existing domain id data.
   if dom exists
   	update runtime
   else
   	add new dom and its runtime
 */
unsigned short add_time_to_list(DomTime *dt, Event *ev)
{
	static unsigned short num = 0;
	unsigned short i = 0;

	/* Search if domain already exists. 

	   Hash Table is the ideal abstraction for
	   looking up dom ids. Instead I am using
	   Linear Search on the list/array, since list 
	   size will be small and HT will be overkill.
	 */
	while(i < num)
	{
		if(dt[i].dom_id == ev->data[0])
		{
			dt[i].runtime += ev->data[1];

			/* No new doms added */
			return num;
		}
		
		++i;
	}

	if (num == MAX_DOMS)
	{
		/* No new doms can be added */
		return num;
	}

	/* If not found add new dom and its time.
	   Update num
	 */
	dt[i].dom_id = ev->data[0];
	dt[i].runtime = ev->data[1];

	num = i + 1; /* +1 since array index starts at 0 */

	return num;
}

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
/* Description:	Generic event parse function
   Parses the next event in log file. 
   Uses modular parse functions
 */
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
