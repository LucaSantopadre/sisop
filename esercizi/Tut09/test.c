#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char* argv[]){
	int page_size = getpagesize();
	int dim_pool=2;
	printf("Linux page size: %d\n" , page_size);


	char* buffer = mmap(NULL, dim_pool * page_size, PROT_READ| PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
	if(buffer == MAP_FAILED)
		return -1;
	scanf("%s",buffer);
	printf("buffer: %p\n" , buffer);




	char *str;
	int len;
	//scanf("%s",str);
	str =  buffer;
	
	len = strlen(str);
	//str="ciaoddd";
	//str[0]='i';

	printf("str:    %p\n\n" , str+10);

	if(munmap(buffer, 4096) == -1){
		printf("IMPOSSSIBILE DEMAPPARE\n");
		exit(-1);
	}


	printf("buffer: %s   , stlen %d\n",buffer,len);
	printf("str   : %c   , stlen %d\n",str[1],len);


	



}