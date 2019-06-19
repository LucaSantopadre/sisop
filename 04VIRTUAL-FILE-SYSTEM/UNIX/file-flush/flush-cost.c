#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define CYCLES 10

char *string = "francesco";

int main (int argc, char *argv[]) {

	int fd;
	int i;

	if (argc!= 2) {
	printf("Syntax: prog <file_name>\n");
	exit(-1);
	}

	fd=open(argv[1], O_CREAT| O_TRUNC|O_WRONLY,
			S_IRUSR|S_IWUSR);
	if (fd== -1) {
		printf("Open error: %s\n", strerror(errno));
		exit(-2);
	}

	for(i=0; i<CYCLES; i++){
		write(fd,string,strlen(string));
#ifdef FLUSH //try running with this macro or not and then check execution times
		fsync(fd);
#endif
	}
}

