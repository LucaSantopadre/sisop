#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>


pid_t pid_figlio;

void fatto(int sign_num){

	sleep(2);

}



int main(int argc, char* argv[]){

	pid_t pid;
	int status;

	signal(SIGUSR1, fatto);


	pid=fork();
	if(pid == -1){
		printf("errore non è possibile eseguire la fork");
		exit(-1);
	}

	// processo figlio
	if(pid == 0){
		printf("PADRE  %d, FIGLIO %d\n", getppid(), getpid());
		pid_figlio = getpid();
		for (int i = 0; i < 5; ++i)
		{
			printf("dio\n");
			kill(getppid(),SIGUSR1);
			pause();
		}
		exit(0);
	}
	
	//processo padre
	for (int i = 0; i < 5; ++i)
	{
		pause();
		printf("can\n");
		kill(pid_figlio,SIGUSR1);
		
	}

	wait(&status);
	exit(0);


}
