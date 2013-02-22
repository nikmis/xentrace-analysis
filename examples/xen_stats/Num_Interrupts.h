#ifndef __HDLR_NUM_INTERRUPTS
#define __HDLR_NUM_INTERRUPTS

#include "Num_Exceptions.h"

typedef struct NumInterrupts
{
	NumVectors numVectors;
	unsigned long long totalInterruptCount;
} NumInterrupts;

int num_interrupts_init(EventHandler *handler);
int num_interrupts_handler(EventHandler *handler, Event *event);
int num_interrupts_finalize(EventHandler *handler);
void num_interrupts_reset(void);

#endif
