#include "../includes/G-2301-05-P2-userTools.h"

/**
 * @brief imprime el mensaje del final del reply por la interfaz
 *
 * @param channel canal donde se escribe, o null si es en system
 * @param nick nick del usuario que lo manda
 * @param msg reply de la cual imprimimos el ultimo campo
 * @param thread T si es la version con threads, false, si no
 * 
 */
//FIXME revisar, porque en muchos casos se llama a esta funcion pero no imprime nada por la interfaz (p.e. linea 91 de G-3201-05-P2-repliesFromServer)
void printXchat(char* channel, char* nick, char* msg, boolean thread) {
    char msg2[SIZE], extra[SIZE], write[SIZE];
    //bzero(write,SIZE);
    memset(write, 0, SIZE);
    memset(msg2, 0, SIZE);
    sscanf(msg, ":%[^:] :%[^\r\n]", extra, msg2);
    if(msg2[0] == 0)
    	return;
    write[0] = '*';
    strncat(write, msg2, SIZE-1);
    if (channel) {
        if (thread == TRUE)
            IRCInterface_WriteChannelThread(channel, nick, write);
        else
            IRCInterface_WriteChannel(channel, nick, write);
    } else {
        if (thread == TRUE)
            IRCInterface_WriteSystemThread(nick, write);
        else
            IRCInterface_WriteSystem(nick, write);
    }
}

/**
 * @brief funcion que devuelve el nick asociado a este cliente. version para ser llamada en algun callback de la interfaz
 *
 * @return el nick asociado a este cliente
 */
char* getMyNick(){
	char *nick, *user, *realname, *password, *server;
	int port, ssl;

	IRCInterface_GetMyUserInfo(&nick, &user, &realname, &password, &server, &port, &ssl);
	printf("nick: %s\nuser: %s\nrealname: %s\npassword: %s\nserver: %s\n",nick, user, realname, password, server);
	IRC_MFree(4, &user, &realname, &password, &server);
	return nick;
}

/**
 * @brief funcion que devuelve el nick asociado a este cliente. version para ser llamada por los hilos del cliente
 *
 * @return el nick asociado a este cliente
 */
char* getMyNickThread(){
	char *nick, *user, *realname, *password, *server;
	int port, ssl;

	IRCInterface_GetMyUserInfoThread(&nick, &user, &realname, &password, &server, &port, &ssl);
	printf("nick: %s\nuser: %s\nrealname: %s\npassword: %s\nserver: %s\n",nick, user, realname, password, server);
	IRC_MFree(4, &user, &realname, &password, &server);
	return nick;
}