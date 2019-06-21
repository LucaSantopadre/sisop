#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096


int produttore(key_t k){
	printf("PRODUTTORE\n");

	int descr;

	descr = shmget(k, PAGE_SIZE, IPC_CREAT|IPC_EXCL|0666);

	if(descr == -1){
		printf("impossibile creare area condivisa\n");
		exit(-1);
	}
	printf("descrittore %d\n",descr);

	void* addr;
	addr = shmat(descr, NULL, 0);
	//printf("indirizzo %p\n",addr);

	printf("Inserisci un messaggio \n");
	scanf("%s",(char*)addr);
	printf("\n\n");

	exit(0);

}


int consumatore(key_t k){
	printf("CONSUMATORE\n");


	int descr;

	descr = shmget(k, PAGE_SIZE, 0);

	if(descr == -1){
		printf("impossibile creare area condivisa\n");
		exit(-1);
	}
	printf("descrittore %d\n",descr);


	
	char* addr;
	addr = shmat(descr, NULL, 0);
	//addr = malloc(128*sizeof(char));

	printf("MESSAGGIO scritto da produttore %s\n",addr);


	//CHIUDI TUTTO
	shmctl(descr, IPC_RMID, NULL);

}

int main(int argc, char* argv[]){

	pid_t pid;
	key_t key = 14;
	int status;

	// fork-a
	pid = fork();

	if(pid == 0){
		produttore(key);
	}
	wait(&status);
	consumatore(key);

	exit(0);



}