#ifndef __HDLR_FRONT_SHARED_RING_QUEUE_UNBLOCKED
#define __HDLR_FRONT_SHARED_RING_QUEUE_UNBLOCKED

int front_shared_ring_queue_unblocked_init(EventHandler *handler);
int front_shared_ring_queue_unblocked_handler(EventHandler *handler, Event *event);
int front_shared_ring_queue_unblocked_finalize(EventHandler *handler);

#endif
