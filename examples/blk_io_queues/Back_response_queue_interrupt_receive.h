#ifndef __HDLR_BACK_RESPONSE_INTERRUPT_RECEIVE
#define __HDLR_BACK_RESPONSE_INTERRUPT_RECEIVE

int back_response_interrupt_receive_init(EventHandler *handler);
int back_response_interrupt_receive_handler(EventHandler *handler, Event *event);
int back_response_interrupt_receive_finalize(EventHandler *handler);

void back_response_interrupt_receive_reset(void);

#endif
