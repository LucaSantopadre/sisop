/**********************************************************************
 *	Soluzione all'ESERCIZIO numero 2 visto in aula                    *
 *                                                                    *
 *	Anche se non previste nel programma del corso di SO, nel resto    *
 *	del sorgente è stata introdotta della sintassi adeguata al fine   *
 *	di produrre un codice che abbia delle garanzie di correttezza.    *
 *                                                                    *
 *	Tali conoscenze non sono richieste! Ma possono fornire uno spunto *
 *	per chi è interessato ad approfondire l'argomento in maniera      *
 *	autonoma.                                                         *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#define NUM_THREADS 50
#define NUM_MESSAGES 10


struct message_group {
	unsigned int level;
	char *message[NUM_MESSAGES];
	/*********************************************************************
	 *	Con l'identificatore "volatile" indichiamo al compilatore di non *
	 *	generare ottimizzazioni nel codice ogni qualvolta intendiamo     *
	 *	leggere o scrivere il valore contenuto nel puntatore "others"    *
	 *	perché potrebbe venir ad essere modificato da qualche altro      *
	 *	thread in esecuzione concorrente.                                *
	 *********************************************************************/
	volatile struct message_group *others;
	/*********************************************************************
	 *	Con l'attributo "aligned" chiediamo al compilatore di allocare   *
	 *	ogni istanza della struttura dati "message_group" alla massima   *
	 *	taglia per l'alineamento delle strutture e delle variabili per   *
	 *	la specifica architetura per cui stiamo compilando.              *
	 *                                                                   *
	 *	In caso contrario non è garantito che le lettura/scritture nella *
	 *	specifica istanza della struttura dati siano atomiche in quanto  *
	 *	il dato potrebbe ricadere su linee di cache differenti.          *
	 *                                                                   *
	 *	In altre architetture questo approccio potrebbe addirittura non  *
	 *	essere sufficiente in quanto la lettura scrittura di un          *
	 *	indirizzo di memoria potrebbe essere non atomico e generare      *
	 *	delle data-race.                                                 *
	 *                                                                   *
	 *	Una soluzione alternativa risiede nell'utilizzare le operazioni  *
	 *	atomiche offerte dalle build-in del tool di compilazione "gcc"   *
	 *	che producono del codice adeguato a gestire le letture scritture *
	 *	in maniera atomica anche sotto le condizioni prima discusse.     *
	 *********************************************************************/
} __attribute__ ((aligned));


struct message_group *group = NULL;


void * write_message(void *arg)
{
	char message[40];
	unsigned long long int i;
	volatile struct message_group *tmp1;
	volatile struct message_group *tmp2;

	tmp1 = group;
	i = (unsigned long long int) arg;
	sprintf(message, "I'm the thread number %llu", i);

	while (1)
	{
		if ( i < (NUM_MESSAGES * tmp1->level) )
		{
			tmp1->message[i % NUM_MESSAGES] = strdup(message);
			pthread_exit(NULL);
		}
		else if (tmp1->others == NULL)
		{
			if ( i == (NUM_MESSAGES * tmp1->level) )
			{
				tmp2 = (struct message_group *) calloc(1, sizeof(struct message_group));
				tmp2->level = tmp1->level + 1;
				tmp1->others = tmp2;
				tmp1 = tmp1->others;
			}
		}
		else
		{
			tmp1 = tmp1->others;
		}
	}
}


int main (void)
{
	void *res;
	unsigned long long int i;
	volatile struct message_group *tmp1;
	volatile struct message_group *tmp2;
	pthread_t thread[NUM_THREADS];

	group = (struct message_group *) calloc(1, sizeof(struct message_group));
	group->level = 1;

	for (i=0; i<NUM_THREADS; i++)
		if (pthread_create(&thread[i], NULL, write_message, (void *) i))
			return 1;

	for (i=0; i<NUM_THREADS; i++)
		pthread_join(thread[i], &res);

	tmp1 = group;

	while (tmp1)
	{
		for (i=0; i<NUM_MESSAGES; i++)
			printf("%s\n", tmp1->message[i]);
		tmp2 = tmp1;
		tmp1 = tmp2->others;
		free((void *) tmp2);
	}

	return 0;
}