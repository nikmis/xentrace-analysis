#ifndef __HDLR_FRONT_REQUEST_QUEUE_BLOCKED
#define __HDLR_FRONT_REQUEST_QUEUE_BLOCKED

int front_request_queue_blocked_init(EventHandler *handler);
int front_request_queue_blocked_handler(EventHandler *handler, Event *event);
int front_request_queue_blocked_finalize(EventHandler *handler);

#endif
