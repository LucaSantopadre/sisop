#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	if(argc<2){
		printf("Usage: command string\n");
		exit(1);
	}

	int len = strlen(argv[1]);
	printf("lunghezza stringa %d\n",len);
	char s[len];
	memcpy(s, argv[1], len);
	printf("stringa copiata: %s\n",s);


	memset(s,'a',len);
	printf("buffer azzerato: %s\n",s);

}