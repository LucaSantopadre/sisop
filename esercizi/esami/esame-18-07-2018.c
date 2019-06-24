/**********************************************************
SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[] i pathname 
associati ad N file, con N maggiore o uguale ad 1. Per ognuno di questi
file generi un processo che legga tutte le stringhe contenute in quel file
e le scriva in un'area di memoria condivisa con il processo padre. Si 
supponga per semplicita' che lo spazio necessario a memorizzare le stringhe
di ognuno di tali file non ecceda 4KB. 
Il processo padre dovra' attendere che tutti i figli abbiano scritto in 
memoria il file a loro associato, e successivamente dovra' entrare in pausa
indefinita.
D'altro canto, ogni figlio dopo aver scritto il contenuto del file nell'area 
di memoria condivisa con il padre entrera' in pausa indefinita.
L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo padre venga colpito da esso dovra' 
stampare a terminale il contenuto corrente di tutte le aree di memoria 
condivisa anche se queste non sono state completamente popolate dai processi 
figli.

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

#define SIZE 4096

int N=0;
char *mem;
key_t key = 1090;
int descr_sem;

union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};





void procedura(char *memoria, char * file_name, int num_processo){
	printf("FIGLIO nome file: %s\n", file_name);

	FILE *file;
	//char *buffer;
	int size;

	


	memoria = memoria + (num_processo * SIZE);
	printf("num processo: %d    indirizzo %p\n\n", num_processo, memoria);


	file = fopen(file_name, "r+");

	while(!feof(file)){
		size = fscanf(file, "%s", memoria );
		printf("%s\n", memoria);
		memoria = memoria + strlen(memoria);
	}

	//printf("fine %s\n", memoria);


	struct sembuf oper;
	oper.sem_num = 0;
	oper.sem_op = 1;
	oper.sem_flg = 0;
	semop(descr_sem, &oper, 1);

	pause();

}

void printer(){

	printf("\n\n\nPRINTER\n");
	for (int i = 0; i < N; ++i)
	{
		printf("%s\n", mem + (i * SIZE));
	}
	
	


}


int main(int argc, char* argv[]){

	pid_t pid;
	int status;

	if(argc<2){
		printf("usage: command filename[]\n");
		exit(-1);
	}

	N = argc - 1;

	// inizializzo memoria condivisa
	mem = (char*) mmap(NULL, N * SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	if(mem == (void*) - 1){
		printf("Impossibile creare memoria condivisa \n");
		exit(-1);
	}
	printf("indirizzo memoria iniziale   %p\n\n", mem);

	// inizializzazione semaforo
	descr_sem = semget(key, 1, IPC_CREAT | 0666);
	if(descr_sem == -1){
		printf("Impossibile creare semaforo\n");
		exit(-1);
	}

	union semun arg;
	arg.val = 0;
	semctl(descr_sem, 0, SETVAL, arg);


	for (int i = 0; i < N; ++i)
	{
		pid = fork();
		if(pid == 0){
			signal(SIGINT, SIG_IGN);
			procedura(mem, argv[i+1], i);
		
		}
		
	}

	signal(SIGINT, printer);


	struct sembuf oper;
	oper.sem_num = 0;
	oper.sem_op = -N;
	oper.sem_flg = 0;
	semop(descr_sem, &oper, 1);


	//wait(&status);
	
	pause();




}