#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int control=0;
int status;



void* thread_function(void *p){
	sleep(3);
	printf("SONO THREAD FIGLIO %d\n",getpid());
	control=1;
	pthread_exit(&status);
}

int main(int argc, char *argv[]){

	pthread_t tid;

	pthread_create(&tid,NULL,thread_function,NULL);

	/* SOL CON pthread_join
	void *stat;
	pthread_join(tid,&stat);
	*/

	// SOL senza pthread_join
	while(!control){
		sleep(1);
	}
	
	printf("PADRE\n");
}