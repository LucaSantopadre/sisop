#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	char *s;

	printf("Inserisci una stringa\n");

	scanf("%m[^\n]",&s);

	printf("stringa nell' heap: %s \n",s);

	char stack_s[strlen(s)];
	memcpy(stack_s,s,strlen(s));
	free(s);

	printf("stringa nello stack: %s \n",stack_s);

}