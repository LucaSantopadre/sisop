#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "mini-tar.h"


/*
#define BUFSIZE 1024
#define MAX_MANAGEABLE_FILES 128
#define MAX_FILE_NAME_LENGTH 128

typedef struct _header{
	int  num_files;
	char file_names[MAX_MANAGEABLE_FILES][MAX_FILE_NAME_LENGTH];
	int  start_position[MAX_MANAGEABLE_FILES];
	int  end_position[MAX_MANAGEABLE_FILES];
} header;
int descriptors[MAX_MANAGEABLE_FILES];
*/

header tar_head;

int main(int argc, char *argv[]) { 
	int dd, size, result; 
	int i;
	off_t seek;
	off_t file_start, file_end;
	char c;

	 if (argc < 3) { /* check the number of arguments */ 

		printf("usage: extract archive file\n");
		exit(1);
	 }  


	 /* destination file creation */
	 dd=open(argv[1],O_RDONLY); 
	 if (dd == -1) {
		printf("archive open error\n");
	        exit(1);
	 }

	result = read(dd,&tar_head,sizeof(tar_head));

	if (result != sizeof(tar_head)){
	   printf("unable to read header in archive %s\n",argv[1]);
	   return -1;
	}

	for (i=0;i<MAX_MANAGEABLE_FILES;i++){

	  if(strcmp(argv[2], tar_head.file_names[i])==0){
	    printf("File %s - found in the archive\n",argv[2]);
	    file_start = tar_head.start_position[i];
	    file_end = tar_head.end_position[i];
	    printf("File %s - found in the archive - position is [%d,%d]\n",argv[2],file_start,file_end);
	 
	    seek = lseek(dd,file_start,SEEK_SET);

	    printf("seek position for file %s is %d\n",argv[2],seek);

	//please optimize here and put the possibility to extract to a file rather than stdout
	    for(;seek<file_end;seek++){
		read(dd,&c,1);
		write(1,&c,1);

	    }
		

	   return 0;
	  }	
	}

	printf("file %s not found in the archive\n",argv[2]);
  }/* end main*/

