#include <stdio.h>
#include <stdlib.h>
#include "Event.h"
#include "Macros.h"
#include "Parse.h"

void parse_setup(Parse *p)
{
	if(p->setup_flag)
		return;
	
	// Init cpuOffset array
	memset(p->cpuOff, 0, MAX_CPUS*sizeof(struct CpuOffset));

	p->h.init();
		
	p->init_cpu_offset(p);
	p->setup_flag = 1;
}

void parse_init_cpu_offset(Parse *self)
{
	Event tmpev;
	
	self->numCpus = 0;

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

Event parse_next_event_for_cpu(Parse *self, int cpu)
{
	// retrieve ev with smallest ns
	CpuOffset coff = self->h.pop(&h);

	off_t offt = coff.nextOffset;
	
	fseek(fp, offt, SEEK_SET);

	Event tmpev;
	
	// Push next ev for retrieved cpu on heap
	while(!feof)
	{
		if(parse_next_event(&tmpev, p->fp) != SUCCESS)
			break;
		
		if(coff.ev.cpu == tmpev.cpu)
		{
			self->h.push(&h, tmpev);
			break;
		}
	}

	rewind(fp);

	return coff.ev;
}

Parse par =
{
	.setup = parse_setup,
	.init_cpu_offset = parse_init_cpu_offset,
	.next_event_for_cpu = parse_next_event_for_cpu,
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

	for(int cpu = 0; cpu < numCpus; cpu++)
	{
		h.push(next_event_for_cpu(cpuOff, MAX_CPUS, cpu));
	}
	
	Event tmpev = h.pop(&h);

	memcpy(ev, tmpev, sizeof(Event));

	h.push(next_event_for_cpu(cpuOff, MAX_CPUS, ev->cpu));

	return 0;
}

