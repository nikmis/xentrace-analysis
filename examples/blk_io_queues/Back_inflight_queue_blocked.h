#ifndef __HDLR_BACK_INFLIGHT_QUEUE_BLOCKED
#define __HDLR_BACK_INFLIGHT_QUEUE_BLOCKED

int back_inflight_queue_blocked_init(EventHandler *handler);
int back_inflight_queue_blocked_handler(EventHandler *handler, Event *event);
int back_inflight_queue_blocked_finalize(EventHandler *handler);

#endif
