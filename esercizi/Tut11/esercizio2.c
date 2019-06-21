#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10000000

int data[SIZE] = {[0 ... (SIZE-1)] -1};
pthread_mutex_t mutex;
int indice = 0;


void* workThread(void* buffer){
	int * num = (int*) buffer;
	//printf("I'm Thread %d\n\n", *num);
	//sleep(2);

	while(1){
		pthread_mutex_lock(&mutex);

		if(indice < SIZE){
			//printf("%d   ",data[indice]);
			data[indice] = indice;
			//printf("%d   \n",data[indice]);
			indice++;
		}else{
			pthread_mutex_unlock(&mutex);
			break;
		}

		pthread_mutex_unlock(&mutex);
	}


	pthread_exit((void*) 0);
}


int main(int argc, char* argv[]){
	long int N;
	pthread_t tid;
	void* status;
	int join;

	if(argc<2){
		printf("usage: program integer\n");
		exit(-1);
	}
	N = strtol(argv[1],NULL,10);
	printf("N: %ld\n",N);

	printf("data: ");
	for (int j = 0; j < SIZE; ++j)
	{
		printf("%d ",data[j]);
	}
	printf("\n");


	// init mutex
	printf("mutex init . . . ");
	for (int j = 0; j < SIZE; ++j)
	{
		pthread_mutex_init(&mutex, NULL);
	}
	printf(" OK. \n");




	// spawn N threads
	for (int i = 0; i < N; ++i)
	{
		int x = i;
		tid = pthread_create(&tid,NULL, workThread, (void*) &x);
		if(tid == -1){
			printf("Attenzione impossibile creare thread indice %d",i);
			continue;
		}
		/*
		join = pthread_join(tid, &status);
		if(join == -1){
			printf("Attenzione impossibile fare join thread indice %d",i);
			break;
		}
		*/
		
	}
	sleep(5);
	printf("NEW data: ");
	for (int j = 0; j < SIZE; ++j)
	{
		printf("%d ",data[j]);
	}
	printf("\n");


}