#include <string.h>
#include <stdio.h>

int main()
{
	char *token1 = NULL;
	char *token2 = NULL;
	char *token3 = NULL;
	char buff[512];
	char *delim = "_";

	scanf("%[^\n]", buff);

	if ((token1 = strtok(buff, delim)) == NULL)
		return 1;
	if ((token2 = strtok(NULL, delim)) == NULL)
		return 1;
	if ((token3 = strtok(NULL, delim)) == NULL)
		return 1;

	printf("Token1: %s\n", token1);
	printf("Token2: %s\n", token2);
	printf("Token3: %s\n", token3);

	int index = 0;
	int num_delims = 0;
	while (1)
	{
		if ((token1[index] == '\0'))
		{
			if ((++num_delims == 3))
				break;
			putchar((int) ' ');
		}
		else
			putchar((int) token1[index]);
		index++;
	}
	putchar((int) '\n');

	return 0;
}