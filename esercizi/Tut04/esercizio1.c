#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	char s[128];
	pid_t p;
	int exit_code;

	printf("Inserisci una stringa\n");
	scanf("%s",s);

	p = fork();

	if(p!=0){
		printf("PADRE my pid is %d\n",getpid());
		wait(&exit_code);
		printf("EHI SONO IL PADRE :%d  figlio uscito%d\n",getpid(),exit_code);
		exit(0);

	}else{
		printf("FIGLIO %d Stringa %s\n",getpid(),s);
		sleep(5);
		exit(1);
	}
	


}