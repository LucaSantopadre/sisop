#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct message_group
{
	unsigned int level;
	char *message[10];
	struct message_group *others;
};

struct message_group *p;
struct message_group *p2;


void* workThread(void* var){
	int *X = (int*) var;
	printf("thread valore i: %d\n",*X);

	struct message_group * g1 = p;
	//printf("Thread %d\n",g1->level);
	if(*X < (10*g1->level)){
		g1->message[*X] = *X;
	}else{

		g1->others= p2;

		struct message_group group2;
		group2.level=2;
		int a=0;
		for(a=0;a<10;a++){
			group2.message[a] = calloc(10*sizeof(char),0);
			group2.message[a]='0';
		}

		p2=&group2;
	}
	sleep(1);
	pthread_exit((void *) 0);
}



int main(int argc, char**argv){

	struct message_group group1;
	group1.level=1;
	group1.others=NULL;
	int a=0;
	for(a=0;a<10;a++){
		group1.message[a] = calloc(10*sizeof(char),0);
		group1.message[a]='0';
	}
	

	p=&group1;

	void *status;

	pthread_t tid[50];

	//pthread_create(&tid, NULL, workThread, (void*) &group1);
	//pthread_join(tid,&status);

	int i;

	int x;
	for(i=0; i<50; i++){
		x=i;
		if(pthread_create(&tid[i], NULL, workThread, (void*) &x) != 0){
			printf("Impossibile creare thread\n");
			exit(-1);
		}
		pthread_join(tid[i],&status);


		int j;
		for(j=0; j<10; j++){
			printf("%d ",group1.message[j]);
		}
		printf("\n");
		if(group1.others>1){
			for(j=0; j<10; j++){
				printf("%d ",group1.others->message[j]);
			}
		}
		
		
		printf("\n\n\n");
	}
	


}