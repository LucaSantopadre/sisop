#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>


pid_t pid1;
pid_t pid2;


void sigusr1_handler(int sig)
{
	printf("Process %d received signal number %d.\n", pid1, sig);
	sleep(1);
	kill(pid2, SIGUSR1);
}


int main(void)
{
	sigset_t set;
	sigset_t old_set;
	struct sigaction act;
	struct sigaction old_act;

	memset((void *) &act, 0, sizeof(struct sigaction));

	/* Blocks all signals until we will
	   be ready to handle them.  */
	if (sigfillset(&set)) exit(-1);
	if (sigprocmask(SIG_BLOCK, &set, &old_set)) exit(-1);

	/* We want our SIGUSR1 signal handler
	   be insensible to all signals while
	   executing.  */
	act.sa_mask = set;
	act.sa_handler = sigusr1_handler;
	if (sigaction(SIGUSR1, &act, &old_act)) exit(-1);

	/* Ok. We can now un-block at least
	   SIGINT and SIGUSR1 signals for
	   our purposes.  */
	if (sigemptyset(&set)) exit(-1);
	if (sigaddset(&set, SIGINT)) exit(-1);
	if (sigaddset(&set, SIGUSR1)) exit(-1);
	if (sigprocmask(SIG_UNBLOCK, &set, NULL)) exit(-1);

	if ((pid2 = fork()) == -1)
		exit(-1);
	else if (pid2 == 0)
	{
		/* Child Process */
		pid1 = getpid();
		pid2 = getppid();

		while (1)
		{
			pause();
		}
	}
	else
	{
		/* Parent Process */
		pid1 = getpid();

		while (raise(SIGUSR1)) ;

		while (1)
		{
			pause();
		}
	}

	/* Restores both the old action for
	   SIGUSR1 signal and the old signal
	   mask for this process.  */
	if (sigaction(SIGUSR1, &old_act, NULL)) exit(-1);
	if (sigprocmask(SIG_SETMASK, &old_set, NULL)) exit(-1);

	exit(0);
}