#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


#define BUFFER_SIZE				1024
#define MAX_REQUEST_HANDLERS	10


typedef struct __thread_arguments {
	unsigned int thread_index;
	sem_t sem_1 __attribute__((aligned(64)));
	sem_t sem_2 __attribute__((aligned(64)));
	int conn_desc __attribute__((aligned(64)));
} thread_args;


void * request_handler(void *arg)
{
	int conn;
	ssize_t bytes;
	thread_args *args;
	char buffer[BUFFER_SIZE];
	unsigned int i;

	if ((args = (thread_args *) arg) == NULL)
		pthread_exit(NULL);

	while (1)
	{
		sem_wait(&(args->sem_1));

		conn = args->conn_desc;
		args->conn_desc = -1;

		if ((bytes = read(conn, (void *) buffer, BUFFER_SIZE-1)) != -1)
		{
			buffer[bytes] = '\0';
			printf("[MESSAGE] Thread %u : %s\n", args->thread_index, buffer);
		}

		close(conn);

		sem_post(&(args->sem_2));
	}
}


int main()
{
	unsigned int i;
	unsigned int assigned;

	int sock_desc;
	int conn_desc; 

	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	socklen_t addr_len;

	unsigned int num_handlers;
	pthread_t req_handlers[MAX_REQUEST_HANDLERS];

	thread_args args[MAX_REQUEST_HANDLERS];

	for (i=0; i<MAX_REQUEST_HANDLERS; i++)
	{
		args[i].thread_index = i;
		sem_init(&(args[i].sem_1), 0, 0);
		sem_init(&(args[i].sem_2), 0, 1);
		args[i].conn_desc = -1;
	}

	for (i=0, num_handlers=0; i<MAX_REQUEST_HANDLERS; i++)
		if (pthread_create(&req_handlers[num_handlers], NULL, request_handler, &args[num_handlers]) == 0)
			num_handlers++;

	if ((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		exit(-1);

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(10001);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_desc, (struct sockaddr *) &serv_addr, (socklen_t) sizeof(struct sockaddr_in)) == -1)
    	exit(-1);

    if (listen(sock_desc, 1024) == -1)
    	exit(-1);

	addr_len = (socklen_t) sizeof(struct sockaddr_in);

    while (1)
	{
		if ((conn_desc = accept(sock_desc, (struct sockaddr *) &cli_addr, &addr_len)) == -1)
			exit(-1);

		printf("[INFO] Server : Accepted connection from %s\n", inet_ntoa(cli_addr.sin_addr));

		assigned = 0;

		while (!assigned)
		{
			for (i=0; i<num_handlers; i++)
			{
				if (sem_trywait(&(args[i].sem_2)) == 0)
				{
					if (args[i].conn_desc == -1)
					{
						args[i].conn_desc = conn_desc;
						sem_post(&(args[i].sem_1));

						assigned = 1;
						break;
					}
				}
			}
		}
    }
}