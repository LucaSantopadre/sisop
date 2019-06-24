#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void procedura1(){

	char *buffer = malloc(128 * sizeof(char));
	char buffer_lettura[128];
	int fd ;
	int byte_scritti;
	int byte_letti;

	

	// apro in scrittura
	fd = open("esempio.txt", O_CREAT | O_TRUNC | O_WRONLY, 0666);

	printf("Inserisci stringhe , 'quit' per terminare l'inserimento \n");
	while(1){
		
		scanf("%s",buffer);

		if(strcmp(buffer, "quit") == 0){
			break;
		}

		//scrivo
		byte_scritti = write(fd, buffer, strlen(buffer));
		write(fd, "\n", 1);
	}

	// chiudo
	close(fd);


	// apro in lettura
	fd = open("esempio.txt", O_RDONLY, 0666);

	// leggo
	printf("Leggo 128 byte dal file. . . \n");
	byte_letti = read(fd, buffer_lettura, 128);
	printf("%s\n",buffer_lettura);

	close(fd);


}



void procedura2(){

	char *buffer = malloc(128 * sizeof(char));
	char buffer_lettura[128];
	FILE *file;
	int char_scritti;
	int char_letti;

	

	// apro in scrittura
	file = fopen("esempio.txt", "w+"); //  w+      │ O_RDWR | O_CREAT | O_TRUNC 

	printf("Inserisci stringhe , 'quit' per terminare l'inserimento \n");
	while(1){
		
		scanf("%s",buffer);

		if(strcmp(buffer, "quit") == 0){
			break;
		}

		//scrivo
		char_scritti = fprintf(file, "%s",  buffer);
		fprintf(file, "%s", "\n");
	}

	// chiudo
	fclose(file);


	// apro in lettura
	file = fopen("esempio.txt", "r"); //  r       │ O_RDONLY   

	// leggo
	printf("Leggo dal file. . . \n");
	while( char_letti = fscanf(file,"%s", buffer_lettura)  != EOF ){
		printf("%s\n",buffer_lettura);
	}
	
	fclose(file);
}




int main(int argc, char* argv){
	int scelta;

	while(1){
		printf("\n\n\n*********************************\n\n");
		printf("Scegli 1 o 2 \n");
		printf("1) open      write      read\n");
		printf("2) fopen    fprintf    fscanf\n");
		scanf("%d",&scelta);
		printf("\n");

		if(scelta == 1){
			procedura1();
			continue;
		}

		if(scelta == 2){
			procedura2();
			continue;
		}
	}

}