#ifndef __HDLR_BACK_INFLIGHT_QUEUE_UNBLOCKED
#define __HDLR_BACK_INFLIGHT_QUEUE_UNBLOCKED

int back_inflight_queue_unblocked_init(EventHandler *handler);
int back_inflight_queue_unblocked_handler(EventHandler *handler, Event *event);
int back_inflight_queue_unblocked_finalize(EventHandler *handler);

#endif
