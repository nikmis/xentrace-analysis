#ifndef __HDLR_FRONT_REQUEST_QUEUE_UNBLOCKED
#define __HDLR_FRONT_REQUEST_QUEUE_UNBLOCKED

int front_request_queue_unblocked_init(EventHandler *handler);
int front_request_queue_unblocked_handler(EventHandler *handler, Event *event);
int front_request_queue_unblocked_finalize(EventHandler *handler);

void front_request_queue_unblocked_reset(void);
unsigned long long get_last_rqunblock_ns();

#endif
