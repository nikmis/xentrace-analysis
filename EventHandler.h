#ifndef __EVENTHANDLER
#define __EVENTHANDLER

#include "list.h"
#include "Reader.h"
#include "Event.h"

typedef struct EventHandler
{
	char		name[50];
	unsigned int 	event_id;
	void		*data;

	list_head	handler_list;

	int	(*init)(struct EventHandler *ev_handler, Event *ev);
	int	(*process_event)(struct EventHandler *ev_handler, Event *ev);
	int	(*finalize)(struct EventHandler *ev_handler, Event *ev);
} EventHandler;

void evh_call_handlers(Reader *reader, Event *ev);
void evh_call_initializers(Reader *reader);
void evh_call_finalizers(Reader *reader);
void evh_register_handler(Reader *reader, EventHandler *ev_handler);

#endif

