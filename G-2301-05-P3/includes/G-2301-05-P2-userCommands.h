#ifndef _USERCOMMANDS_H
#define _USERCOMMANDS_H
#include "../includes/G-2301-05-P1-commands.h"
#include "../includes/G-2301-05-P2-userTools.h"
boolean isCommand(long ncommand);
 long userAway (int socket,char* strin);
 //long userCycle(int socket,char* strin);
 //long userHelp(int socket,char* strin);
 //long userInvite(int socket,char* strin);
 long userJoin(int socket,char* strin);
 long userKick(int socket,char* strin);
 //long userLeave(int socket,char* strin);
 long userList(int socket,char* strin);
 //long userLusers(int socket,char* strin);
 long userMode(int socket,char* strin);
 long userMotd(int socket,char* strin);
 //long userMsg(int socket,char* strin);
 long userNames(int socket,char* strin);
 
 long userNick(int socket,char* strin);
 //long userNotice(int socket,char* strin);
 long userPart(int socket,char* strin);
 long userPriv(int socket,char* strin);
 //long userQuery(int socket,char* strin);
 long userQuit(int socket,char* strin);
 long userTopic(int socket,char* strin);
 //long userWho(int socket,char* strin);
 long userWhois(int socket,char* strin);
#endif