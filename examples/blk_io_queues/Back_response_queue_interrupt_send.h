#ifndef __HDLR_BACK_RESPONSE_INTERRUPT_SEND
#define __HDLR_BACK_RESPONSE_INTERRUPT_SEND

int back_response_interrupt_send_init(EventHandler *handler);
int back_response_interrupt_send_handler(EventHandler *handler, Event *event);
int back_response_interrupt_send_finalize(EventHandler *handler);

void back_response_interrupt_send_reset(void);

#endif
