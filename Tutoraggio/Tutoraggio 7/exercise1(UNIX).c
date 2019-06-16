#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CPUs 4
#define CPU_BUSY_TIME 2*1000*1000*1000L

void * thread_function(void *arg)
{
	char *retval;
	int i;

	retval = strdup("All-right!");

	for (i=0; i<CPU_BUSY_TIME; i++) ;

	pthread_exit((void *) retval);
}

int main()
{
	pthread_attr_t attr;
	pthread_t thrd;
	cpu_set_t cpus;
	int stack_size;
	void *res;
	int i;

	stack_size = 8388608;

	for (i=0; i<CPUs; i++) {

		if (pthread_attr_init(&attr)) {
			printf("Error. Unable to initialize thread attributes.\n");
			exit(1);
		}

		if (pthread_attr_setstacksize(&attr, stack_size)) {
			printf("Warning. Unable to set thread stack size.\n");
		}

		CPU_ZERO(&cpus);
		CPU_SET(i, &cpus);
		if (pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus)) {
			printf("Warning. Unable to set thread affinity.\n");
		}

		if (pthread_create(&thrd, &attr, thread_function, NULL)) {
			printf("Error. Unable to start new thread.\n");
			return 1;
		}

		if (pthread_attr_destroy(&attr)) {
			printf("Warning. Unable to destroy thread attributes.\n");
		}

		if (pthread_join(thrd, &res)) {
			printf("Error. Unable to join created thread.\n");
			return 1;
		}

		printf("Joined with thread. Returned value was: %s.\n", (char *) res);
		free(res);

		sleep(2);

	}

	return 0;
}