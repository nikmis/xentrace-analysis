#ifndef __PARSE_H
#define __PARSE_H

#include "Event.h"
#include "Macros.h"
//#include "ParseHeap.h"

typedef struct CpuOffset
{
	char 		cpuFlag;	// bool flag
	off_t 		nextOffset;
	Event		ev;

} CpuOffset;


typedef struct MinHeap
{
	CpuOffset mh[MAX_CPUS+1];
	int num;

	void 		(*init)(struct MinHeap *mheap);
	void 		(*push)(struct MinHeap *mheap, CpuOffset x);
	CpuOffset 	(*pop)(struct MinHeap *mheap);	
} MinHeap;


typedef struct Parse
{
	// Data
	CpuOffset	cpuOff[MAX_CPUS];
	MinHeap		h;
	FILE* 		fp;
	int 		numCpus;

	// Flags
	int setup_flag;

	// Methods
	void 	(*setup)(struct Parse *self, FILE *fp);
	void	(*init_cpu_offset)(struct Parse *self);
	Event	(*get_next_event)(struct Parse *self);
} Parse;

// MinHeap methods
void mh_init(MinHeap *mheap);
int mh_parent(int n);
int mh_child(int n);
void mh_swap(MinHeap *mheap, int n, int par);
void mh_bubble_up(MinHeap *mheap, int num);
void mh_push(MinHeap *mheap, CpuOffset x);
void mh_bubble_down(MinHeap *mheap, int p);
CpuOffset mh_pop(MinHeap *mheap);
void make_heap(MinHeap *mheap, CpuOffset s[], int n);
void heap_sort(CpuOffset s[], int n);

// Parse methods
void parse_setup(Parse *self, FILE *fp);
void parse_init_cpu_offset(Parse *self);
Event parse_get_next_event(Parse *self);
Parse* get_parse();
int parse_return_next_event(FILE *fp, Event *ev);

#endif
