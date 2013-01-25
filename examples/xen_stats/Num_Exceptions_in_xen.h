#ifndef __HDLR_NUM_EXCEPTIONS_IN_XEN
#define __HDLR_NUM_EXCEPTIONS_IN_XEN

#include "Num_Exceptions.h"

typedef struct NumExceptionsInXen
{
	NumVectors numVectors;
	unsigned long long totalExceptionCount;
} NumExceptionsInXen;

int num_exceptions_in_xen_init(EventHandler *handler);
int num_exceptions_in_xen_handler(EventHandler *handler, Event *event);
int num_exceptions_in_xen_finalize(EventHandler *handler);
void num_exceptions_in_xen_reset(void);

#endif
