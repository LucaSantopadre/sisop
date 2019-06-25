/**************************************************
SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[1] un numero
intero N maggiore o uguale ad 1 (espresso come una stringa di cifre 
decimali), e generi N nuovi thread. Ciascuno di questi, a turno, dovra'
inserire in una propria lista basata su memoria dinamica un record
strutturato come segue:

typedef struct _data{
	int val;
	struct _data* next;
} data; 

I record vengono generati e popolati dal main thread, il quale rimane
in attesa indefinita di valori interi da standard input. Ad ogni nuovo
valore letto avverra' la generazione di un nuovo record, che verra'
inserito da uno degli N thread nella sua lista. 
L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra' 
stampare a terminale il contenuto corrente di tutte le liste (ovvero 
i valori interi presenti nei record correntemente registrati nelle liste
di tutti gli N thread). 

*****************************************************************/
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N;
pthread_mutex_t mutex;
pthread_mutex_t next;

int value;


typedef struct _data{
	int val;
	struct _data* next;
} data; 

struct _data *list;



void* thread_function(void* arg){
	int *num_thr = (int*) arg;

	while(1){
		struct _data *record;

		pthread_mutex_lock(&mutex);

		printf("Thread n. %d , read the value %d\n", *num_thr, value);
		record->val = value;
		printf("inserisco...\n");
		list[*num_thr] = *record;

		pthread_mutex_unlock(&next);

	}
}


void printer(){
	int i;
	for (i = 0; i < N; ++i)
	{
		printf("%d:%d    ", i, list[i].val);
	}
	printf("\n\n");
}


int main(int argc, char * argv[]){

	pthread_t tid;
	sigset_t set;
	struct sigaction act;
	//sigfillset(&set);



	if(argc < 2){
		printf("usage: command N\n");
		exit(0);
	}

	// leggo intero da argv[1]
	N = strtol(argv[1], NULL, 10);
	printf("N: %d\n", N);

	// inizializzo lista di tipo struct _data
	list = malloc(N * sizeof(data));
	for(int i = 0; i< N ; i++){
		list[i].val = -1;
		list[i].next = NULL;
	}

	// inizializzo mutex
	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&next, NULL);

	// signal init
	sigfillset(&set);

	act.sa_sigaction = printer;
	act.sa_mask = set;
	act.sa_flags = 0;

	sigaction(SIGINT, &act, NULL);






	// spawn N threads
	for (int i = 0; i < N; ++i)
	{
		int x = i;
		pthread_create(&tid, NULL, thread_function, (void*) &x);	
	}


	
	printf("MAIN THREAD , inserisci valori interi\n");

	while(1){
		pthread_mutex_lock(&next);

		scanf("%d",&value);

		pthread_mutex_unlock(&mutex);

	}

	exit(0);

}