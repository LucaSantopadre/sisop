/****************************************************************

SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[] i pathname 
associati ad N file, con N maggiore o uguale ad 1. Per ognuno di questi
file generi un thread (quindi in totale saranno generati N nuovi thread 
concorrenti). 
Successivamente il main-thread acquisira' stringhe da standard input in 
un ciclo indefinito, ed ognuno degli N thread figli dovra' scrivere ogni
stringa acquisita dal main-thread nel file ad esso associato.
L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso 
WinAPI) in modo tale che quando uno qualsiasi dei thread dell'applicazione
venga colpito da esso dovra' stampare a terminale tutte le stringhe gia' 
immesse da standard-input e memorizzate nei file destinazione.

*****************************************************************/
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

int N;
char *buffer;
key_t key = 5437;
int descr_sem;
struct sembuf oper;

__thread char* nome_file;


union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};


void printer(){
	char *temp = malloc(128);

	FILE *file;

	printf("--------------- colpito , mio file: %s\n\n", nome_file);

	file = fopen(nome_file, "r+");

	while (fscanf(file,"%s",temp) != EOF){
		printf("read %s\n",temp);
		temp += strlen(temp)+1;
	}
}


void* procedura(void *name){

	

	//struct sembuf oper;
	//nome_file = malloc(128);
	//strcpy(nome_file, (char*)name);
	nome_file = (char*) name;

	printf("mio nome file:%s\n", nome_file);

	int df, result;
	df = open(name ,O_WRONLY, 0666);


	while(1){
		oper.sem_num = 1;
		oper.sem_op = -1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);



		printf("thread %s\n",buffer);
		result = write(df , buffer, strlen(buffer));  
		if (result == -1) {
     	 	printf("destination file write error\n");
        	exit(1);
       }




		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);
	}
}





int main(int argc, char* argv[]){

	pthread_t tid;
	int ret;


	N = argc -1;
	if(argc < 2){
		printf("usage: command files");
		exit(-1);
	}

	buffer = malloc(BUFSIZE);

	// inizializzo semaforo
	descr_sem = semget(key, 2, IPC_CREAT | 0666);
	//union semun arg;

	//arg.val = N;
	semctl(descr_sem, 0, SETVAL, N);

	//arg.val = 0;
	semctl(descr_sem, 1, SETVAL, 0);

	
	signal(SIGINT, printer);

	nome_file = argv[1];

	for (int i = 0; i < N; ++i)
	{
		pthread_create(&tid, NULL, procedura, (void*) argv[i+1]);
	}


	//signal(SIGINT, SIG_IGN);


	//printf("MAIN thread, inserisci stringhe\n");
	while(1){
		oper.sem_num = 0;
		oper.sem_op = -N;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);

		printf("MAIN thread, inserisci stringhe\n");
		scanf("%s", buffer);
		getc(stdin);

		oper.sem_num = 1;
		oper.sem_op = N;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);
	}
	

	exit(0);


}