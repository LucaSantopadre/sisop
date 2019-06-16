#include <stdio.h>

#ifdef MMCP
#include <string.h>
#endif

#include <stdlib.h>

int main()
{
	int buff_size = 0;
	char *dyn_buff;

	scanf("%m[^\n]", &dyn_buff);
	
	while (dyn_buff[buff_size] != '\0')
		buff_size++;

	printf("\nStringa inserita nel buffer allocato dinamicamente:  %s\n", dyn_buff);
	printf("\nTaglia del buffer allocato dinamicamente:  %d\n", buff_size);

	char stk_buff[buff_size+1];

#ifdef MMCP
	memcpy(stk_buff, dyn_buff, buff_size);
#else
	int i;
	for (i=0; i<buff_size; i++)
		stk_buff[i] = dyn_buff[i];
#endif
	stk_buff[buff_size] = '\0';

	free(dyn_buff);

	printf("\nStringa copiata nel buffer allocato sullo stack:  %s\n\n", stk_buff);
}