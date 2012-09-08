#ifndef __HDLR_FRONT_REQUEST_QUEUE_BLOCKED
#define __HDLR_FRONT_REQUEST_QUEUE_BLOCKED

typedef struct RQBlockData
{
	unsigned int domId;
	unsigned long long lastRQBlockNs;
	unsigned long long requestQueueWaitTime;
} RQBlockData;

int front_request_queue_blocked_init(EventHandler *handler);
int front_request_queue_blocked_handler(EventHandler *handler, Event *event);
int front_request_queue_blocked_finalize(EventHandler *handler);

void front_request_queue_blocked_reset(void);
unsigned long long get_last_rqblock_ns();

#endif
