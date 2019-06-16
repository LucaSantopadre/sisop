#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N 8
#define W 100

static const char *filename = "file-lock.txt";

void main(int argc, char *argv[])
{
	int i;
	int fd;
	int len;
	int status;
	int p_created;
	pid_t pid;
	char text[10];
	struct flock fl;

	if ((fd = open(filename, O_CREAT|O_RDWR|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO)) == -1) {
		printf("Unable to create and open file %s\n", filename);
		exit(-1);
	}

	p_created = N;

	for (i=0; i<N; i++) {

		if ((pid = fork()) == -1) {

			printf("Unable to fork process\n");
			p_created--;

		} else if (pid == 0) {

			pid = getpid();
			len = sprintf(text, "%d ", (int)pid);

			fl.l_type = F_WRLCK;
			fl.l_whence = SEEK_SET;
			fl.l_start = 0;
			fl.l_len = 0;
			fl.l_pid = pid;

			if (fcntl(fd, F_SETLKW, &fl) == -1) {  
				printf("Unable to lock file %s\n", filename);
				exit(-1);
			}

			for (i=0; i<W; i++) {
				if (write(fd, text, (size_t)len) == -1) {
					printf("Unable to write on file\n");
				}
			}
			if (write(fd, "\n\n", 2) == -1) {
				printf("Unable to write on file\n");
			}

			fl.l_type = F_UNLCK;

			if (fcntl(fd, F_SETLKW, &fl) == -1) {  
				printf("Unable to unlock file %s\n", filename);
				exit(-1);
			}

			close(fd);
			exit(0);

		}

	}

	for (i=0; i<p_created; i++) {
		if (wait(&status) == -1) {
			printf("Wait has failed\n");
		}
	}

	close(fd);
	exit(0);
}