#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>



void* thread1_read(void *buf){
	int status;

	char *s= (char *)buf;
	printf("buffer nel thread1  %d\n",buf);
	printf("THREAD 1: Inserisci una stringa\n");
	scanf("%s",s);
	status=0;
	pthread_exit(&status);
}

void* thread2_write(void *buf){
	
	char *s= (char *)buf;
	printf("\nTHREAD 2: %s\n",s);
	pthread_exit((void*)0);
}

int main(int argc, char **argv){

	pthread_t tid;
	void* status;

	char *buffer;
	buffer = (char*)malloc((size_t) 4096);

	printf("buffer nel main %d\n",buffer);

	pthread_create(&tid,NULL,thread1_read,(void *)buffer);
	pthread_join(tid,&status);


	pthread_create(&tid,NULL,thread2_write,(void *)buffer);
	

	
	pthread_join(tid,&status);


	exit(0);

}