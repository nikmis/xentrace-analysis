#include <stdio.h>
#include "Event.h"
#include "Parse.h"
#include "Pipe.h"
#include "Stages.h"

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

	Stage *s1 = create_stage(event_id, free_event_id, (int []){TRAP});
	Stage *s2 = create_stage(count, free_count, NULL);

	pipe(s1, s2);

	Event event;
	int ret = 0, i = 0;
	do
	{
		i++;
		ret = parse_return_next_event(&event);
		if(ret == FAIL)
			break;
		execute_pipe(s1, event, NULL);
	//} while(i < COUNT);
	} while(!feof(fp));

	free_stage(s1);
	free_stage(s2);

	return 0;
}
