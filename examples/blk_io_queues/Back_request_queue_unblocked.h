#ifndef __HDLR_BACK_REQUEST_QUEUE_UNBLOCKED
#define __HDLR_BACK_REQUEST_QUEUE_UNBLOCKED

int back_request_queue_unblocked_init(EventHandler *handler);
int back_request_queue_unblocked_handler(EventHandler *handler, Event *event);
int back_request_queue_unblocked_finalize(EventHandler *handler);

void back_request_queue_unblocked_reset(void);
unsigned long long get_last_back_rqunblock_ns();

#endif
