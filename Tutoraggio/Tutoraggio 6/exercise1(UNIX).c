#include <unistd.h>
#include <pthread.h>
#include <stdio.h>


volatile unsigned int shared_variable = 1;


void *thread_function(void *arg)
{
	printf("2. Child Thread is executing.\n");
	sleep(5);
	printf("3. Child Thread is terminating.\n");
	shared_variable = 0;
	pthread_exit(NULL);
}


int main()
{
	int result;
	void *exit_status;
	pthread_t thread;
	printf("1. Parent Thread is executing.\n");
	if ((result = pthread_create(&thread, NULL, thread_function, NULL))) {
		printf("Unable to create a new thread. Exit.\n");
		return 1;
	} else {
		while (shared_variable) ;
		printf("4. Parent Thread is terminating.\n");
		// pthread_join(thread, &exit_status);
	}
	return 0;
}