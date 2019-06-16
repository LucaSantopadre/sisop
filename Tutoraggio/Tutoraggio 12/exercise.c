#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define N_ROUND	3

#define N_WRITER	3
#define N_READER	8

#define SHMEM_MSG_KEY	6832
#define SHMEM_MSG_SIZE	4096

#define SHMEM_PID_KEY	6833
#define SHMEM_PID_SIZE	(N_WRITER * sizeof(pid_t))

#define SEMAPHORE_KEY	6842


union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};


void writer_function(void);
void reader_function(void) __attribute__((noreturn));


int pidid;
int msgid;
int semid;

int idx_writer;

char * message;
pid_t * pid_writer;


void sigusr1_handler(int sig)
{
	writer_function();
	kill(pid_writer[((idx_writer + 1) % N_WRITER)], SIGUSR1);
}

void reader_function(void)
{
	struct sembuf buf[1];

	/*
	 * Inizialmente i Lettori devono attendere alla barriera
	 * imposta dal semaforo ad indice 2 in maniera tale che sia
	 * impostato al valore corretto durante la normale esecuzione
	 * del protocollo di sincronizzazione contenuta nel ciclo while.
	 */
	buf[0].sem_num = 2;
	buf[0].sem_op = -1;
	buf[0].sem_flg = 0;

	semop(semid, buf, 1);

	while (1)
	{
		/*
		 * Attesa del Lettore sul semaforo ad indice 1, in attesa del
		 * completamento delle attività dello Scrittore che segnalerà
		 * il suo completamento su questo semaforo.
		 */
		buf[0].sem_num = 1;
		buf[0].sem_op = -1;
		buf[0].sem_flg = 0;

		semop(semid, buf, 1);

		printf("Il Lettore con PID %d ha letto il messaggio: %s\n", getpid(), (char *) message);

		/*
		 * Ogni lettore segnala il completamento della sua attività di
		 * lettura allo Scrittore di turno per mezzo del semaforo ad
		 * indice 0, il quale verrà sbloccato solo al completamento di
		 * tutte le letture.
		 */
		buf[0].sem_num = 0;
		buf[0].sem_op = 1;
		buf[0].sem_flg = 0;

		semop(semid, buf, 1);

		/*
		 * Questa barriera, imposta ai Lettori per mezzo del semaforo ad
		 * indice 2, impedisce ad uno stesso lettore di effettuare molteplici
		 * letture dello stesso messaggio consumando più volte il gettone
		 * destinato ad altri lettori. Sarà lo Scrittore a sbloccare tutti
		 * i lettori bloccati su questo semaforo non appena avrà garanzia
		 * che ognuno di essi abbia completato.
		 */
		buf[0].sem_num = 2;
		buf[0].sem_op = -1;
		buf[0].sem_flg = 0;

		semop(semid, buf, 1);
	}
}

void writer_function(void)
{
	struct sembuf buf[1];

	/*
	 * Lo Scrittore di turno si mette in attesa del completamento
	 * delle operazioni di lettura di ogni singolo Lettore e, una
	 * volta sbloccato (tutti i lettori hanno letto)...
	 */
	buf[0].sem_num = 0;
	buf[0].sem_op = -N_READER;
	buf[0].sem_flg = 0;

	semop(semid, buf, 1);

	/*
	 * ...sblocca i lettori dalla barriera imposta dal semaforo con
	 * indice 2 al fine di riabilitare i lettori per la lettura di
	 * un nuovo messaggio che lo Scrittore andrà a scrivere a breve.
	 * In questo modo ogni lettore ha la possibilità di leggere ogni
	 * nuovo messaggio, una ed una sola volta.
	 */
	buf[0].sem_num = 2;
	buf[0].sem_op = N_READER;
	buf[0].sem_flg = 0;

	semop(semid, buf, 1);

	printf("\nLo Scrittore con PID %d chiede di inserire un messaggio: ", getpid());
	if (scanf("%[^\n]", message) == 0)
		(*message) = '\0';
	getc(stdin);

	/*
	 * Qui segnaliamo a tutti i Lettori, per mezzo del semaforo ad indice 1,
	 * che lo Scrittore di turno ha completato le attività di scrittura di
	 * un nuovo messaggio.
	 */
	buf[0].sem_num = 1;
	buf[0].sem_op = N_READER;
	buf[0].sem_flg = 0;

	semop(semid, buf, 1);
}

int main()
{
	int i;
	int ret;

	pid_t pid;
	pid_t pid_reader[N_READER];

	union semun sem_arg;

	struct sembuf buf[1];

	sigset_t set;
	sigset_t old_set;

	struct sigaction act;
	struct sigaction old_act;

	ret = 1;

	/*
	 * Setup della memoria condivisa utilizzata dai
	 * Lettori e dagli Scrittori per leggere e scrivere
	 * i messaggi.
	 */
	if ((msgid = shmget(SHMEM_MSG_KEY, SHMEM_MSG_SIZE, IPC_CREAT|IPC_EXCL|0666)) == -1)
		goto exit_process_5;
	if ((message = (char *) shmat(msgid, NULL, 0)) == (void *) -1)
		goto exit_process_4;

	/*
	 * Setup di 3 istanze di Semaforo con i
	 * seguenti indici:
	 *  0 - Semaforo per lo Scrittore inizializzato
	 *      ad N_READER gettoni;
	 *  1 - Semaforo per tutti i Lettori inizializzato
	 *      a 0;
	 *  2 - Semaforo per tutti i Lettori che che hanno
	 *      completato la singola operazione di lettur
	 *      ammessa per ognuno di essi ed inizializzato
	 *      a 0.
	 */
	if ((semid = semget(SEMAPHORE_KEY, 3, IPC_CREAT|0666)) == -1)
		goto exit_process_3;
	sem_arg.val = N_READER;
	if (semctl(semid, 0, SETVAL, sem_arg) == -1)
		goto exit_process_2;
	sem_arg.val = 0;
	if (semctl(semid, 1, SETVAL, sem_arg) == -1)
		goto exit_process_2;
	sem_arg.val = 0;
	if (semctl(semid, 2, SETVAL, sem_arg) == -1)
		goto exit_process_2;

	/* 
	 * Generazione dei processi Lettori.
	 */
	for (i=0; i<N_READER; i++)
	{
		if ((pid = fork()) == -1)
			goto exit_process_2;
		else if (pid == 0)
			reader_function();
		else
			pid_reader[i] = pid;
	}

	/*
	 * Inizializzazione di un Handler per gestire
	 * il segnale SIGUSR1, utile al fine di coordinare
	 * tutti i processi Scrittori connessi nell'anello
	 * che opereranno sulla memoria condivisa per turno.
	 */
	memset((void *) &act, 0, sizeof(struct sigaction));
	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, &old_set);
	act.sa_mask = set;
	act.sa_handler = sigusr1_handler;
	sigaction(SIGUSR1, &act, &old_act);
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_UNBLOCK, &set, NULL);

	/*
	 * Setup della memoria condivisa utilizzata dai soli
	 * Scrittori per conoscere il Process-ID dello scrittore
	 * successivo nell'anello.
	 */
	if ((pidid = shmget(SHMEM_PID_KEY, SHMEM_PID_SIZE, IPC_CREAT|IPC_EXCL|0666)) == -1)
		goto exit_process_2;
	if ((pid_writer = (pid_t *) shmat(pidid, NULL, 0)) == (void *) -1)
		goto exit_process_1;

	/* 
	 * Generazione dei processi Scrittori.
	 */
	for (i=1; i<N_WRITER; i++)
	{
		if ((pid = fork()) == -1)
			goto exit_process_0;
		else if (pid == 0)
		{
			idx_writer = i;

			while (1) pause();
		}
		else
			pid_writer[i] = pid;
	}

	idx_writer = 0;
	pid_writer[idx_writer] = getpid();

	/*
	 * Ci auto-lanciamo un segnale per essere i primi ad
	 * eseguire in accordo al modello Token-Ring.
	 */
	raise(SIGUSR1);

	/*
	 * Gli Scrittori eseguono il programma per un numero
	 * fissato N_ROUND di rounds. 
	 */
	for (i=0; i<N_ROUND; i++) pause();

	/*
	 * Attendiamo che gli ultimi lettori abbiano terminato
	 * la loro attività... 
	 */
	buf[0].sem_num = 0;
	buf[0].sem_op = -N_READER;
	buf[0].sem_flg = 0;

	semop(semid, buf, 1);

	/*
	 * ...prima che possano essere uccisi insieme agli altri
	 * Scrittori in pausa.
	 */
	for (i=0; i<N_READER; i++)
		kill(pid_reader[i], SIGKILL);
	for (i=1; i<N_WRITER; i++)
		kill(pid_writer[i], SIGKILL);

	printf("\n");

	ret = 0;

exit_process_0:
	shmdt(pid_writer);
exit_process_1:
	shmctl(pidid, IPC_RMID, NULL);
exit_process_2:
	semctl(semid, -1, IPC_RMID, sem_arg);
exit_process_3:
	shmdt(message);
exit_process_4:
	shmctl(msgid, IPC_RMID, NULL);
exit_process_5:
	exit(ret);
}