#ifndef __HDLR_FRONT_SHARED_RING_QUEUE_UNBLOCKED
#define __HDLR_FRONT_SHARED_RING_QUEUE_UNBLOCKED

typedef struct SRUnblockData
{
	unsigned int domId;
	unsigned long long lastSRUnblockNs;
	unsigned long long sharedRingWaitTime;
} SRUnblockData;

int front_shared_ring_queue_unblocked_init(EventHandler *handler);
int front_shared_ring_queue_unblocked_handler(EventHandler *handler, Event *event);
int front_shared_ring_queue_unblocked_finalize(EventHandler *handler);

void front_shared_ring_queue_unblocked_reset(void);
unsigned long long get_last_srunblock_ns();

#endif
