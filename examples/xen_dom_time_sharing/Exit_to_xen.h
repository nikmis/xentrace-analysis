#ifndef __HDLR_EXIT_TO_XEN
#define __HDLR_EXIT_TO_XEN

int exit_to_xen_init(EventHandler *handler);
int exit_to_xen_handler(EventHandler *handler, Event *event);
int exit_to_xen_finalize(EventHandler *handler);
void exit_to_xen_reset(void);

#endif
