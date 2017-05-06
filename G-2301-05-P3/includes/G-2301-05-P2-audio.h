#include "G-2301-05-P2-basicCommandsFromServer.h"
#include "G-2301-05-P1-socket.h"
#include <arpa/inet.h>
#include <unistd.h>
 
#define BUFLEN 160  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
#define SERVER "127.0.0.1"

int canIWrite(int len);
int canIRead(int len);
void writeBuffer(char* buf, int len);
void playBuffer(int len);
boolean alreadyRecordingQuery();
void endAudioTransmission();
long int initiateReciever();
long int initiateSender();