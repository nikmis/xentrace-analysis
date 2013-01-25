#ifndef __HDLR_BACK_REQUEST_INTERRUPT_RECEIVE
#define __HDLR_BACK_REQUEST_INTERRUPT_RECEIVE

int back_request_interrupt_receive_init(EventHandler *handler);
int back_request_interrupt_receive_handler(EventHandler *handler, Event *event);
int back_request_interrupt_receive_finalize(EventHandler *handler);

void back_request_interrupt_receive_reset(void);

#endif
