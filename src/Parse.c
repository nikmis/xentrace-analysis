#include <stdio.h>
#include <stdlib.h>
#include "Event.h"
#include "Macros.h"
#include "Parse.h"

void parse_setup(Parse *self)
{
	if(self->setup_flag)
		return;
	
	// Init cpuOffset array
	memset(self->cpuOff, 0, MAX_CPUS*sizeof(struct CpuOffset));

	self->h.init();
		
	self->init_cpu_offset(p);
	self->setup_flag = 1;
}

void parse_init_cpu_offset(Parse *self)
{
	Event tmpev;

	while(!feof(fp))
	{

		if(parse_next_event(&tmpev, p->fp) != SUCCESS)
			return;

		if(!self->cpuOff[tmpev.cpu].cpuFlag)
		{
			self->cpuOff[tmpev.cpu].cpuFlag = 1;
			self->cpuOff[tmpev.cpu].nextOffset = ftell(fp);	// File off_t of next event.
			self->cpuOff[tmpev.cpu].ev = tmpev;
			self->numCpus++;

			// Push ev->ns on heap
			p->h.push(&h, self->cpuOff[tmpev.cpu]); 
		}
	}

	// Set fp to off_t 0
	rewind(fp);
}

Event parse_get_next_event(Parse *self)
{
	// retrieve ev with smallest ns
	CpuOffset coff = self->h.pop(&h);

	off_t offt = coff.nextOffset;
	
	fseek(fp, offt, SEEK_SET);

	Event nextev;
	
	// Push next ev for retrieved cpu on heap
	while(!feof)
	{
		if(parse_next_event(&nextev, p->fp) != SUCCESS)
			break;
		// If cpu of next ev is same as popped ev
		if(coff.ev.cpu == nextev.cpu)
		{
			self->h.push(&h, nextev);
			break;
		}
	}

	rewind(fp);

	return coff.ev;
}

Parse par =
{
	.setup_flag = 0,
	.numCpus = 0,
	.setup = parse_setup,
	.init_cpu_offset = parse_init_cpu_offset,
	.get_next_event = parse_get_next_event,
};

Parse* get_parse()
{
	par.setup(&par, fp);
	return &par;
}

int return_next_event(FILE *fp, Event *ev)
{
	Parse *p = get_parse();

	// calc num cpus and fill hash table
	// Read next event
	// ev = min(ns(cpu-list))
	// update offset table

	
	Event tmpev = h.pop(&h);

	memcpy(ev, tmpev, sizeof(Event));

	h.push(next_event_for_cpu(cpuOff, MAX_CPUS, ev->cpu));

	return 0;
}

