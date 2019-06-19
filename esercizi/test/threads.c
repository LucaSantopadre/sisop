#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* workRoutine(void *buffer){
	int *x = (int*)buffer;

	printf("I'm thread! %d\n", *x);
	sleep(5);
	pthread_exit((void*) 0 );
}




int main(int argc, char* argv[]){
	int i;
	long int N;
	void *status;
	char *endptr;
	int ret;

	if(argc<2){
		printf("usage: command num_threads");
		exit(EXIT_FAILURE);

	}else if(argc == 2){

		N = strtol(argv[1],&endptr , 10);
		printf("long %ld\n",N);

		pthread_t tid[N];
		int map[N];
		for (int j = 0; j < N; ++j)
		{
			map[j]=0;

		}



		for (int i = 0; i < N; ++i)
		{
			int num = i;
			//spawn N threads
			ret = pthread_create(&tid[i],NULL, workRoutine, &num);
			if(ret != 0 ){
				printf("IMPOSSIBILE CREARE threads n %d\n",i);
				exit(-1);
			}

		}
		/*
		for(i=0; i<N; i++){
			pthread_join(tid[i], &status);
		}
		*/

	}


}