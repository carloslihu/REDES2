#ifndef _TOOLS_H
#define _TOOLS_H

#include "G-2301-05-P1-types.h"
void daemonizar();
char* getIP(struct sockaddr_in* client, char** ip);
int getHostName(struct sockaddr_in *dev, char** hostname);
int getNickFromSocket(int socket, char** nick);
int getSocketFromNick(int* socket, char* nick);
int getAwayFromSocket(int socket, char** away);
long int logIntError(long int returnValue, char * msg);
void* logPointerError(void* returnValue , char * msg);
void logVoidError(char * msg);
#endif