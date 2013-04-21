#include <stdio.h>
#include "Pipe.h"
#include "Event.h"

Event event_id(Stage *s, Event ev, void *data);
void free_event_id(Stage *s);
Event count(Stage *s, Event ev, void *data);
void free_count(Stage *s);
