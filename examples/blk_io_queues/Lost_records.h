#ifndef __HDLR_LOST_RECORDS
#define __HDLR_LOST_RECORDS

typedef struct LostRecTime
{
	int cpu;
	list_head cpuList;
	unsigned long long totalLostTime;
	unsigned long long totalLostRec;
} LostRecTime;

int lost_records_init(EventHandler *handler);
int lost_records_handler(EventHandler *handler, Event *event);
int lost_records_finalize(EventHandler *handler);

void update_lrt_cpulist(unsigned int cpu);
void free_lrt_cpulist();

#endif
