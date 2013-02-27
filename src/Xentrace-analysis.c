#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NUM_ANALYSES 6
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
	printf("Usage: $ ./xa [anaysis-name] [[optional]]\n"
		"\tcpu-util   = Cpu utilization\n"
		"\tsched-lat  = Cpu scheduling latency\n"
		"\txen-time   = Time spent inside xen\n"
		"\tdisk-queue = Disk i/o queue states and wait times\n"
		"\tstats      = Xen event stats\n"
		"\t-a\t: Collect all xentrace events\n"
		"\t-f\t: Log file name\n"
		"Example: $ ./xa xen-time\n"
		"Example: $ ./xa cpu-util -f xentrace.out -a\n\n"
		);
}


Options handle_options(int argc, char **argv)
{
	if(argc < 2) 
	{
		return INVALID;	
	}

	
	if(argc > 2)
	{
		short i = 0;
		// If argv[2] == -f or argv[4] == -f and argv[3|5] != \0
		if((!strcmp(argv[i=2], "-f") || !strcmp(argv[i=4], "-f")) && strcmp(argv[i+1], "\0"))
		{
			xentraceFileName = argv[i+1];
		}
	}
	
	if(!strcmp(argv[1], "all"))
		return ALL_OPTIONS;
	else if(!strcmp(argv[1], "cpu-util"))
		return CPU_UTILIZATION;
	else if(!strcmp(argv[1], "sched-lat"))
		return CPU_WAIT;
	else if(!strcmp(argv[1], "xen-time"))
		return XEN_DOM_TIME;
	else if(!strcmp(argv[1], "disk-queue"))
		return DISK_IO;
	else if(!strcmp(argv[1], "stats"))
		return XEN_STATS;
	else
	{
		return INVALID;
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
		case CPU_UTILIZATION 	: 
		case CPU_WAIT		: argv[5] = "0x0002f000";
					  break;
		case XEN_DOM_TIME	: 
		case XEN_STATS		: argv[5] = "0x0020f000";
					  break;	
		case ALL_OPTIONS 	:
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
	
	// Check existsnce of log file.
	struct stat sb;
	while(!stat(xentraceFileName, &sb) && S_ISREG(sb.st_mode))
	{
		printf("Filename %s exists. Delete and continue y/n ? : ", xentraceFileName);
		char c;
		scanf("%c", &c);

		if((c == 'y') || (c == 'Y'))
		{
			if(unlink(xentraceFileName) == -1)
			{
				perror("Couldn't delete xentrace log file");
				exit(0);
			}
		}
		else
		{
			printf("Enter new filename : ");
			scanf("%s", xentraceFileName);
		}
	}

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

	
	pid_t xentracePid = fork();

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
		kill(xentracePid, SIGTERM);
		exit(0);
	}

	// Received SIGINT. Kill xentrace.
	if(kill(xentracePid, SIGTERM) == -1)
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
	pid_t analysisPid = fork();

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
