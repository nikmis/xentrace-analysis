#ifndef __HDLR_BACK_REQUEST_QUEUE_BLOCKED
#define __HDLR_BACK_REQUEST_QUEUE_BLOCKED

typedef struct BackRQBlockData
{
	unsigned long long lastRQBlockNs;
	unsigned long long backRQBlockWaitTime;
} BackRQBlockData;

int back_request_queue_blocked_init(EventHandler *handler);
int back_request_queue_blocked_handler(EventHandler *handler, Event *event);
int back_request_queue_blocked_finalize(EventHandler *handler);

void back_request_queue_blocked_reset(void);
unsigned long long get_last_back_rqblock_ns();

#endif
