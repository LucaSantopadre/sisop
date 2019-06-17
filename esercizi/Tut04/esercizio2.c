#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int N=3;
	int residual_forks = N;
	int i=0;
	char lista[N][4096];

	int forkati=0;
	int status;

	printf("Inserisci %d directory valide:\n",N);
	for(i=0;i<N;i++){
		scanf("%s",&lista[i]);
	}
	
	pid_t p;
	printf("PADRE %d\n\n",getpid());

	i=0;
	while(residual_forks>0){

		p=fork();

		if(p==0){
			printf("STO STAMPANDO %s\n",&lista[i]);
			printf("FIGLIO: %d    padre %d\n",getpid(),getppid());
			execlp("ls","ls",&lista[i],NULL);
			exit(0);
		}
		sleep(5);
		i++;
		forkati++;
		residual_forks--;
	}

	int j;
	for(j=0;j<forkati;j++){
		printf("PADRE in wait . . . %d\n",getpid());
		wait(&status);
	}
	
}