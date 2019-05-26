#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#define timeout 5 

void wakeup(int sgnumber){
	printf("I'm awake\n");
	fflush(stdout);
	alarm(timeout);
}

int main (int argc, char**argv){

	signal(SIGALRM, wakeup);
	alarm(timeout);
	while (1){
		pause();	
	}	

}
