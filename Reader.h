#ifndef __READER
#define __READER

#include "EventHandler.h"

typedef struct Reader
{
	FILE 		*fp;
	EventHandler	*handler_array;
}

void reader_init(Reader *reader, const char *filename);
void reader_exit(Reader *reader);
int reader_loop(Reader *reader);
#endif
