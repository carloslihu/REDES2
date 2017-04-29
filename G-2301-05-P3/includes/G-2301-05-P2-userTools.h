#include "G-2301-05-P1-tools.h"

#define SIZE 10000
#define PORT_RECV 6668
#define PORT_SEND 6669

int sockfd;

void printXchat(char* channel, char* nick, char* msg, boolean thread);
char* getMyNick();
char* getMyNickThread();
long prefixParse(char* prefix, char** nick, char** user, char** host);