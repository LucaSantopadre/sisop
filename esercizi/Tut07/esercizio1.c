#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>


 
cpu_set_t mask;

int N=8;

void* workThread(){
	int stat;
	printf("Thread\n");
	
	int i= 0;
	for(i=0;i<10;i++){
		if(CPU_ISSET(i, &mask)){
			printf("cpu %d affine\n\n",i);
		}
	}
	printf("%d\n",pthread_self());
	
	sleep(2);
	stat=0;
	pthread_exit((void*) stat);
}

int main(int argc,char ** argv){

	pthread_t tid[N];

	CPU_ZERO(&mask);
	CPU_SET(0,&mask);
	int i=0;
	int status;


	for(i=0;i<N;i++){
		/*
		CPU_ZERO(&mask);
		CPU_SET(i%3 ,&mask);
		*/
		printf("i %d   i%3 %d\n",i,i%3);
		
		if(pthread_create(&tid[i],NULL,workThread,NULL) != 0){
			printf("Impossibile creare il thread");
			exit(1);
			break;
		}
		
		pthread_join(tid[i], (void *) &status);

	}
	
	exit(0);
}
