#ifndef __HDLR_FRONT_SHARED_RESP_RING_QUEUE_BLOCKED
#define __HDLR_FRONT_SHARED_RESP_RING_QUEUE_BLOCKED

int front_shared_ring_resp_queue_blocked_init(EventHandler *handler);
int front_shared_ring_resp_queue_blocked_handler(EventHandler *handler, Event *event);
int front_shared_ring_resp_queue_blocked_finalize(EventHandler *handler);

void front_shared_ring_resp_queue_blocked_reset(void);

#endif
