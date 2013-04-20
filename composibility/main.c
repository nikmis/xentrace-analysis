#include <stdio.h>
#include "Event.h"
#include "Parse.h"
#include "ParseHeap.h"
#include "Pipe.h"
#include "Stages.h"

Stage* build_tree(void)
{
	Stage *s1 = create_stage(event_id, (int []){5});
	Stage *s2 = create_stage(count, NULL);

	pipe(s1, s2);

	return s1;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		perror("Reader: Couldn't open log file");
		printf("Usage: ./a.out <filename>. Given filename %s\n", filename);
		exit(0);
	}

	Parse *p = get_parse();
	p->setup(p, fp);

	Stage *s = build_tree();
	Event event;
	do
	{
		ret = parse_return_next_event(&event);
		execute_pipe(s, event, NULL);
	} while(!feof(fp));

	return 0;
}

