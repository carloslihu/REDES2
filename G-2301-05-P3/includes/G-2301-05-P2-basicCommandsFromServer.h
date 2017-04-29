#include "../includes/G-2301-05-P2-userTools.h"

#define MSG_DEFAULT 0
#define MSG_PASS 1
#define MSG_NICK 2
#define MSG_MODE 5
#define MSG_SERVICE 6
#define MSG_QUIT 7
#define MSG_JOIN 9
#define MSG_PART 10
#define MSG_TOPIC 11
#define MSG_NAMES 12
#define MSG_KICK 15
#define MSG_PRIVMSG 16
#define MSG_WHO 30
#define MSG_WHOIS 31
#define MSG_PING 34
#define MSG_SETNAME 58

long reactDefault(char*strin);
long reactPrint(char*strin);
long reactPass(char* strin);
long reactNick(char *strin);
long reactMode(char* strin);
long reactService(char* strin);
long reactQuit(char* strin);
long reactJoin(char *strin);
long reactPart(char* strin);
long reactTopic(char* strin);
long reactKick(char* strin);
long reactPrivmsg(char* strin);
long reactWho(char* strin);
long reactWhoIs(char* strin);
long reactPing(char* strin);
long reactSetName(char* strin);
long reactNames(char* strin);