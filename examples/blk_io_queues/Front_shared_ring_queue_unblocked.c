#include <stdio.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Lost_records.h"
#include "Front_shared_ring_queue_unblocked.h"
#include "Front_shared_ring_queue_blocked.h"

SRUnblockData srunblockData;

/* Avg data */
unsigned long long noLostWaitTime = 0;
unsigned long long lostWaitTime = 0;
unsigned long long n1 = 0;
unsigned long long n2 = 0;

FILE *histoFp;

typedef struct Histo
{
	unsigned type; /* 1 = lost wait; 2 = no lost */
	unsigned long long waitTime;
} Histo;


int front_shared_ring_queue_unblocked_init(EventHandler *handler)
{
	memset(&srunblockData, 0, sizeof(SRUnblockData));

	if((histoFp = fopen("/study/Dropbox/xen/cpu-share/xentrace-analysis/test/out.dat", "w+")) == NULL)
	{
		fprintf(stderr, "Error creating new histogram data file\n");
	}

	return 0;
}

int front_shared_ring_queue_unblocked_handler(EventHandler *handler, Event *event)
{
	unsigned long long lastSRBlockNs = get_last_srblock_ns();
	unsigned long long lastLostRecordNs = get_last_lost_records_ns(event->cpu);

	Histo lostHisto, noLostHisto;
	
	lostHisto.type = 1; noLostHisto.type = 2;
	lostHisto.waitTime = noLostHisto.waitTime = 0;

	/* Ignore successive unblock msgs.
	 * Can also appear successive due to lost records.
	 *
	 * SCENARIOS
	 * 	1		2		3		4
	 * last_block	| last_unblock	| last_block	| lost_rec
	 * 	.	|	.	|	.	|	.
	 * 	.	|	.	|	.	|	.
	 * lost_rec	| last_block	| last_unblock	| last_block
	 * 	.	|	. 	|	.	|	.
	 * 	.	|	.	|	.	|	.
	 * last_unblock	| lost_rec	| lost_rec	| last_unblock
	 * 	.	|	.	|	.	|	.
	 * unblock	| unblock	| unblock	| unblock
	 */
	if(	(lastLostRecordNs > lastSRBlockNs) || /* Scenario 1, 2, 3 */
		(srunblockData.lastSRUnblockNs > lastSRBlockNs)) /* Scenario 1, 3, 4 */
	{
		/* Incase you have a lost_rec betn a unblock & last_block/last_unblock events.
		 * Scenario 2, 3
		 */
		if(lastLostRecordNs > srunblockData.lastSRUnblockNs)
		{
			/*
			fprintf(stderr, "lost_rec > unblock ns:%llu diff:%llu lost:%llu bns:%llu uns%llu\n", 
					event->ns, 
					event->ns - lastLostRecordNs,
					lastLostRecordNs,
					lastSRBlockNs,
					srunblockData.lastSRUnblockNs);
					*/

			/* Since we don't know when exactly a block event occured last due to lost_rec,
			 * we add the time between lost_rec and the 1st unblock event after.
			 */
			unsigned long long diff = event->ns - lastLostRecordNs;
			srunblockData.sharedRingWaitTime += event->ns - lastLostRecordNs;
			srunblockData.lastSRUnblockNs = event->ns;

			lostWaitTime += event->ns - lastLostRecordNs;
			n1++;

			if(diff >= 2*1E8)
				fprintf(stderr, "lost_rec : ns = %llu,  wait_time = %llu\n", event->ns, event->ns - lastLostRecordNs);
			lostHisto.waitTime = event->ns - lastLostRecordNs;
			fwrite(&lostHisto, sizeof(Histo), 1, histoFp);
		}
		/* Scenario 1, 4 */
		return 0;
	}

	//fprintf(stderr, "wait time %15.5f ns %llu\n", (float)(event->ns - lastSRBlockNs)/MEGA, event->ns);

	if(lastSRBlockNs) /* Edge case: beginning of log, no block msg seen */
	{
		srunblockData.sharedRingWaitTime += event->ns - lastSRBlockNs;
		noLostWaitTime += event->ns - lastSRBlockNs;
		n2++;

		unsigned long long diff = event->ns - lastSRBlockNs;
		if(diff >= 2*1E8)
			fprintf(stderr, "normal : ns = %llu,  wait_time = %llu\n", event->ns, event->ns - lastSRBlockNs);

		noLostHisto.waitTime = event->ns - lastSRBlockNs;
		fwrite(&noLostHisto, sizeof(Histo), 1, histoFp);
	}

	srunblockData.lastSRUnblockNs = event->ns;

	return 0;
}

int front_shared_ring_queue_unblocked_finalize(EventHandler *handler)
{
	printf("Front Shared Ring Queue was blocked for %15.3f (ms)\n"
		"Avg wait time with lost_rec = %15.3f (ms), Total = %15.3f (ms), num = %llu\n" 
		"Avg wait time w/o lost_rec = %15.3f (ms), Total = %15.3f (ms), num = %llu\n\n", 
			(float)srunblockData.sharedRingWaitTime/MEGA,
			((float)lostWaitTime/(n1*MEGA)), (float)lostWaitTime/MEGA, n1,
			((float)noLostWaitTime/(n2*MEGA)), (float)noLostWaitTime/MEGA, n2);

	fclose(histoFp);

	return 0;
}

void front_shared_ring_queue_unblocked_reset(void)
{
	memset(&srunblockData, 0, sizeof(SRUnblockData));
}

unsigned long long get_last_srunblock_ns()
{
	return srunblockData.lastSRUnblockNs;
}

struct EventHandler frontSharedRingQueueUnblockedHandler = 
{
	.name = "front_shared_ring_queue_unblocked",
	.event_id = TRC_BLKFRONT_SHARED_RING_QUEUE_UNBLOCKED,
	.init = front_shared_ring_queue_unblocked_init,
	.data = (void *)NULL,
	.process_event = front_shared_ring_queue_unblocked_handler,
	.finalize = front_shared_ring_queue_unblocked_finalize,
};
