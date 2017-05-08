#ifndef _TYPES_H
#define _TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> 
#include <stdint.h>
#include <syslog.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <redes2/irc.h>
#include <redes2/irctad.h>
#include <redes2/ircxchat.h>

#define CLIENTNAME "JohnTitor"
#define ERROR -1
#define ERROR_SEND -2
#define ERROR_RECV -3
#define ERROR_MALLOC -4
#define ERROR_CONTINUE -5
#define MACRO_QUIT -6
#endif