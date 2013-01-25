/******************************************************************************
 * include/public/trace.h
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Mark Williamson, (C) 2004 Intel Research Cambridge
 * Copyright (C) 2005 Bin Ren
 */

#ifndef __XEN_PUBLIC_TRACE_H__
#define __XEN_PUBLIC_TRACE_H__

/* Trace classes */
#define TRC_CLS_SHIFT 16
#define TRC_GEN     0x0001f000    /* General trace            */
#define TRC_SCHED   0x0002f000    /* Xen Scheduler trace      */
#define TRC_DOM0OP  0x0004f000    /* Xen DOM0 operation trace */
#define TRC_HVM     0x0008f000    /* Xen HVM trace            */
#define TRC_MEM     0x0010f000    /* Xen memory trace         */
#define TRC_XA_XEN  0x0020f000    /* Low-level transitions between guests and Xen */
#define TRC_XA_BLK  0x0040f000    /* Events from the block devices  */
#define TRC_ALL     0xfffff000

/* Trace subclasses */
#define TRC_SUBCLS_SHIFT 12

/* trace subclasses for SVM */
#define TRC_HVM_ENTRYEXIT 0x00081000   /* VMENTRY and #VMEXIT       */
#define TRC_HVM_HANDLER   0x00082000   /* various HVM handlers      */

/* Trace events per class */
#define TRC_LOST_RECORDS        (TRC_GEN + 1)

#define TRC_SCHED_DOM_ADD       (TRC_SCHED +  1)
#define TRC_SCHED_DOM_REM       (TRC_SCHED +  2)
#define TRC_SCHED_SLEEP         (TRC_SCHED +  3)
#define TRC_SCHED_WAKE          (TRC_SCHED +  4)
#define TRC_SCHED_YIELD         (TRC_SCHED +  5)
#define TRC_SCHED_BLOCK         (TRC_SCHED +  6)
#define TRC_SCHED_SHUTDOWN      (TRC_SCHED +  7)
#define TRC_SCHED_CTL           (TRC_SCHED +  8)
#define TRC_SCHED_ADJDOM        (TRC_SCHED +  9)
#define TRC_SCHED_SWITCH        (TRC_SCHED + 10)
#define TRC_SCHED_S_TIMER_FN    (TRC_SCHED + 11)
#define TRC_SCHED_T_TIMER_FN    (TRC_SCHED + 12)
#define TRC_SCHED_DOM_TIMER_FN  (TRC_SCHED + 13)
#define TRC_SCHED_SWITCH_INFPREV (TRC_SCHED + 14)
#define TRC_SCHED_SWITCH_INFNEXT (TRC_SCHED + 15)

#define TRC_EVENT                                (TRC_XA_XEN + 1)
#define TRC_EXIT_TO_GUEST                        (TRC_XA_XEN + 2)
#define TRC_EXIT_TO_XEN                          (TRC_XA_XEN + 3)
#define TRC_EXCEPTION                            (TRC_XA_XEN + 4)
#define TRC_EXCEPTION_IN_XEN                     (TRC_XA_XEN + 5)
#define TRC_INTERRUPT                            (TRC_XA_XEN + 6)
#define TRC_INTERRUPT_IN_XEN                     (TRC_XA_XEN + 7)
#define TRC_HYPERCALL                            (TRC_XA_XEN + 8)
#define TRC_EXIT_TO_XEN_NO_VCPU                  (TRC_XA_XEN + 9)
#define TRC_IRQ                                  (TRC_XA_XEN + 10)

/*
#define TRC_EVENT				(TRC_SCHED + 101)
#define TRC_EXIT_TO_GUEST			(TRC_SCHED + 102)
#define TRC_EXIT_TO_XEN				(TRC_SCHED + 103)
#define TRC_EXCEPTION				(TRC_SCHED + 104)
#define TRC_EXCEPTION_IN_XEN			(TRC_SCHED + 105)
#define TRC_INTERRUPT				(TRC_SCHED + 106)
#define TRC_INTERRUPT_IN_XEN			(TRC_SCHED + 107)
#define TRC_HYPERCALL				(TRC_SCHED + 108) 
*/
/* Netback queues: inflight, and shared-ring */

// Queue of in-fligt requests, blocked if allocation of a new request
// fails

#define TRC_BLKBACK_INFLIGHT_QUEUE_BLOCKED        (TRC_XA_BLK + 1)
#define TRC_BLKBACK_INFLIGHT_QUEUE_UNBLOCKED      (TRC_XA_BLK + 2)

// Shared ring queue of incoming requests, blocked when there are no 
// new requests from the frontend
#define TRC_BLKBACK_REQUEST_QUEUE_BLOCKED         (TRC_XA_BLK + 3)
#define TRC_BLKBACK_REQUEST_QUEUE_UNBLOCKED       (TRC_XA_BLK + 4)

// Allocation of the grant table entries may fail, and then this 
// queue gets blocked
#define TRC_BLKFRONT_GRANT_QUEUE_BLOCKED       (TRC_XA_BLK + 5)
#define TRC_BLKFRONT_GRANT_QUEUE_UNBLOCKED     (TRC_XA_BLK + 6)

// Shared ring queue, blocked when there is no space left in the ring
#define TRC_BLKFRONT_SHARED_RING_QUEUE_BLOCKED    (TRC_XA_BLK + 7)
#define TRC_BLKFRONT_SHARED_RING_QUEUE_UNBLOCKED  (TRC_XA_BLK + 8)

// Requests from the elevator sheduler, the queue gets blocked when 
// no new requests are there
#define TRC_BLKFRONT_REQUEST_QUEUE_BLOCKED        (TRC_XA_BLK + 9)
#define TRC_BLKFRONT_REQUEST_QUEUE_UNBLOCKED      (TRC_XA_BLK + 10)

// Shared ring response queue, blocked when there are no responses in the ring
#define TRC_BLKFRONT_SHARED_RING_RESP_QUEUE_BLOCKED    (TRC_XA_BLK + 11)
#define TRC_BLKFRONT_SHARED_RING_RESP_QUEUE_UNBLOCKED  (TRC_XA_BLK + 12)

// Notify backend with an interrupt
#define TRC_BLKBACK_INT                          (TRC_XA_BLK + 13)
#define TRC_BLKBACK_INT_SEND                     (TRC_XA_BLK + 14)
//
// Notify frontend with an interrupt
#define TRC_BLKFRONT_INT                         (TRC_XA_BLK + 15)
#define TRC_BLKFRONT_INT_SEND                    (TRC_XA_BLK + 16)


#ifdef CONFIG_TIMETRAVEL
#define TRC_SCHED_START_TIME_DILATE    (TRC_SCHED + 32)
#define TRC_SCHED_STOP_TIME_DILATE     (TRC_SCHED + 33)
#define TRC_SCHED_TIME_DILATE          (TRC_SCHED + 34)
#define TRC_SCHED_CONTEXT_SWITCH       (TRC_SCHED + 35)
#define TRC_SCHED_SEND_TIMER_EVENT     (TRC_SCHED + 36)
#define TRC_SCHED_SINGLESHOT_TIMER_EVENT (TRC_SCHED + 37)
#define TRC_SCHED_PERIODIC_TIMER_EVENT (TRC_SCHED + 38)
#define TRC_SCHED_SET_PERIODIC_TIMER (TRC_SCHED + 39)
#define TRC_SCHED_DOMAIN_RUNNING                (TRC_SCHED + 40)
#define TRC_SCHED_PERIODIC_TIMER_TOO_EARLY      (TRC_SCHED + 41)
#define TRC_SCHED_PERIODIC_TIMER_TOO_EARLY_2    (TRC_SCHED + 42)
#define TRC_SCHED_PERIODIC_TIMER_EVENT_COUNT    (TRC_SCHED + 43)
#define TRC_SCHED_SINGLESHOT_TIMER_TOO_EARLY    (TRC_SCHED + 44)

#define TRC_SCHED_DOMU_YIELD                    (TRC_SCHED + 45)
#define TRC_SCHED_TIME_CALIBRATION              (TRC_SCHED + 46)

#define TRC_SCHED_DOMU_NETFRONT_INT 		(TRC_SCHED + 47)
#define TRC_SCHED_DOMU_NETFRONT_SCHED_POLL 	(TRC_SCHED + 48)

#define TRC_SCHED_DOMU_NETFRONT_TRACE_PCKT 	(TRC_SCHED + 49)
#define TRC_SCHED_DOMU_NETBACK_TRACE_PCKT 	(TRC_SCHED + 50)
#define TRC_SCHED_DOMU_NETBACK_QUEUE_FULL 	(TRC_SCHED + 51)
#define TRC_SCHED_DOMU_NET_ENTER_LOSS_2		(TRC_SCHED + 52)
#define TRC_SCHED_DOMU_TCP_RETRANSMIT_SKB_2 	(TRC_SCHED + 53)
#define TRC_SCHED_TIME_DILATE_LEAVE		(TRC_SCHED + 54)
#define TRC_SCHED_DOMU_SET_SINGLESHOT_TIMER	(TRC_SCHED + 55)
#define TRC_SCHED_DOMU_TCP_RETRANSMIT_SKB_1 	(TRC_SCHED + 56)
#define TRC_SCHED_DOMU_TCP_RETRANSMIT_SKB 	(TRC_SCHED + 57)

#define TRC_SCHED_DOMU_PROCESS_SWITCH 		(TRC_SCHED + 58)
#define TRC_SCHED_DOMU_PROCESS_SWITCH_IDLE	(TRC_SCHED + 59)
#define TRC_SCHED_DOMU_SOFT_IRQ			(TRC_SCHED + 60)
#define TRC_SCHED_DOMU_IRQ			(TRC_SCHED + 61)
#define TRC_SCHED_DOMU_DEVICE_UPCALL		(TRC_SCHED + 62)
#define TRC_SCHED_DOMU_PROCESS_SWITCH_RESCHED	(TRC_SCHED + 63)
#define TRC_SCHED_DOMU_PROCESS_SWITCH_NEXT	(TRC_SCHED + 64)
#define TRC_SCHED_DOMU_PROCESS_SWITCH_FREEZE	(TRC_SCHED + 65)
#define TRC_SCHED_DOMU_PROCESS_SWITCH_UNFREEZE	(TRC_SCHED + 66)
#define TRC_SCHED_DOMU_PROCESS_SWITCH_DEACTIVATE (TRC_SCHED + 67)
#define TRC_SCHED_STOP_TIME_DILATE_SCHEDULED    (TRC_SCHED + 68)
#define TRC_SCHED_DOMU_NO_NEED_TO_RESCHEDULE    (TRC_SCHED + 69)
#define TRC_SCHED_DOMU_NETFRONT_TRACE_SEND_PCKT (TRC_SCHED + 70)
#define TRC_SCHED_DOMU_NETBACK_TRACE_SEND_PCKT  (TRC_SCHED + 71)
#define TRC_SCHED_DOMU_NETBACK_STOP_QUEUE	(TRC_SCHED + 72)
#define TRC_SCHED_DOMU_NETBACK_RECV		(TRC_SCHED + 73)
#define TRC_SCHED_DOMU_DEV_HARD_XMIT		(TRC_SCHED + 74)
#define TRC_SCHED_DOMU_DEV_QUEUE_XMIT		(TRC_SCHED + 75)
#define TRC_SCHED_DOMU_DEV_QUEUE_XMIT_QUEUE_FAILED (TRC_SCHED + 76)
#define TRC_SCHED_DOMU_DEV_QUEUE_XMIT_SUCCESS	(TRC_SCHED + 77)
#define TRC_SCHED_DOMU_DEV_QUEUE_XMIT_FAILED	(TRC_SCHED + 78)
#define TRC_SCHED_DOMU_NETFRONT_SUSPENDED	(TRC_SCHED + 81)
#define TRC_SCHED_DOMU_NETBACK_SUSPENDED	(TRC_SCHED + 82)
#define TRC_SCHED_DOMU_NETBACK_QUEUE_DURING_REPLAY (TRC_SCHED + 83)
#define TRC_SCHED_DOMU_NETBACK_START_SMOOTH_REPLAY (TRC_SCHED + 84)
#define TRC_SCHED_DOMU_NETBACK_STOP_SMOOTH_REPLAY (TRC_SCHED + 85)
#define TRC_SCHED_DOMU_NETBACK_SMOOTH_REPLAY_PKT (TRC_SCHED + 86)

#endif /* CONFIG_TIMETRAVEL */

#define TRC_MEM_PAGE_GRANT_MAP      (TRC_MEM + 1)
#define TRC_MEM_PAGE_GRANT_UNMAP    (TRC_MEM + 2)
#define TRC_MEM_PAGE_GRANT_TRANSFER (TRC_MEM + 3)

/* trace events per subclass */
#define TRC_HVM_VMENTRY         (TRC_HVM_ENTRYEXIT + 0x01)
#define TRC_HVM_VMEXIT          (TRC_HVM_ENTRYEXIT + 0x02)
#define TRC_HVM_PF_XEN          (TRC_HVM_HANDLER + 0x01)
#define TRC_HVM_PF_INJECT       (TRC_HVM_HANDLER + 0x02)
#define TRC_HVM_INJ_EXC         (TRC_HVM_HANDLER + 0x03)
#define TRC_HVM_INJ_VIRQ        (TRC_HVM_HANDLER + 0x04)
#define TRC_HVM_REINJ_VIRQ      (TRC_HVM_HANDLER + 0x05)
#define TRC_HVM_IO_READ         (TRC_HVM_HANDLER + 0x06)
#define TRC_HVM_IO_WRITE        (TRC_HVM_HANDLER + 0x07)
#define TRC_HVM_CR_READ         (TRC_HVM_HANDLER + 0x08)
#define TRC_HVM_CR_WRITE        (TRC_HVM_HANDLER + 0x09)
#define TRC_HVM_DR_READ         (TRC_HVM_HANDLER + 0x0A)
#define TRC_HVM_DR_WRITE        (TRC_HVM_HANDLER + 0x0B)
#define TRC_HVM_MSR_READ        (TRC_HVM_HANDLER + 0x0C)
#define TRC_HVM_MSR_WRITE       (TRC_HVM_HANDLER + 0x0D)
#define TRC_HVM_CPUID           (TRC_HVM_HANDLER + 0x0E)
#define TRC_HVM_INTR            (TRC_HVM_HANDLER + 0x0F)
#define TRC_HVM_NMI             (TRC_HVM_HANDLER + 0x10)
#define TRC_HVM_SMI             (TRC_HVM_HANDLER + 0x11)
#define TRC_HVM_VMMCALL         (TRC_HVM_HANDLER + 0x12)
#define TRC_HVM_HLT             (TRC_HVM_HANDLER + 0x13)
#define TRC_HVM_INVLPG          (TRC_HVM_HANDLER + 0x14)

///* This structure represents a single trace buffer record. */
//struct t_rec {
//    uint64_t cycles;          /* cycle counter timestamp */
//    uint32_t event;           /* event ID                */
//#ifdef CONFIG_TIMETRAVEL_XENTRACE
//    unsigned long long data[5];    /* event data items        */
//#else 
//    unsigned long data[5];    /* event data items        */
//#endif 
//};
//
///*
// * This structure contains the metadata for a single trace buffer.  The head
// * field, indexes into an array of struct t_rec's.
// */
//struct t_buf {
//    uint32_t cons;      /* Next item to be consumed by control tools. */
//    uint32_t prod;      /* Next item to be produced by Xen.           */
//    /* 'nr_recs' records follow immediately after the meta-data header.    */
//};

#endif /* __XEN_PUBLIC_TRACE_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
