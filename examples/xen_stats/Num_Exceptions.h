#ifndef __HDLR_NUM_EXCEPTIONS
#define __HDLR_NUM_EXCEPTIONS

typedef struct NumVectors
{
	unsigned int 	vector;
	unsigned int 	totalVectorCount;
	list_head 	vectorList;
} NumVectors;

typedef struct NumExceptions
{
	NumVectors numVectors;
	unsigned long long totalExceptionCount;
} NumExceptions;

int num_exceptions_init(EventHandler *handler);
int num_exceptions_handler(EventHandler *handler, Event *event);
int num_exceptions_finalize(EventHandler *handler);
void num_exceptions_reset(void);

#endif
