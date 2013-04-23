#include <stdio.h>
#include "Event.h"
#include "Parse.h"
#include "Pipe.h"
#include "Stages.h"
#include "Trace.h"

#define COUNT 10000000

/*
Stage* build_tree(void)
{
}
*/
int main(int argc, char *argv[])
{
	FILE *fp;
	
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("Reader: Couldn't open log file");
		printf("Usage: ./a.out <filename>. Given filename %s\n", argv[1]);
		exit(0);
	}

	Parse *p = get_parse();
	p->setup(p, fp);

	//Stage *s = build_tree();

	Stage *s0 = create_stage(NULL, NULL, NULL);
	Stage *s1 = create_stage(event_id, free_event_id, (int []){TRC_EXIT_TO_GUEST});
	//Stage *s2 = create_stage(event_id, free_event_id, (int []){TRAP});
	Stage *s3 = create_stage(count, free_count, NULL);

	or(s0, s1);
	//or(s0, s2);

	join(s1, s3, JOIN_OR);
	//join(s2, s3, JOIN_OR);

	Event event;
	int ret = 0, i = 0;
	do
	{
		i++;
		ret = parse_return_next_event(&event);
		if(ret == FAIL)
			break;
		execute_pipe(s0, event, NULL);
	//} while(!feof(fp));
	} while(i < 100);

	free_stage(s1);
	//free_stage(s2);
	free_stage(s3);

	return 0;
}
