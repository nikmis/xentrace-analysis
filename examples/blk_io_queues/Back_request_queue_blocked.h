#ifndef __HDLR_BACK_REQUEST_QUEUE_BLOCKED
#define __HDLR_BACK_REQUEST_QUEUE_BLOCKED

int back_request_queue_blocked_init(EventHandler *handler);
int back_request_queue_blocked_handler(EventHandler *handler, Event *event);
int back_request_queue_blocked_finalize(EventHandler *handler);

#endif
