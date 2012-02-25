#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>

#include "macros.h"
#include "Event.h"


int main(int argc, char *argv[])
{
	FILE *fp;
	Event event; 

	/* Init struct */
	clear_event(&event);

	if((fp = fopen(argv[1], "rb")) == NULL)
	{
		strerror(errno);
		printf("Usage: ./a.out <filename>\n");
		exit(0);
	}

	while(!feof(fp))
	{
		if(parse_event(&event, fp, TRAP) != FAIL)
		{
			printf("event = %d, tsc = %lld, eip = %d, error = %d\n", event.event_id, event.tsc, event.data[0], event.data[1]);
		}

		clear_event(&event);
	}

	fclose(fp);

	return 0;
}

