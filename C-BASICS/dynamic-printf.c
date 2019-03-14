#include <stdio.h>

char format_line[4096];

unsigned long x = 68361982; 

int main(int a, char** b){

	while(1){
		scanf("%s",format_line);
		printf(format_line,x);
		printf("\n");

	}
}
