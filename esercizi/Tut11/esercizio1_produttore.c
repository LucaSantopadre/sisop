#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define PAGE_SIZE 4096


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};




int main(int argc, char*argv[]){
	int descr_mem;
	int descr_sem;
	key_t key_mem = 205;
	key_t key_sem = 305;
	char *buffer;

	// creo area mem condivisa
	descr_mem = shmget(key_mem,PAGE_SIZE, IPC_CREAT| IPC_EXCL| 0666 );
	if(descr_mem  == -1){
		printf("Impossibile creare mem condivisa con key %d\n",key_mem);
		//exit(-1);
	}
	printf("Descrittore memoria %d\n", descr_mem);

	// attach area mem condivisa
	buffer = shmat(descr_mem, NULL, 0);
	if(buffer == (void*)-1){
		printf("Impossibile eseguire attach su descrittore %d\n", descr_mem);
		//exit(-1);

	}

	// creo semaforo
	descr_sem = semget(key_sem, 2, IPC_CREAT|IPC_EXCL|0666);
	if(descr_sem == -1){
		printf("Impossibile creare semaforo con key %d\n",key_sem);
		//exit(-1);
	}
	printf("Descrittore  semaforo %d\n", descr_sem);

	// inizializzo semaforo
	struct sembuf oper;
	union semun sem_arg;

	sem_arg.val = 1;
	semctl(descr_sem, 0, SETVAL, sem_arg);

	sem_arg.val = 0;
	semctl(descr_sem, 1, SETVAL, sem_arg);







	while(1){
		
		// chiedo token su semaforo 0 che regola la scrittura
		oper.sem_num = 0;
		oper.sem_op = -1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);

		// token ricevuto , scrivo
		printf("Token per scrivere ricevuto, scrivi messaggio\n");
		scanf("%s",buffer);

		// rilascio token su semaforo 1 che dice che è disponibile un messaggio
		oper.sem_num = 1;
		oper.sem_op = 1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);

	}

	// Chiudi mem condivisa
	shmctl(descr_mem, IPC_RMID, NULL);
	semctl(descr_sem, 0, IPC_RMID, NULL);

}