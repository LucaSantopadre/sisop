/***********************************************************
SPECIFICATION TO BE IMPLEMENTED: 
Implementare un'applicazione che riceva in input tramite argv[] il 
nome di un file F ed una stringa indicante un valore numerico N maggiore
o uguale ad 1.
L'applicazione, una volta lanciata dovra' creare il file F ed attivare 
N thread. Inoltre, l'applicazione dovra' anche attivare un processo 
figlio, in cui vengano attivati altri N thread. 
I due processi che risulteranno attivi verranno per comodita' identificati
come A (il padre) e B (il figlio) nella successiva descrizione.

Ciascun thread del processo A leggera' stringhe da standard input. 
Ogni stringa letta dovra' essere comunicata al corrispettivo thread 
del processo B tramite memoria condivisa, e questo la scrivera' su una 
nuova linea del file F. Per semplicita' si assuma che ogni stringa non
ecceda la taglia di 4KB. 

L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo A venga colpito esso dovra' 
inviare la stessa segnalazione verso il processo B. Se invece ad essere 
colpito e' il processo B, questo dovra' riversare su standard output il 
contenuto corrente del file F.

Qalora non vi sia immissione di input, l'applicazione dovra' utilizzare 
non piu' del 5% della capacita' di lavoro della CPU. 

*****************************************************************/


#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N;
char *file_name;
int fd;
char **mem;


void* function_thread_A(void* buf){
	int *num = buf;
	printf("A  %d\n", *num);

	//scanf("%s", mem[*num]);
	printf("%p\n", mem[*num]);

}

void *function_thread_B(void* buf){
	int *num = buf;
	int fd;
	printf("B  %d\n", *num);


	fd = open(file_name, O_RDWR, 0666);
	printf("fd %d\n", fd);
}


int main(int argc, char* argv[]){
	int len;
	pthread_t tid;
	pid_t pid;

	if(argc != 3){
		printf("usage: command file N\n");
		exit(-1);
	}

	N = strtol(argv[2], NULL, 10);
	len = strlen(argv[1]);
	file_name = malloc(len * sizeof(char));
	file_name = argv[1];

	printf("file: %s    N: %d\n",file_name, N);

	fd = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0666);


	//inizializzo memoria condivisa
	mem = malloc(N * sizeof(char*));

	for (int i = 0; i < N; ++i)
	{
		mem[i] =(char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , 0,0 );
	}
	printf("memoria condivisa mappata\n");




	for (int i = 0; i < N; ++i)
	{
		int x = i;
		pthread_create(&tid, NULL, function_thread_A, (void*) &x);
	}

	pid = fork();
	if(pid == 0){
		for (int i = 0; i < N; ++i)
		{
			int x = i;
			pthread_create(&tid, NULL, function_thread_B,(void*) &x);
		}

	}


	sleep(5);




}
