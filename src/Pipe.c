#define SIZE 10

typedef enum StageType = {NORMAL, SPLIT, JOIN};

typedef Event (*StageFunc)(struct Stage *s, Event ev);

typedef struct Stage
{
	StageFunc f;
	StageType nextType;
	union
	{
		StageFunc next;
		StageFunc list_next[SIZE];
	};

} Stage;

void pipe(Stage *s1, Stage *s2)
{
	s1->nextType = NORMAL;

	memcpy(&s1->next, s2, sizeof(Stage));
}

void split(Stage *s1, Stage *s2)
{
	s1->nextType = SPLIT;

	int i = 0;
	while((i < SIZE) && (s1->list_next[i] != NULL))
		i++;
	if(i < SIZE)
	{
		memcpy(&s1->list_next[i], s2, sizeof(Stage));
	}
	else
	{
		fprintf(stderr, "Cannot Pipe s2 to s1. Pipe full\n");
	}
}

void join(Stage *s1, Stage *s2)
{
	s1->nextType = JOIN;

	memcpy(&s1->next, s2, sizeof(Stage));
}

Event execute_pipe(Stage *s, Event ev)
{
	Event tmpev;
	event_init(&tmpev);

	if(ev.id != INVALID)
	{
		tmpev = s->f(s, ev);	

		switch(s->nextType)
		{
			case NORMAL : execute_pipe(&s->next, tmpev);
				      break;
			case SPLIT  : 
				      {
					      int i = 0;
					      // Doesnt account for logical OR and joins
					      while((i < SIZE) && (s->next[i] != NULL))
					      {
						      execute_pipe(s->next[i], tmpev);
					      }
				      }
				      break;
			case JOIN   : 
				      break;
	}

	return tmpev;
}

