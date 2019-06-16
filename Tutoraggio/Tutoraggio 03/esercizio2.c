#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char c;
	int i, j;
	char buff[21];

	if (argc < 2)
	{
		printf("Nessuna stringa passata come argomento.\n");
		return 0;
	}

	for (i=0; i<20; i++)
	{
		if (argv[1][i] == '\0')
			break;
		buff[i] = argv[1][i];
	}
	buff[i] = '\0';

	printf("%s\n", buff);

	for (j=0; j<(i/2); j++)
	{
		c = buff[i-j-1];
		buff[i-j-1] = buff[j];
		buff[j] = c;
	}

	printf("%s\n", buff);

	/* Reset del Buffer */
	memset(buff, 0, 21);
}