#ifndef __HDLR_FRONT_GRANT_QUEUE_UNBLOCKED
#define __HDLR_FRONT_GRANT_QUEUE_UNBLOCKED

int front_grant_queue_unblocked_init(EventHandler *handler);
int front_grant_queue_unblocked_handler(EventHandler *handler, Event *event);
int front_grant_queue_unblocked_finalize(EventHandler *handler);

void front_grant_queue_unblocked_reset(void);

#endif
