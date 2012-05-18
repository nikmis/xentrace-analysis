#ifndef	__EVENT
#define	__EVENT

#include "Macros.h"

/* Struct for storing Events.
   For Xen 4.1 and above */
typedef struct Event
{
	unsigned int cpu;
	unsigned int event_id;
	unsigned int n_data;
	unsigned int tsc_in;
	unsigned long long tsc;
	/* nano seconds is much better metric than tsc since it's synchronized across cpus/cores. 
	 * Logs can be sorted without much error in timeline. */
	unsigned long long ns;
	unsigned int data[MAX_EV_DATA];
} Event;

/* CODE CLEANUP: Modify the Event struct to incorporate 32, 64 bit compatibility.
 * 		 Also remove t_rec struct and use only Event struct
 *
 * This structure represents a single trace buffer record.
   Similar to Event struct above
   For Anton's Xen TT only */
typedef struct t_rec 
{
    unsigned long long cycles;          /* cycle counter timestamp */
    unsigned long long ns;          	/* nano seconds timestamp */
    unsigned int event;			/* event ID                */
    unsigned long data[5];		/* event data items        */
} t_rec;

/* Helper functions */
int check_null(void *, void *);

/* Generic event parse functions */
int parse_event(Event *ev, FILE *fp, unsigned int ev_id);
int parse_next_event(Event *, FILE *);
void clear_event(Event *);

/* Modular event parse functions */
int parse_event_id(Event *, FILE *);
int parse_timestamp(Event *, FILE *);
int parse_event_data(Event *, FILE *);

#endif
