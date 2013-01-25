#ifndef __HDLR_EXIT_TO_XEN
#define __HDLR_EXIT_TO_XEN

typedef struct DomRuntime
{
	unsigned long long 	lastExitToXen;
	unsigned long long 	runtime[MAX_DOMS];
	unsigned int 		cpuId;
	list_head		cpuList;
} DomRuntime;

int exit_to_xen_init(EventHandler *handler);
int exit_to_xen_handler(EventHandler *handler, Event *event);
int exit_to_xen_finalize(EventHandler *handler);
void exit_to_xen_reset(void);

/* Accessor functions */
unsigned long long get_last_exit_to_xen(unsigned int cpuId);

#endif
