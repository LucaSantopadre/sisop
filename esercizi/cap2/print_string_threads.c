#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

char *buffer[128];


void* print_string(){
	pthread_t t_id;
	t_id = pthread_self();
	printf("%ld  Figlio\n\n\n",t_id);

	printf("%s\n",buffer);
	printf("fine thread figlio\n");
	sleep(10);
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	printf("Inserisci una stringa\n");
	scanf("%[^\n]",buffer);


	pthread_t id;
	void **status;

	pthread_create(&id,NULL,print_string,NULL);


	pthread_t t_id;
	t_id = pthread_self();
	printf("%ld  Padre\n\n\n",t_id);
	sleep(10);
	pthread_join(id,status);
	
	printf("fine thread padre\n");
}
