#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Event.h"
#include "EventHandler.h"
#include "Macros.h"
#include "Trace.h"
#include "Common.h"

int num_ev_init(NumEvs *dat)
{
	memset(dat, 0, sizeof(NumEvs)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(dat[i].numTypes.typeList));
	}
	
	return SUCCESS;
}

int num_ev_in_xen_init(NumEvs *dat)
{
	memset(dat, 0, sizeof(NumEvs));

	INIT_LIST_HEAD(&(dat->numTypes.typeList));
	
	return SUCCESS;
}

int num_ev_handler(NumEvs *dat, Event *event)	
{
	unsigned int domId = (event->data[0] == 0x7fff) ? MAX_DOMS - 1: event->data[0];
	unsigned int type = event->data[2];
	
	list_head *head = &(dat[domId].numTypes.typeList);
	NumTypes  *tmpNumTypes;

	list_for_each_entry(tmpNumTypes, head, typeList)
	{
		if(tmpNumTypes->type == type)
		{
			tmpNumTypes->totalTypeCount++;
			dat[domId].totalEvsCount++;

			return SUCCESS;
		}
	}

	NumTypes *newNumTypes = (NumTypes *) malloc(sizeof(NumTypes));

	newNumTypes->type = type;
	newNumTypes->totalTypeCount = 1;	/* Init. first event for given type */

	dat[domId].totalEvsCount++;

	list_add_tail(&(newNumTypes->typeList), head);

	return SUCCESS;
}

int num_ev_in_xen_handler(NumEvs *dat, Event *event)	
{
	unsigned int type = event->data[0];
	
	list_head *head = &(dat->numTypes.typeList);
	NumTypes  *tmpNumTypes;

	list_for_each_entry(tmpNumTypes, head, typeList)
	{
		if(tmpNumTypes->type == type)
		{
			tmpNumTypes->totalTypeCount++;
			dat->totalEvsCount++;

			return SUCCESS;
		}
	}

	NumTypes *newNumTypes = (NumTypes *) malloc(sizeof(NumTypes));

	newNumTypes->type = type;
	newNumTypes->totalTypeCount = 1;	/* Init. first event for given type */

	dat->totalEvsCount++;

	list_add_tail(&(newNumTypes->typeList), head);

	return SUCCESS;
}

int num_ev_finalize(NumEvs *dat, char* evName, char* typeName)
{
	unsigned int flag = 0;

	NumTypes  *tmpNumTypes;

	printf("\n%s Stats\n", evName);
	int i;
	for(i = 0; i < MAX_DOMS; i++)
	{
		/* If no events received for domain then skip the domain */
		if(!dat[i].totalEvsCount)
		{
			continue;
		}

		flag = 1;
		unsigned int domId = (i == (MAX_DOMS - 1)) ? 0x7fff: i; 
		list_head *head = &(dat[i].numTypes.typeList);

		printf("  Domain %5u:\n\tTotal %s Count = %*llu\n", domId, evName, 13, dat[i].totalEvsCount); 

		list_for_each_entry(tmpNumTypes, head, typeList)
		{
			printf("\t\t%s %4u : Count = %7u\n", typeName, tmpNumTypes->type, tmpNumTypes->totalTypeCount);
		}
	}

	if(!flag)	printf("\tNo %ss found\n", evName);

	for(i = 0; i < MAX_DOMS; i++)
	{
		/* Free memory */
		list_head *head = &(dat[i].numTypes.typeList);

		list_for_each_entry_reverse(tmpNumTypes, head, typeList)
		{
			free(tmpNumTypes);
		}
	}


	return 0;
}

int num_ev_in_xen_finalize(NumEvs *dat, char* evName, char* typeName)
{
	NumTypes  *tmpNumTypes;

	printf("\n%s in Xen Stats\n", evName);

	/* If no events received for domain then skip the domain */
	if(!dat->totalEvsCount)
	{
		printf("\tNo %ss in Xen\n", evName);
		return SUCCESS;
	}

	list_head *head = &(dat->numTypes.typeList);

	printf("\tTotal %s in Xen Count = %13llu\n", evName, dat->totalEvsCount); 

	list_for_each_entry(tmpNumTypes, head, typeList)
	{
		printf("\t\t%s %4u : Count = %7u\n", typeName, tmpNumTypes->type, tmpNumTypes->totalTypeCount);
	}

	/* Free memory */
	list_for_each_entry_reverse(tmpNumTypes, head, typeList)
	{
		free(tmpNumTypes);
	}

	return 0;

}

void num_ev_reset(NumEvs *dat)
{
	NumTypes  *tmpNumTypes;

	memset(dat, 0, sizeof(NumEvs)*MAX_DOMS);

	int i;
	for(i = 0; i< MAX_DOMS; i++)
	{
		INIT_LIST_HEAD(&(dat[i].numTypes.typeList));

		/* Free memory */
		list_head *head = &(dat[i].numTypes.typeList);

		list_for_each_entry_reverse(tmpNumTypes, head, typeList)
		{
			free(tmpNumTypes);
		}
	}
}

void num_ev_in_xen_reset(NumEvs *dat)
{
	NumTypes  *tmpNumTypes;
	list_head *head = &(dat->numTypes.typeList);

	memset(dat, 0, sizeof(NumEvs));

	INIT_LIST_HEAD(&(dat->numTypes.typeList));

	/* Free memory */
	list_for_each_entry_reverse(tmpNumTypes, head, typeList)
	{
		free(tmpNumTypes);
	}
}
