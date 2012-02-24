#ifndef __EVENTHANDLER
#define __EVENTHANDLER

#include "list.h"
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

#endif

