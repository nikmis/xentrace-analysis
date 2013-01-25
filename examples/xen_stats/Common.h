#ifndef __HDLR_NUM_EV
#define __HDLR_NUM_EV

typedef struct NumTypes
{
	unsigned int 	type;
	unsigned int 	totalTypeCount;
	list_head 	typeList;
} NumTypes;

typedef struct NumEvs
{
	NumTypes numTypes;
	unsigned long long totalEvsCount;
} NumEvs;

int num_ev_init(NumEvs *dat);
int num_ev_handler(NumEvs *dat, Event *event);
int num_ev_finalize(NumEvs *dat, char *evName, char *typeName);
void num_ev_reset(NumEvs *dat);

int num_ev_in_xen_init(NumEvs *dat);
int num_ev_in_xen_handler(NumEvs *dat, Event *event);
int num_ev_in_xen_finalize(NumEvs *dat, char *evName, char *typeName);
void num_ev_in_xen_reset(NumEvs *dat);

#endif
