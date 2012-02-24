#ifndef __READER
#define __READER

#include "EventHandler.h"

typedef struct Reader
{
	FILE 		*fp;
	EventHandler	*handler_array;
}

#endif
