#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


void *foo1(void *arg)
{
	char *buff;

	if ((buff = (char *) arg) == NULL)
		pthread_exit((void *) 1);

	printf("\nInserisci un messaggio: ");
	scanf("%[^\n]", buff);

	pthread_exit((void *) 0);
}


void *foo2(void *arg)
{
	char *buff;

	if ((buff = (char *) arg) == NULL)
		pthread_exit((void *) 1);

	printf("Messaggio letto: %s\n\n", buff);

	pthread_exit((void *) 0);
}


int main()
{
	int result;
	void *exit_status;
	pthread_t thread;

	char *buff;

	if ((buff = (char *) malloc((size_t) 4096)) == NULL)
	{
		printf("Unable to allocate memory space for the buffer. Exit.\n");
		return 1;
	}

	if ((result = pthread_create(&thread, NULL, foo1, (void *) buff)))
	{
		printf("Unable to create thread for executing foo1. Exit.\n");
		return 1;
	}

	if (pthread_join(thread, &exit_status))
	{
		printf("Unable to join thread executing foo1. Exit.\n");
		return 1;
	}
	else if ((unsigned long long int) exit_status)
	{
		printf("An error has occurred while executing foo1. Exit.\n");
		return 1;
	}

	if ((result = pthread_create(&thread, NULL, foo2, (void *) buff)))
	{
		printf("Unable to create thread for executing foo2. Exit.\n");
		return 1;
	}

	if (pthread_join(thread, &exit_status))
	{
		printf("Unable to join thread executing foo2. Exit.\n");
		return 1;
	}
	else if ((unsigned long long int) exit_status)
	{
		printf("An error has occurred while executing foo2. Exit.\n");
		return 1;
	}

	free(buff);
	
	return 0;
}