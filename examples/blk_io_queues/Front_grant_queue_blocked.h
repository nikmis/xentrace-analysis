#ifndef __HDLR_FRONT_GRANT_QUEUE_BLOCKED
#define __HDLR_FRONT_GRANT_QUEUE_BLOCKED

int front_grant_queue_blocked_init(EventHandler *handler);
int front_grant_queue_blocked_handler(EventHandler *handler, Event *event);
int front_grant_queue_blocked_finalize(EventHandler *handler);

void front_grant_queue_blocked_reset(void);

#endif
