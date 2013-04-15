#define SIZE 10

typedef Event (*StageFunc)(struct Stage *s, void *data);

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


