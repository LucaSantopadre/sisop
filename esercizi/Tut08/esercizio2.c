#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024


int len;

int main(int argc, char* argv[]){
	int status;
	char *buf_read;

	int d = open("prova.txt",O_CREAT | O_RDWR |O_TRUNC,0666);
	printf("file aperto : %d\n",d);

	pid_t pid;

	

	pid = fork();

	if(pid==0){
		printf("processo figlio, inserisci una stringa \n");
		char *buf;
		scanf("%m[^\n]",&buf);

		len=strlen(buf);
		int res = write(d,buf,len);
		printf("file scrivo : %d\n",res);
		close(d);
		sleep(3);
		exit(0);
	}
	printf("Processo padre in wait . . .\n");
	wait(&status);

	//d = open("prova.txt",O_CREAT | O_RDONLY,0666);
	lseek(d,0,SEEK_SET);
	read(d,buf_read,BUFSIZE);
	printf("LETTURA : %s\n",buf_read);
	

	

		
	close(d);

	
}