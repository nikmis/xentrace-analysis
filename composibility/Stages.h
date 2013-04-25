#include <stdio.h>
#include "Pipe.h"
#include "Event.h"

Event event_id(Stage *s, Event ev, void *data);
void free_event_id(Stage *s);

Event count(Stage *s, Event ev, void *data);
void free_count(Stage *s);

Event wait_time(Stage *s, Event ev, void *data);
Event wait_time_t(Stage *s, Event ev1, Event ev2);
void free_wait_time(Stage *s);
