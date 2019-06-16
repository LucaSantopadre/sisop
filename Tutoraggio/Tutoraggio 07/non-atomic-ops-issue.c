#include <stdio.h>
#include <pthread.h>


/* Compilare su una macchina dotata di almeno due CPU-core.

   Compilare il sorgente con il flag -O0 per indicare al compilatore
   di non generare ottimizzazioni nel codice.

   Compilare sia con -DCROSS_CACHE_LINE definito che non ed osservare
   cosa succede nei due casi. */


#define VAL1	15ULL
#define VAL2	64424509440ULL


void * foo1 (void *arg)
{
	unsigned long long int local;
	unsigned long long int *cross_cl_boundary;

	cross_cl_boundary = (unsigned long long int *) arg;

	while (1)
	{
		local = (*cross_cl_boundary);
		if (local != VAL1 && local != VAL2)
			printf("Errore dovuto ad operazione non-atomica. Valore letto: %llu\n", local);
		(*cross_cl_boundary) = VAL1;
	}
}


void * foo2 (void *arg)
{
	unsigned long long int local;
	unsigned long long int *cross_cl_boundary;

	cross_cl_boundary = (unsigned long long int *) arg;

	while (1)
	{
		local = (*cross_cl_boundary);
		if (local != VAL1 && local != VAL2)
			printf("Errore dovuto ad operazione non-atomica. Valore letto: %llu\n", local);
		(*cross_cl_boundary) = VAL2;
	}
}


int main (void)
{
	int i;
	void *res;

	pthread_t thread1;
	pthread_t thread2;

	/* Ci prendiamo sufficiente spazio all'interno dallo stack
	   nel cui mezzo andremo a pescare l'indirizzo che separa
	   due linee di cache.  */
	char stackspace[256];

	unsigned long long int *cross_cl_boundary;

#ifdef CROSS_CACHE_LINE
	/* Prendiamo un indirizzo buono per spezzare un "unsigned long long int" su due linee di cache differenti. */
	cross_cl_boundary = (unsigned long long int *) ((((unsigned long long int) &stackspace[128]) & (~63ULL)) - 4ULL);
#else
	/* Prendiamo un indirizzo tale che un "unsigned long long int" risiede nella stessa linea di cache. */
	cross_cl_boundary = (unsigned long long int *) ((((unsigned long long int) &stackspace[128]) & (~63ULL)) - 20ULL);
#endif

	(*cross_cl_boundary) = VAL1;

	pthread_create(&thread1, NULL, foo1, (void *) cross_cl_boundary);
	pthread_create(&thread2, NULL, foo2, (void *) cross_cl_boundary);

	pthread_join(thread1, &res);
	pthread_join(thread2, &res);

	return 0;
}