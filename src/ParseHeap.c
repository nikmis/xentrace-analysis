#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Parse.h"
#include "Macros.h"

void mh_init(MinHeap *mheap)
{
	memset(mheap->mh, 0, (MAX_CPUS+1)*sizeof(CpuOffset));
	mheap->num = 0;

	mheap->init = mh_init;
	mheap->push = mh_push;
	mheap->pop = mh_pop;
}

int mh_parent(int n)
{
	if(n == 1)	return -1;
	return (int)(n/2);
}

int mh_child(int n)
{
	return n*2;
}

void mh_swap(MinHeap *mheap, int n, int par)
{
	CpuOffset tmp = mheap->mh[n];
	mheap->mh[n] = mheap->mh[par];
	mheap->mh[par] = tmp;
}

void mh_bubble_up(MinHeap *mheap, int num)
{
	if(mh_parent(num) == -1) return;

	if(mheap->mh[mh_parent(num)].ev.ns > mheap->mh[num].ev.ns)
	{
		mh_swap(mheap, num, mh_parent(num));
		mh_bubble_up(mheap, mh_parent(num));
	}
}

void mh_push(MinHeap *mheap, CpuOffset x)
{
	if(mheap->num >= MAX_CPUS)
		fprintf(stderr, "Parseheap.c: Heap full\n");
	else
	{
		mheap->num++;
		mheap->mh[mheap->num] = x;
		mh_bubble_up(mheap, mheap->num);
	}
}

void mh_bubble_down(MinHeap *mheap, int p)
{
	int child = mh_child(p);
	int min_index = p;

	for(int i = 0; i <=1; i++)
	{
		if((child+i) <= mheap->num)
		{
			if(mheap->mh[min_index].ev.ns > mheap->mh[child+i].ev.ns)
				min_index = child+i;
		}
	}

	if(min_index != p)
	{
		mh_swap(mheap, p, min_index);
		mh_bubble_down(mheap, min_index);
	}
}

int mh_pop(MinHeap *mheap, CpuOffset *min)
{
	CpuOffset tmpmin;
	
	if(mheap->num <= 0)	
	{
		fprintf(stderr, "Parseheap.c: Pop() from empty heap\n");
		return FAIL;
	}
	else
	{
		tmpmin = mheap->mh[1];
		mheap->mh[1] = mheap->mh[mheap->num];
		mheap->num--;
		mh_bubble_down(mheap, 1);
	}

	memcpy(min, &tmpmin, sizeof(CpuOffset));

	return SUCCESS;
}

void make_heap(MinHeap *mheap, CpuOffset s[], int n)
{
	mh_init(mheap);

	for(int i = 0; i < n; i++)
		mh_push(mheap, s[i]);
}

/*
void heap_sort(CpuOffset s[], int n)
{
	MinHeap mh;

	make_heap(&mh, s, n);

	for(int i = 0; i < n; i++)
	{
		s[i] = mh_pop(&mh);
	}

}
void print_array(CpuOffset s[], int n)
{
	for(int i = 0; i < n; i++)
	{

	}
	printf("\n");
}
*/
