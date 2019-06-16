#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define BUFFER_SIZE		1024


int main(void)
{
    int sock_desc;

    struct sockaddr_in serv_addr;

    char buffer[BUFFER_SIZE];

    const char *address = "127.0.0.1";

#ifdef STUBBORN
    while (1)
    {
#endif
	    if ((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			exit(-1);

	    memset(&serv_addr, 0, sizeof(struct sockaddr_in));

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(10001);
	    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	    if (inet_aton(address, &serv_addr.sin_addr) == 0)
	    	exit(-1);

	    if (connect(sock_desc, (struct sockaddr *) &serv_addr, (socklen_t) sizeof(struct sockaddr_in)) == -1)
	    	exit(-1);

#ifdef STUBBORN
	    strcpy(buffer,	"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
	    			    "Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
						"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
						"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
						"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
						"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
						"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! "
						"Una valanga di messaggi stanno per arrivare al Server! Una valanga di messaggi stanno per arrivare al Server! ");
#else
	    printf("Inserire la stringa da inviare al Server: ");
	    if (scanf("%[^\n]", buffer) == 0)
	    	buffer[0] = '\0';
	    getc(stdin);
#endif

	    write(sock_desc, buffer, BUFFER_SIZE);

	    close(sock_desc);
#ifdef STUBBORN
	}
#endif
}