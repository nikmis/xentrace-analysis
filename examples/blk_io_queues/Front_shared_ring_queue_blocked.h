#ifndef __HDLR_FRONT_SHARED_RING_QUEUE_BLOCKED
#define __HDLR_FRONT_SHARED_RING_QUEUE_BLOCKED

typedef struct SRBlockData
{
	unsigned int domId;
	unsigned long long lastSRBlockNs;
} SRBlockData;

int front_shared_ring_queue_blocked_init(EventHandler *handler);
int front_shared_ring_queue_blocked_handler(EventHandler *handler, Event *event);
int front_shared_ring_queue_blocked_finalize(EventHandler *handler);

void front_shared_ring_queue_blocked_reset(void);

unsigned long long get_last_srblock_ns();

#endif
