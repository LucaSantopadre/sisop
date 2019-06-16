#include "pch.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>


#define BUFFER_SIZE 1024


__declspec(align(64)) volatile char * buffer;


BOOL ctrl_break_handler(DWORD ctrl_type)
{
	char * tmp;

	if (ctrl_type == CTRL_BREAK_EVENT)
	{
		tmp = (char *)InterlockedExchangePointer((PVOID volatile *)&buffer, NULL);

		printf("\nStringa consumata: %s\n", tmp);

		free(tmp);

		return true;
	}
	
	return false;
}


int main(int argc, TCHAR* argv[])
{
	if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrl_break_handler, true) == 0)
		return -1;

	while (1)
	{
		if ((buffer = (char *)malloc(BUFFER_SIZE)) == NULL)
			return -1;

		printf("\nInserisci una nuova stringa: ");
		scanf_s("%[^\n]", buffer, BUFFER_SIZE);
		getchar();

		GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, 0);

		while (buffer != NULL) ;
	}

	return 0;
}