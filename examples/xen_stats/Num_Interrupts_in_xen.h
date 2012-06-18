#ifndef __HDLR_NUM_INTERRUPTS_IN_XEN
#define __HDLR_NUM_INTERRUPTS_IN_XEN

#include "Num_Exceptions.h"

typedef struct NumInterruptsInXen
{
	NumVectors numVectors;
	unsigned long long totalInterruptCount;
} NumInterruptsInXen;

int num_interrupts_in_xen_init(EventHandler *handler);
int num_interrupts_in_xen_handler(EventHandler *handler, Event *event);
int num_interrupts_in_xen_finalize(EventHandler *handler);
void num_interrupts_in_xen_reset(void);

#endif
