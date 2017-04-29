#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "G-2301-05-P1-socket.h"

long int commandDefault(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//DEFAULT 0
long int commandNick(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//NICK 2
long int commandMode(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//MODE 5 (?)
long int commandQuit(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//QUIT 7 advanced
long int commandJoin(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//JOIN 9
long int commandPart(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//PART 10
long int commandTopic(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//TOPIC 11
long int commandNames(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//NAMES 12
long int commandList(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//LIST 13
long int commandKick(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//KICK 15
long int commandPrivmsg(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//PRIVMSG 16
long int commandMotd(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//MOTD 18 advanced
long int commandWhois(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//WHOIS 31
long int commandPing(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//PING 34
long int commandAway(int socket, struct sockaddr_in *client, struct sockaddr_in *server ,char* strin);//AWAY 37 advanced

#endif
