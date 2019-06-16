#include "pch.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>


#define N_PAGES 1024


static int page_size;
static int page_bitmap[N_PAGES];

static void * pages;


static int init_memory_allocator(void)
{
	int i;
	SYSTEM_INFO info;

	GetSystemInfo((LPSYSTEM_INFO) &info);
	page_size = (int) info.dwPageSize;

	for (i = 0; i < N_PAGES; i++)
		page_bitmap[i] = 0;

	if ((pages = VirtualAlloc(NULL, (SIZE_T) (N_PAGES * page_size), MEM_COMMIT, PAGE_NOACCESS)) == NULL)
		return -1;

	return 0;
}


static int fini_memory_allocator(void)
{
	return (VirtualFree(pages, (SIZE_T) (N_PAGES * page_size), MEM_DECOMMIT)) ? 0 : -1;
}


int main(int argc, char *argv[])
{
	unsigned i, j, found, old_protect;
	unsigned answer, first, contiguous;

	if (init_memory_allocator() == -1)
	{
		printf("[EROOR] - Unable to initialize the Memory Allocator. Exit.\n");
		return -1;
	}

	while (1)
	{
		printf("\nEseguire una delle seguenti operazioni.\n");
		printf("\t[1] - Richiesta pagine contigue\n");
		printf("\t[2] - Liberare pagine contigue\n");
		printf("\t[3] - Genera il Dump della Bitmap\n");
		printf("\t[4] - Uscire\n");

		printf("\nInserire il numero corrispondente: ");

		if (scanf_s("%u", &answer) == 0)
		{
			printf("[WARNING] - Non valid input. Retry.\n");
			continue;
		}

		if (answer == 1)
		{
			printf("\nInserire il numero di pagine contigue desiderate: ");

			if (scanf_s("%u", &contiguous) == 0 || contiguous == 0 || contiguous > N_PAGES)
			{
				printf("[WARNING] - Non valid input. Retry.\n");
				continue;
			}

			found = 0;

			for (i = 0; i < (N_PAGES - contiguous); i++)
			{
				if (page_bitmap[i] == 0)
				{
					found = 1;

					for (j = 0; j < contiguous; j++)
					{
						if (page_bitmap[i + j] == 1)
						{
							found = 0;
							i = i + j + 1;
							break;
						}
					}

					if (found) break;
				}
			}

			if (found)
			{
				if (VirtualProtect((LPVOID) (((unsigned long long int) pages) + (i * page_size)), (SIZE_T) (contiguous * page_size), PAGE_READWRITE, (PDWORD) &old_protect) == 0)
				{
					printf("[WARNING] - Unable to update page access mode.\n");
					continue;
				}

				for (j = i; j < (i + contiguous); j++)
					page_bitmap[j] = 1;

				printf("[INFO] - %u contiguous pages have been allocated.\n", contiguous);
			}
			else
			{
				printf("[INFO] - There are no %u contiguous free pages.\n", contiguous);
				continue;
			}
		}
		else if (answer == 2)
		{
			printf("\nInserire l'indice della prima pagina da liberare: ");

			if (scanf_s("%u", &first) == 0 || first >= N_PAGES)
			{
				printf("[WARNING] - Non valid input. Retry.\n");
				continue;
			}

			if (page_bitmap[first] == 0)
			{
				printf("[INFO] - Page at index %u was not allocated.\n", first);
				continue;
			}

			printf("Inserire il numero massimo di pagine contigue da liberare: ");

			if (scanf_s("%u", &contiguous) == 0 || contiguous == 0 || (first + contiguous) > N_PAGES)
			{
				printf("[WARNING] - Non valid input. Retry.\n");
				continue;
			}

			found = 1;

			for (i = first; i < (first + contiguous); i++)
			{
				if (page_bitmap[i] == 0)
				{
					found = 0;
					break;
				}
			}

			if (found)
			{
				if (VirtualProtect((LPVOID) (((unsigned long long int) pages) + (first * page_size)), (SIZE_T) (contiguous * page_size), PAGE_NOACCESS, (PDWORD) &old_protect) == 0)
				{
					printf("[WARNING] - Unable to update page access mode.\n");
					continue;
				}

				for (i = first; i < (first + contiguous); i++)
					page_bitmap[i] = 0;

				printf("[INFO] - %u contiguous pages have been freed.\n", contiguous);
			}
			else
			{
				printf("[INFO] - There are less than %u allocated pages starting from index %u.\n", contiguous, first);
				continue;
			}
		}
		else if (answer == 3)
		{
			printf("\n");
			for (i = 0; i < N_PAGES; i++)
				printf("%d", page_bitmap[i]);
			printf("\n");
		}
		else if (answer == 4)
		{
			break;
		}
		else
		{
			printf("[WARNING] - The operation requested does not exists. Retry.\n");
			continue;
		}
	}

	if (fini_memory_allocator() == -1)
	{
		printf("[EROOR] - Unable to finalize the Memory Allocator. Exit.\n");
		return -1;
	}
}