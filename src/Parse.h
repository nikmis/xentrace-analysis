#ifndef __PARSE_H
#define __PARSE_H

#include "Event.h"
#include "Macros.h"

typedef struct CpuOffset
{
	char 		cpuFlag;	// bool flag
	off_t 		nextOffset;
	Event		ev;

} CpuOffset;

typedef struct Parse
{
	// Data
	CpuOffset	cpuOff[MAX_CPUS];
	Heap 		h;
	FILE* 		fp;
	int 		numCpus;

	// Flags
	int setup_flag;

	// Methods
	void 	(*setup)(Parse *self)
	void	(*init_cpu_offset)(Parse *self);
	Event	(*get_next_event)(Parse *self);
} Parse;

#endif
