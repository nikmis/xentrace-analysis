#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/errno.h>

const int NUM_ANALYSES = 6;
char *xentraceFileName = "xentrace.out";
static volatile sig_atomic_t xentraceKillRcvd = 0;

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
	"\0",				// ALL_OPTIONS
	"../test/Cpu_util.out",		// CPU_UTILIZATION
	"../test/Cpu_wait.out",		// CPU_WAIT
	"../test/Xen_dom_time.out",	// XEN_DOM_TIME
	"../test/blk_queue_io",		// DISK_IO
	"../test/Xen_stats.out"		// XEN_STATS
};

static void sigint_handler(int sig)
{
	if(!xentraceKillRcvd)
		xentraceKillRcvd = 1;
}

void display_usage(void)
{
	printf("Usage: $ ./xa [option] [-f] [xentrace-file-name]\n"
		"\t-b\t: Run analysis [analysis-name]\n"
		"\t\t\tall        = Run all analyses\n"
		"\t\t\tcpu-util   = Cpu utilization\n"
		"\t\t\tsched-lat  = Cpu scheduling latency\n"
		"\t\t\txen-time   = Time spent inside xen\n"
		"\t\t\tdisk-queue = Disk i/o queue states and wait times\n"
		"\t\t\tstats      = Xen event stats\n\n"
		);
}


Options handle_options(int argc, char **argv)
{
	if(argc < 3) 
	{
		return INVALID;	
	}

	/*
	if(argc > 3)
	{
		if(strcmp(argv[3], "-f"))
		{
			argv[4] = "xentrace.out";
		}
	}
	*/
	if(!strcmp(argv[1], "-b"))
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

	argv[6] = xentraceFileName;
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
	char *xentraceArgv[10];
		
	construct_xentrace_argv(xentraceArgv, opt);
	
	// Setup signal handlers and block SIGINT until fork() & exec() are complete
	sigset_t origMask, blockMask;
	struct sigaction sa;

	sigemptyset(&blockMask);
	sigaddset(&blockMask, SIGINT);

	if(sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
	{
		perror("Couldn't set signal mask. Exiting.");
		exit(0);
	}

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sigint_handler;

	if(sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Couldn't register signal handler. Exiting.");
		exit(0);
	}

	
	int xentracePid = fork();

	switch(xentracePid)
	{
		case -1	: perror("Couldn't fork xentrace");
			  exit(0);
		case 0	: // Child 
			  // Pending signals are not inherited, so no need to handle them for exec().
			  execvp("xentrace", xentraceArgv);
			  perror("Coudln't exec xentrace.");
			  exit(0);
		default	: // Parent
			  break;
	}
	
	// Add signal handlers for killing xentrace.

	// Pause for signal. sigsuspend() atomically pasues for signal.
	// Handle any pending signals.
	if((sigsuspend(&origMask) == -1) && (errno != EINTR))
	{
		perror("Sigsuspend failed. Exiting.");
		// Kill xentrace process.
		kill(xentracePid, SIGINT);
		exit(0);
	}

	// Received SIGINT. Kill xentrace.
	if(kill(xentracePid, SIGINT) == -1)
	{
		perror("Couldn't kill xentrace.");
		fprintf(stderr, "Xentrace pid is %d\n", xentracePid);
		exit(0);
	}

	// Signal received and process un-paused. 
	// Unmask signals.
	if(sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
	{
		perror("Couldn't unmask signals. Exiting.");
		exit(0);
	}

	// Run analysis.
	int analysisPid = fork();

	switch(analysisPid)
	{
		case -1	: perror("Couldn't fork analysis");
			  exit(0);
		case 0	: // Child
			  // exec() analysis 

			  if(opt == ALL_OPTIONS)
			  {
				  // exec() all analysis.
			  }
			  else
			  {
				  // extrace filename from path.
				  char *filename = strrchr(analyses[opt], '/');
				  filename++;  

				  execl(analyses[opt], filename, xentraceFileName, (char *)NULL); 
				  perror("Coudln't exec analysis.");
				  exit(0);
			  }

		default	: // Parent
			  break;
	}

	return 0;
}
