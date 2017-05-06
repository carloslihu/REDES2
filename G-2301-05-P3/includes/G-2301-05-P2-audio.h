#include "G-2301-05-P2-basicCommandsFromServer.h"
#include "G-2301-05-P1-socket.h"
#include <arpa/inet.h>
 
#define BUFLEN 160  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
#define SERVER "127.0.0.1"


long int initiateReciever();
long int initiateSender();