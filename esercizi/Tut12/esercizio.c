#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>


#define N 4
#define M 8
#define SIZE 4096

pid_t *scrittori;
pid_t *lettori;
int descrittore_memoria_condivisa;
int *indice_scrittore;

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

// ---------------------------------------------------

void* createSharedMemory(key_t key){
	int descr;
	void* addr;

	descr = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if(descr == -1){
		printf("Impossibile creare mem condivisa con chiave %d\n",key);
		shmctl(descr, IPC_RMID, NULL);
		exit(-1);
	}
	descrittore_memoria_condivisa = descr;

	addr = shmat(descr, NULL, 0);
	if(addr == (void*)-1){
		printf("Impossibile attach mem condivisa con chiave %d\n", key);
		shmctl(descr, IPC_RMID, NULL);
		exit(-1);
	}

	printf("[info] mem condivisa creata.\n");
	printf("[info] descrittore: %d\n", descr);
	printf("[info] indirizzo: %p\n\n", addr);

	return addr;
	//shmctl(descr, IPC_RMID, NULL);
}

int createSemaphore(key_t k){
	int descr;

	descr = semget(k, 2 , IPC_CREAT | 0666);
	if(descr == -1){
		printf("Impossibile creare SEMAFORO con chiave %d\n",k);
		shmctl(descr, IPC_RMID, NULL);
		exit(-1);
	}

	// inizializzo semaforo
	union semun arg;
	arg.val = M;
	semctl(descr, 0, SETVAL, arg);

	arg.val = 0;
	semctl(descr, 1, SETVAL, arg);

	printf("[info] semaforo creato.\n");
	printf("[info] descrittore: %d\n\n", descr);

	//semctl(descr, IPC_RMID, 0);

	return descr;
}


void token_ring(){
	printf("%d  sono stato colpito!!!!\n", getpid());
}

void iniizializzaSegnaliScrittori(){
	sigset_t set;
	sigfillset(&set);

	struct sigaction act;
	act.sa_sigaction = token_ring ;
	act.sa_mask = set ;
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, NULL);

	printf("[info] segnali inizializzati.\n\n");
}

// ------------------------------------------------------





int main(int argc, char* argv[]){
	pid_t pid;
	int status;
	key_t key_m = 1271;
	key_t key_s = 8450;
	int descr_sem;
	char* buffer;

	printf("-----------  INIT -----------\n\n");

	// inizializzazione semafori e memoria condivisa
	scrittori = mmap(0, N *sizeof(pid_t), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	lettori   = mmap(0, M *sizeof(pid_t), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	buffer = createSharedMemory(key_m);
	descr_sem = createSemaphore(key_s);

	// inizializzazione segnali scrittori,  eredidati dai figli 
	iniizializzaSegnaliScrittori();
	indice_scrittore  = mmap(0, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


	printf("----------- FINE INIT -----------\n\n");

	// genero N scrittori
	for (int i = 0; i < N; ++i)
	{
		pid = fork();
		//figlio
		if(pid == 0){
			scrittori[i]=getpid();
			struct sembuf oper;

			while(1){
				pause();
				// wait che posso scrivere (ho bisogno di M gettoni)
				printf("wait(S0) . . .\n");
				oper.sem_num = 0;
				oper.sem_op = -M ;
				oper.sem_flg = 0;
				semop(descr_sem, &oper, 1);


				//scrivi su memoria condivisa
				printf("Sto scrivendo sulla memoria indice: %d\n", *indice_scrittore);
				sprintf(buffer, "ciao");


				//signal ho scritto (segnalo hai lettori che ho fatto, quindi i lettori hanno M gettoni)
				printf("signal(S1).\n");
				oper.sem_num = 1;
				oper.sem_op = M ;
				oper.sem_flg = 0;
				semop(descr_sem, &oper, 1);
				

				*indice_scrittore = (*indice_scrittore + 1) % N;
				printf("fatto... colpisco %d\n\n\n", *indice_scrittore);


				// colpisci prossimo processo
				kill(scrittori[*indice_scrittore], SIGUSR1);
				exit(0);

			}
		}
	}
	sleep(3);
	printf("id scrittori: ");
	for (int i = 0; i < N; ++i)
	{
		printf("%d ",scrittori[i]);
	}
	printf("\n");

	*indice_scrittore = 0;
	kill(scrittori[*indice_scrittore], SIGUSR1);




	// genero M lettori
	for (int i = 0; i < M; ++i)
	{
		pid = fork();
		//figlio
		if(pid == 0){
			lettori[i]=getpid();
			struct sembuf oper;

			// wait(S1)
			oper.sem_num = 1;
			oper.sem_op = -1;
			oper.sem_flg = 0;
			semop(descr_sem, &oper, 1);


		}
	}
	sleep(3);

	printf("id lettori: ");
	for (int i = 0; i < M; ++i)
	{
		printf("%d ",lettori[i]);
	}
	printf("\n");






	/* test 1 lettore . OKKKKK

	struct sembuf oper;
	// wait(S1)
	printf("******************* Wait(S1) . . . . . . . . %d\n", descr_sem);
	oper.sem_num = 1;
	oper.sem_op = -1;
	oper.sem_flg = 0;
	semop(descr_sem, &oper, 1);

	printf("TOKEN per leggere ricevuto buffer: %s\n",buffer);

	sleep(3);

	// signal(S0)
	oper.sem_num = 0;
	oper.sem_op = 1;
	oper.sem_flg = 0;
	semop(descr_sem, &oper, 1);
	*/

/*
	int k = 0;
	int index ;
	while(k < 6){
		int index = k % N;
		printf("----- k %d index %d \n",k, index);
		kill(scrittori[index], SIGUSR1);
		sleep(2);
		k++;
	}
*/
	//sleep(10);
	//wait(&status);	
	shmctl(descrittore_memoria_condivisa, IPC_RMID, NULL);
	exit(0);
	


	// test 1 scrittore e 1 lettore  OKKKK!!!!
	/*
	pid = fork();
	if(pid == 0){
		struct sembuf oper;
		
		while(1){
			// wait(S0)
			oper.sem_num = 0;
			oper.sem_op = -1;
			oper.sem_flg = 0;
			semop(descr_sem, &oper, 1);

			scanf("%s",buffer);
			printf("buffer: %s\n",buffer);

			// signal(S1)
			oper.sem_num = 1;
			oper.sem_op = 1;
			oper.sem_flg = 0;
			semop(descr_sem, &oper, 1);
		}
	}

	struct sembuf oper;

	while(1){
			// wait(S1)
			oper.sem_num = 1;
			oper.sem_op = -1;
			oper.sem_flg = 0;
			semop(descr_sem, &oper, 1);

			printf("PADRE: %s\n",buffer);

			// signal(S1)
			oper.sem_num = 0;
			oper.sem_op = 1;
			oper.sem_flg = 0;
			semop(descr_sem, &oper, 1);
		}
	sleep(10);
	exit(0);
	//end test
	*/
}