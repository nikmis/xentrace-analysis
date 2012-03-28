#ifndef __HDLR_TRAP
#define __HDLR_TRAP

int trap_init(EventHandler *handler);
int trap_finalize(EventHandler *handler);
int trap_event(EventHandler *handler, Event *event);

#endif
