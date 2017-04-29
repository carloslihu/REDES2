#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <redes2/irc.h>
#include "../includes/G-2301-05-P1-socket.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);

	sockfd = openSocket_TCP();
	if(connectTo(sockfd,argv[1],portno) < 0)
		printf("WTFFFFFF\n");
	while(1)
	{	
		printf("Please enter the message: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
			printf("ERROR writing to socket");
		printf("has escrito: %s\n", buffer);

		printf("Please enter the message: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
			printf("ERROR writing to socket");
		printf("has escrito: %s\n", buffer);

		bzero(buffer,256);//clean the buffer
		n = read(sockfd,buffer,255);//read the message from the client
		if (n < 0)
			error("ERROR reading from socket");
		printf("%s\n",buffer);
    	}
	close(sockfd);
	return 0;
}
