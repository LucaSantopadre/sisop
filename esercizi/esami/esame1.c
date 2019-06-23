/**********************************************************************
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

********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/shm.h>

#define PAGE_SIZE 4096


int main(int argc, char* argv[]){

	int N;
	char *file_name;
	key_t key = 1246;
	int descr;
	int *buffer;
	pid_t pid;
	pid_t *processi;

	if(argc < 3){
		printf("usage: command file_path N\n");
		exit(-1);
	}

	N = strtol(argv[2], NULL, 10); 
	file_name = argv[1];

	if(N < 0 ){
		printf("N deve esserre >= 0\n");
		exit(-1);
	}

	// init memoria condivisa
	descr = shmget(key, PAGE_SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if(descr == -1){
		printf("Impossibile creare memoria condivisa\n");
		exit(-1);
	}

	buffer = shmat(descr, NULL, 0);
	if(buffer == (void*)-1 ){
		printf("Impossibile effettuare attach mem condivisa\n");
		exit(-1);
	}
	printf("Memoria condivisa creata, descrittore: %d\n", descr);


	processi = mmap(NULL, N * sizeof(pid_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);


	// genero N processi
	for (int i = 0; i < N; ++i)
	{
		pid = fork();
		processi[i] = pid;
		if(pid == 0){
			int x;

			/*
			while(1){
				pause();
				printf("figlio %d inserisci un intero x \n", getpid());
				scanf("%d", &x);
				*buffer = x;
				exit(0);
			}
			*/

		}
	}


	sleep(5);
	for (int i = 0; i < N; ++i)
	{
		printf("%d ", processi[i]);
	}
	shmctl(descr, IPC_RMID, NULL);
	exit(0);





}