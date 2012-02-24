#include <stdio.h>

#include "Reader.h"
#include "EventHandler.h"

void evh_call_handlers(Reader *reader, Event *ev)
{
	int i = 0, index = -1;
	EventHandler	*ev_handler;
	list_head 	*head;

	for(i = 0; i < MAX_EVENTS; i++)
	{
		if(reader->handler_array[i].event_id == ev->event_id)
		{
			index = i;
			head = &(reader->handler_array[index].handler_list);
			break;
		}
	}

	if(index == -1)
	{
		fprintf(stderr, "%s:%d : Event %d not found\n",
				__FILE__,
				__LINE__,
				ev->event_id);
		return;
	}

	if(list_empty(head))
	{
#ifdef DEBUG
		fprintf(stderr, "%s:%d : Event %d doesn't have any handlers\n",
				__FILE__,
				__LINE__,
				ev->event_id);
#endif
		return;
	}

	list_for_each_entry_safe(ev_handler, head, handler_list)
	{
		ev_handler->process_event(ev_handler, ev);
	}

	return;
}

void evh_call_initializers(Reader *reader)
{
	int i = 0, index = -1;
	EventHandler	*ev_handler;
	list_head 	*head;

	for(i = 0; i < MAX_EVENTS; i++)
	{
		if(reader->handler_array[i].event_id != NULL)
		{
			index = i;
			head = &(reader->handler_array[index].handler_list);
			if(list_empty(head))
			{
#ifdef DEBUG
				fprintf(stderr, "%s:%d : Event %d doesn't have any handlers\n",
						__FILE__,
						__LINE__,
						ev->event_id);
#endif
				continue;
			}

			list_for_each_entry_safe(ev_handler, head, handler_list)
			{
				ev_handler->init(ev_handler, ev);
			}
		}
		else /* Null indicates no more handlers */
			return;
	}
}

void evh_call_finalizers(Reader *reader)
{
	int i = 0, index = -1;
	EventHandler	*ev_handler;
	list_head 	*head;

	for(i = 0; i < MAX_EVENTS; i++)
	{
		if(reader->handler_array[i].event_id != NULL)
		{
			index = i;
			head = &(reader->handler_array[index].handler_list);
			if(list_empty(head))
			{
#ifdef DEBUG
				fprintf(stderr, "%s:%d : Event %d doesn't have any handlers\n",
						__FILE__,
						__LINE__,
						ev->event_id);
#endif
				continue;
			}

			list_for_each_entry_safe(ev_handler, head, handler_list)
			{
				ev_handler->finalize(ev_handler, ev);
			}
		}
		else /* Null indicates no more handlers */
			return;
	}
}

void evh_register_handler(Reader *reader, EventHandler *ev_handler)
{
	int i = 0, index = -1;
	
	/* linear search thru event handlers to see if event registerd (list head exists).
	   if not reg,
	   	create new head
	   add to handler head.
	 */
	for(i = 0; i < MAX_EVENTS; i++)
	{
		/* ptr exists.
		   check for event. 
		 */
		if(reader->handler_array[i].event_id != NULL)
		{
			if(reader->handler_array[i].event_id == ev_handler->event_id)
			{
				/* event handler exists */
				index = i;
				break;
			}
			else 
			{
				continue;
			}
		}
		else /* evh doesnt exist */
		{
			/* init handler head */
			reader->handler_array[i].event_id = ev_handler->event_id;
			INIT_LIST_HEAD(&(reader->handler_array[i].handler_list));
			index = i;
			break;
		}
	}

	if(index == -1)
	{
		/* evh doesnt exist or didn't register */
		fprintf(stderr, "%s:%d: Couldn't register event handler for event %d\n",
				__FILE__,
				__LINE__,
				ev_handler->event_id);
		return;
	}

	list_add_tail(&ev_handler->handler_list, &(reader->handler_array[index].handler_list));
	return;
}
