#ifndef _SOCKET_H
#define _SOCKET_H

#include "G-2301-05-P1-tools.h"

#define SERVERINFO "servidor de Mambru, DoReMi DoReFa"
#define SERVERNAME "Mambru@war"

int openSocket_TCP();

int bindSocket_TCP(int sockfd, int portno, struct sockaddr_in* serv_addr);

int acceptConnection(int sockfd, struct sockaddr_in* client_addr);

int connectTo(int sockfd, char* hostname, int portno);

int connectToIP(int sockfd, char* IP, int portno);
#endif