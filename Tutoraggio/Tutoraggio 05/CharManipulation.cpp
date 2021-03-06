#include "pch.h"
#include <string.h>
#include <stdio.h>


int main(int argc, wchar_t* argv[])
{
	unsigned i;
	char str[] = "CIAO!";
	wchar_t wstr[] = L"CIAO!";

	printf("\n");
	printf("=================================================================================\n");
	printf("| BYTE0 | BYTE1 | BYTE2 | BYTE3 | BYTE4 | BYTE5 | BYTE6 | BYTE7 | BYTE8 | BYTE9 |\n");
	printf("=================================================================================\n");
	printf("\n\n");

	printf("PRINTING \"char\" STRING\n");
	printf("=================================================================================\n");
	printf("| CHAR0 | CHAR1 | CHAR2 | CHAR3 | CHAR4 | CHAR5 | CHAR6 | CHAR7 | CHAR8 | CHAR9 |\n");
	printf("=================================================================================\n");
	for (i = 0; i < strlen(str); i++)
		printf("|   %c   ", str[i]);
	printf("|\n");
	printf("=================================================================================\n");
	printf("\n\n");

	printf("PRINTING \"char\" STRING AFTER CAST TO \"wchar_t\"\n");
	printf("=================================================================================\n");
	printf("|     WCHR0     |     WCHR1     |     WCHR2     |     WCHR3     |     WCHR4     |\n");
	printf("=================================================================================\n");
	for (i = 0; i < (wcslen((wchar_t *)str) / 2) - 1; i++)
		printf("|   %c           ", ((wchar_t *)str)[i]);
	printf("|\n");
	printf("=================================================================================\n");
	printf("\n\n");

	printf("PRINTING \"wchar_t\" STRING\n");
	printf("=================================================================================\n");
	printf("|     WCHR0     |     WCHR1     |     WCHR2     |     WCHR3     |     WCHR4     |\n");
	printf("=================================================================================\n");
	for (i = 0; i < wcslen(wstr); i++)
		printf("|   %c           ", wstr[i]);
	printf("|\n");
	printf("=================================================================================\n");
	printf("\n\n");

	printf("PRINTING \"wchar_t\" STRING AFTER CAST TO \"char\"\n");
	printf("=================================================================================\n");
	printf("| CHAR0 | CHAR1 | CHAR2 | CHAR3 | CHAR4 | CHAR5 | CHAR6 | CHAR7 | CHAR8 | CHAR9 |\n");
	printf("=================================================================================\n");
	for (i = 0; i < wcslen(wstr) * 2; i++)
		printf("|   %c   ", ((char *)wstr)[i]);
	printf("|\n");
	printf("=================================================================================\n");
	printf("\n\n");

	printf("PRINTING \"wchar_t\" STRING AFTER CAST TO \"char\" AND MEASURING LENGTH in \"char\"\n");
	printf("=================================================================================\n");
	printf("| CHAR0 | CHAR1 | CHAR2 | CHAR3 | CHAR4 | CHAR5 | CHAR6 | CHAR7 | CHAR8 | CHAR9 |\n");
	printf("=================================================================================\n");
	for (i = 0; i < strlen((char *)wstr); i++)
		printf("|   %c   ", ((char *)wstr)[i]);
	printf("|\n");
	printf("=================================================================================\n");
	printf("\n\n");

	for (i = 1; i < wcslen(wstr) * sizeof(wstr[0]); i += 2)
		((char *)wstr)[i] = (char) '-';

	printf("PRINTING \"wchar_t\" STRING AFTER CAST TO \"char\" AND MEASURING LENGTH in \"char\"\n");
	printf("=================================================================================\n");
	printf("| CHAR0 | CHAR1 | CHAR2 | CHAR3 | CHAR4 | CHAR5 | CHAR6 | CHAR7 | CHAR8 | CHAR9 |\n");
	printf("=================================================================================\n");
	for (i = 0; i < strlen((char *)wstr); i++)
		printf("|   %c   ", ((char *)wstr)[i]);
	printf("|\n");
	printf("=================================================================================\n");
	printf("\n\n");

	return 0;
}