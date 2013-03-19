#ifndef __PARSEHEAP_H
#define __PARSEHEAP_H

#include <stdio.h>
//#include "Parse.h"
#include "Macros.h"

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

#endif
