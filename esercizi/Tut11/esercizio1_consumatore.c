#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define PAGE_SIZE 4096

int main(int argc, char*argv[]){
	int descr_mem;
	int descr_sem;
	key_t key_mem = 205;
	key_t key_sem = 305;
	char *buffer;

	// creo area mem condivisa
	descr_mem = shmget(key_mem,PAGE_SIZE, 0);
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
	descr_sem = semget(key_sem, 2, 0);
	if(descr_sem == -1){
		printf("Impossibile creare semaforo con key %d\n",key_sem);
		//exit(-1);
	}
	printf("Descrittore  semaforo %d\n", descr_sem);

	// inizializzo semaforo
	struct sembuf oper;


	while(1){
		sleep(3);
		// chiedo token su semaforo 1 che regola la lettura
		oper.sem_num = 1;
		oper.sem_op = -1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);

		// token ricevuto , leggo
		printf("Token per leggere ricevuto,  messaggio\n");
		printf("%s",buffer);
		printf("\n");

		// rilascio token su semaforo 1 che ho letto il messaggio
		oper.sem_num = 0;
		oper.sem_op = 1;
		oper.sem_flg = 0;
		semop(descr_sem, &oper, 1);

	}

	// Chiudi mem condivisa
	shmctl(descr_mem, IPC_RMID, NULL);
	semctl(descr_sem, 0, IPC_RMID, NULL);

}