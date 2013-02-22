#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int NUM_ANALYSES = 6;

typedef enum
{
	ALL_OPTIONS,
	CPU_UTILIZATION,
	CPU_WAIT,
	XEN_DOM_TIME,
	DISK_IO,
	XEN_STATS,
	INVALID
} Options;

char *analyses[NUM_ANALYSES] = {
	"\0",
	"../test/Cpu_util.out",
	"../test/Cpu_wait.out",
	"../test/Xen_dom_time.out",
	"../test/blk_queue_io",
	"../test/Xen_stats.out"
};

void display_usage(void)
{
	printf("Usage: $ ./xa [option] [analysis-name]\n"
		"\t-b\t: Only run analysis..\n"
		"\t\t\tall        = Run all analyses\n"
		"\t\t\tcpu-util   = Cpu utilization\n"
		"\t\t\tsched-lat  = Cpu scheduling latency\n"
		"\t\t\txen-time   = Time spent inside xen\n"
		"\t\t\tdisk-queue = Disk i/o queue states and wait times\n"
		"\t\t\tstats      = Xen event stats\n\n");
}


Options handle_options(int argc, char **argv)
{
	if(argc < 3) 
	{
		return INVALID;	
	}

	else if(!strcmp(argv[1], "-b"))
	{
		if(!strcmp(argv[2], "all"))
			return ALL_OPTIONS;
		else if(!strcmp(argv[2], "cpu-util"))
			return CPU_UTILIZATION;
		else if(!strcmp(argv[2], "sched-lat"))
			return CPU_WAIT;
		else if(!strcmp(argv[2], "xen-time"))
			return XEN_DOM_TIME;
		else if(!strcmp(argv[2], "disk-queue"))
			return DISK_IO;
		else if(!strcmp(argv[2], "stats"))
			return XEN_STATS;
		else
		{
			return INVALID;
		}
	}
	
	return INVALID;
}

// E.x: sudo xentrace -S 120 -D -e 0x0040f000 xentrace-5.out
void construct_xentrace_argv(char *argv[10], Options opt)
{
	argv[0] = "xentrace";
	argv[1] = "-S";
	argv[2] = "120";
	argv[3] = "-D";
	argv[4] = "-e";

	switch(opt)
	{
		case DISK_IO		: argv[5] = "0x0040f000";
					  break;
		case ALL_OPTIONS 	:
		case CPU_UTILIZATION 	: 
		case CPU_WAIT		: 
		case XEN_DOM_TIME	: 
		case XEN_STATS		: 
		default			: argv[5] = "all";
					  break;
	}

	argv[6] = "xentrace.out";
	argv[7] = NULL;
}


int main(int argc, char *argv[])
{
	Options opt = handle_options(argc, argv);

	if(opt == INVALID)
	{
		display_usage();
		exit(0);
	}

	// Run Xentrace 
	char *xentrace_argv[10];
		
	construct_xentrace_argv(xentrace_argv, opt);

	int xentrace_pid = fork();

	switch(xentrace_pid)
	{
		case -1	: perror("Couldn't fork xentrace");
			  exit(0);
		case 0	: // Child 
			  execvp("xentrace", xentrace_argv);
			  fprintf(stderr, "Coudln't exec xentrace\n");
			  exit(0);
		default	: // Parent
			  // Add signal handlers for killing xentrace.

	}
	return 0;
}
