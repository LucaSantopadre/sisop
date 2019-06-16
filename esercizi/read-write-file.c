#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fd;
	int result;
	char buffer[1024];
	char *s;
	int len;

	if(argc != 2){
		printf("Usage: prog filename\n");
		exit(1);
	}

	printf("Inserisci stringa da scrivere nel file\n");
	scanf("%s",&s);
	printf("Stringa inserita: %s\n",&s);
	len = strlen(s);

	fd = open(argv[1],O_WRONLY);
	if(fd == -1){
		printf("Errore nell'apertura file\n");
		exit(1);
	}
	int i;
	//for(i=0;i<len;i++){
		write(fd,&s,2);
	close(fd);

}

