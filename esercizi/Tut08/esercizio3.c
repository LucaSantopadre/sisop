#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int N=10;
int W=50;

int main(int argc, char* argv[]){

	int status;
	int dd;
	char *buffer[W];
	char text[10];
	int ww;
	int len;
	struct flock fl;

	// apertura file

	dd = open("prova.txt",O_CREAT | O_WRONLY| O_TRUNC, 0666);
	if(dd==-1){
		printf("Impossibile aprire il file\n");
		exit(-1);
	}

	// spawn N processi
	pid_t pid;
	int j;
	for(j=0; j<N; j++){
		pid = fork();

		// figlio
		if(pid==0){
			
			// lock del file
			fl.l_type = F_WRLCK;
			fl.l_whence = SEEK_SET;
			fl.l_start = 0;
			fl.l_len = 0;
			fl.l_pid = pid;
			fcntl(dd, F_SETLKW, &fl);

			len = sprintf(text, "%d", (int)getpid()); // in text c'è una strtinga con il pid
			printf("pid %d\n",getpid());

			int x;
			for(x=0; x<W; x++){
				ww = write(dd, text, (size_t)len);
				//printf("%s\n",text);
				if(ww==-1){
					printf("Impossibile scrivere sul file\n");
					exit(-1);
				}
				write(dd, ",", 1);
			}
			write(dd, "\n\n", 2);
			
			close(dd);
			exit(0);
		}
	}


	// padre goes in wait
	wait(&status);





	/*
	int i;
	for(i=0;i<W;i++){
		buffer[i]=getpid();
	}
	*/



	
	exit(0);

}