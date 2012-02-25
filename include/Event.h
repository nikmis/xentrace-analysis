#ifndef	__EVENT
#define	__EVENT

#include "Macros.h"

/* Struct for storing Events */
typedef struct Event
{
	unsigned int event_id;
	unsigned int n_data;
	unsigned int tsc_in;
	unsigned long long tsc;
	unsigned int data[MAX_EV_DATA];
} Event;

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
