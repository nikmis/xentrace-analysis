#define SIZE 10

typedef Event (*StageFunc)(struct Stage *s, Event ev);

typedef struct Stage
{
	StageFunc f;
	StageFunc next[SIZE];
} Stage;

void pipe(Stage *s1, Stage *s2)
{
	int i = 0;
	while((i < SIZE) && (s1->next[i] != NULL))
		i++;
	if(i < SIZE)
	{
		memcpy(&s1->next[i], s2, sizeof(Stage));
	}
	else
	{
		fprintf(stderr, "Cannot Pipe s2 to s1. Pipe full\n");
	}
}

Event execute_pipe(Stage *s, Event ev)
{
	Event tmpev;
	event_init(&tmpev);

	if(ev.id != INVALID)
	{
		tmpev = s->f(s, ev);	
		
		int i = 0;
		// Doesnt account for logical OR and joins
		while((i < SIZE) && (s->next[i] != NULL))
		{
			execute_pipe(s->next[i], tmpev);
		}
		
	}

	return tmpev;
}

