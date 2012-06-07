#ifndef __HDLR_EXIT_TO_GUEST
#define __HDLR_EXIT_TO_GUEST

int exit_to_guest_init(EventHandler *handler);
int exit_to_guest_handler(EventHandler *handler, Event *event);
int exit_to_guest_finalize(EventHandler *handler);
void exit_to_guest_reset(void);

#endif
