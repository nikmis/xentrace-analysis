#include <stdio.h>
#include "Event.h"
#include "Macros.h"
#include "Switch_infprev.h"

/* Description:	Event specfic parse function
   Parses SWITCH_INFPREV events and prints the cpu time-sharing stats 

   To-Do: 
   - Use Linked list instead of fixed len array for DomTime. 
 */
void generate_cpu_share_stats(FILE *fp, unsigned int ev_id)
{
	unsigned long long	total_time = 0;
	unsigned short		num_of_doms = 0;
	unsigned short		i = 0;
	Event ev;
	DomTime dt[MAX_DOMS];

	/* Init Struct */
	clear_event(&ev);

	while(!feof(fp))
	{
		if(parse_event(&ev, fp, SWITCH_INFPREV) != FAIL)
		{
			num_of_doms = add_time_to_list(dt, &ev);
			total_time += ev.data[1];
		}
	} /* EOF reached. */
	
	while((i < num_of_doms) && (i < MAX_DOMS))
	{
		printf("Dom ID = %d Cpu_Time = %lld Total_Time = %lld Cpu_Time_Share = %.2f %%\n", 
				dt[i].dom_id, 
				dt[i].runtime, 
				total_time, 
				((float)dt[i].runtime/total_time)*100);
		++i;
	}
}

/* Description:	Helper function for generate_cpu_share_stats()
   Searches for existing domain id data.
   if dom exists
   	update runtime
   else
   	add new dom and its runtime
 */
unsigned short add_time_to_list(DomTime *dt, Event *ev)
{
	static unsigned short num = 0;
	unsigned short i = 0;

	/* Search if domain already exists. 

	   Hash Table is the ideal abstraction for
	   looking up dom ids. Instead I am using
	   Linear Search on the list/array, since list 
	   size will be small and HT will be overkill.
	 */
	while(i < num)
	{
		if(dt[i].dom_id == ev->data[0])
		{
			dt[i].runtime += ev->data[1];

			/* No new doms added */
			return num;
		}
		
		++i;
	}

	if (num == MAX_DOMS)
	{
		/* No new doms can be added */
		return num;
	}

	/* If not found add new dom and its time.
	   Update num
	 */
	dt[i].dom_id = ev->data[0];
	dt[i].runtime = ev->data[1];

	num = i + 1; /* +1 since array index starts at 0 */

	return num;
}
		
