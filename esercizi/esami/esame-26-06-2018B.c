/***************************************************

SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[2] un numero
intero N maggiore o uguale ad 1 (espresso come una stringa di cifre 
decimali), e generi N nuovi processi. Ciascuno di questi leggera' in modo 
continuativo un valore intero da standard input, e lo comunichera' al
processo padre tramite memoria condivisa. Il processo padre scrivera' ogni
nuovo valore intero ricevuto su di un file, come sequenza di cifre decimali. 
I valori scritti su file devono essere separati dal carattere ' ' (blank).
Il pathname del file di output deve essere comunicato all'applicazione 
tramite argv[1].
L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che se il processo padre venga colpito il contenuto
del file di output venga interamente riversato su standard-output.
Nel caso in cui non vi sia immissione in input, l'applicazione non deve 
consumare piu' del 5% della capacita' di lavoro della CPU.

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

#define PAGE_SIZE 4096

int N;
char *file_name;
int *mem;



void procedura_figlio(int descr){
	struct sembuf oper;
	int val;

	while(1){
		
		

		// wait che padre ha scritto nel file
		oper.sem_num = 1;
		oper.sem_op = -1;
		oper.sem_flg = 0;
		semop(descr, &oper, 1);

		printf("OK puoi scrivere nel buffer\n");
		scanf("%d",&val);
		*mem = val;
		

		// signal che ho scritto nel buffer
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = 0;
		semop(descr, &oper, 1);
	}
}



void printer(){
	FILE *f;
	f = fopen(file_name, "r+");
	int ret;
	char *buffer = malloc(128);

	while( ret = fscanf(f, "%s", buffer)  != EOF){
		printf("%s ",buffer);
	}
	printf("\n");
}




int main(int argc, char* argv[]){
	int len;
	
	int fd;
	FILE *file;

	key_t key = 8455;
	int descr_sem;

	pid_t pid;
	int res;

	if(argc != 3){
		printf("usage: command filename N");
	}

	N = strtol(argv[2], NULL, 10);
	len = strlen(argv[1]);
	file_name = malloc(len * sizeof(char));
	file_name = argv[1];

	// init memoria condivisa
	mem =  mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , 0, 0);
	if(mem == (void*) -1 ){
		printf("Map error.\n");
		exit(0);
	}

	// init semaforo
	descr_sem = semget(key, 2, IPC_CREAT | 0666);
	if(descr_sem == -1){
		printf("Semaphore init error.\n");
		exit(0);

	}
	semctl(descr_sem, 0, SETVAL, 0);
	semctl(descr_sem, 1, SETVAL, 1);


	fd = open(file_name, O_RDWR | O_CREAT , 0666);
	file = fdopen(fd, "w+");


	for (int i = 0; i < N; ++i)
	{
		pid = fork();
		if(pid == -1){
			printf("Fork error.\n");
			exit(0);
		}

		if(pid == 0){
			procedura_figlio(descr_sem);
		}
	}
	

	signal(SIGINT, printer);

	// padre
	struct sembuf oper;
	while(1){
		// wait messaggio nel buffer condiviso
		oper.sem_num = 0;
		oper.sem_op = -1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);

		

		res = fprintf(file, "%d ",  *mem);
		if(res == -1){
			printf("Impossibile scrivere sul file\n");
			exit(-1);
		}
		printf("Ho scritto: %d\n", *mem);
		fflush(file);


		//signal che ho scritto sul file
		oper.sem_num = 1;
		oper.sem_op = 1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);
	}



}