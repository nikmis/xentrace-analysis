#ifndef __HDLR_LOST_RECORDS
#define __HDLR_LOST_RECORDS


int lost_records_init(EventHandler *handler);
int lost_records_handler(EventHandler *handler, Event *event);
int lost_records_finalize(EventHandler *handler);

#endif
