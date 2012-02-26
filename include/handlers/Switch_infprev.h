#ifndef __HDLR_SWITCH_INFPREV
#define __HDLR_SWITCH_INFPREV

/* Struct for storing SWITCH_INFPREV related data */
typedef struct
{
	unsigned int dom_id;
	unsigned long long runtime;
} DomTime;

/* Event specfic parse functions */
int parse_trap(Event *, FILE *);
void generate_cpu_share_stats(FILE *, unsigned int);
unsigned short add_time_to_list(DomTime *, Event *);

#endif
