#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	BOOL newprocess;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD exitcode;

	char text[1024];

	size_t slength;

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);

	if (argc == 1) {
		slength = strlen(argv[0]);
		memcpy(text, argv[0], slength);
		text[slength] = ' ';

		printf("Enter any string: ");
		scanf_s("%[^\n]", &text[slength + 1], (unsigned)1022-slength);

		newprocess = CreateProcessA((LPCSTR)argv[0], (LPSTR)text, NULL, NULL, FALSE,
						NORMAL_PRIORITY_CLASS, NULL, NULL, (LPSTARTUPINFOA)&si, &pi);

		if (newprocess == FALSE) {
			printf("Unable to create new process\n");
			ExitProcess(-1);
		}

		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &exitcode);
		printf("Process with PID=%d has finished\n", GetCurrentProcessId());
		ExitProcess(0);
	}
	else
	{
		int i;
		printf("Process with PID=%d inhered string: ", GetCurrentProcessId());
		for (i = 0; i < argc; i++)
			printf("%s ", (LPSTR)argv[i]);
		printf("\n");
		ExitProcess(0);
	}
}