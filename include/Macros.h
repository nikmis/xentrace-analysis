#ifndef __MACROS
#define __MACROS

/* Return Values */
#define FAIL 		-1
#define SUCCESS 	0

/* Generic */
#define MAX_EV_DATA 	5
#define MAX_DOMS	25
#define MAX_VCPUS	16
#define MAX_EVENTS	200
#define MAX_CPUS	32

#define CPU_FREQ	2.4
#define MEGA		1000000

/* Event Specfic Macros  */

/* Lost Records */

#define LOST_REC_MAX_TIME 5000000000 /*  seconds (Unit in ns) */

/* Event Ids not present in xen_tt */
#define TRAP 		0x0020f003

#endif
