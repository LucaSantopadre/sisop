#include "pch.h"
#include <windows.h>
#include <stdio.h>


volatile DWORD shared_variable = 1;


DWORD WINAPI thread_function(LPVOID lpParam)
{
	printf("2. Child Thread is executing.\n");
	Sleep(5000);
	printf("3. Child Thread is terminating.\n");
	shared_variable = 0;
	ExitThread(0);
}


int main()
{
	DWORD hId;
	// DWORD exitCode;
	HANDLE hThread;
	printf("1. Parent Thread is executing.\n");
	if (!(hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_function, NULL, 0, &hId))) {
		printf("Unable to create a new thread. Exit.\n");
		ExitProcess(1);
	}
	else {
		while (shared_variable);
		printf("4. Parent Thread is terminating.\n");
		// GetExitCodeThread(hThread, & exitCode);   /* POTREBBE NON RITORNARE IL VALORE ATTESO ... PERCHè ? */
	}
	ExitProcess(0);
}