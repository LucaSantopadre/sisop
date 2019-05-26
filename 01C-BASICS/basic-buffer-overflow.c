#include <stdio.h>

int v[10];
int control_variable;

int main(int argc, char * argv[]){

	int index, value;

#ifdef IN_STACK
	int v[10];
	int control_variable;


#endif

	control_variable = 1;
	
	printf("v:                   %p\n",v);
	printf("indirizzo v[10]:     %p\n",&(v[10]));
	printf("indirizzo control_v: %p\n",&control_variable);



	while (control_variable == 1){
		scanf("%d%d",&index,&value);
		v[index] = value;
		printf("array elem at position %d has been set to value %d\n",index,v[index]);


	}

	printf("exited cycle\n");

	return 0;
}

