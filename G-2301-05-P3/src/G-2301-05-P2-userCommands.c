#include "../includes/G-2301-05-P2-userCommands.h"
#define TAM 13
char*command = NULL;
char*msg = NULL;
long ret = 0;
long nCommands[TAM] = {UAWAY, UJOIN, UMSG, UKICK, ULIST, UMODE, UMOTD, UNAMES, UNICK, UPART, UQUIT, UTOPIC, UWHOIS};

/**
 * @brief funcion que comprueba si el entero largo es un comando de usuario implementado por este cliente
 *
 * @param ncommand el entero largo que representa un comando
 *
 * @return TRUE si ese comando es soportado por este cliente, FALSE de lo contrario
 */
boolean isCommand(long ncommand) {
    int i;
    for (i = 0; i < TAM; i++)
        if (nCommands[i] == ncommand)
            return TRUE;
    return FALSE;
}

/**
 * @brief funcion que envia un comando AWAY bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userAway(int socket, char* strin) {
    command = msg = NULL;
    if ((ret = IRCUserParse_Away(strin, &msg)) != IRC_OK)
        return logIntError(ret, "error @ userAway -> IRCUserParse_Away");
    if ((ret = IRCMsg_Away(&command, NULL, msg)) != IRC_OK)
        return logIntError(ret, "error @ userAway -> IRCMsg_Away");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userAway -> send");
    IRC_MFree(2, &command, &msg);
    return IRC_OK;
}

/**
 * @brief funcion que envia un comando JOIN bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userJoin(int socket, char* strin) {
    char* channel, *password;
    channel = password = command = NULL;

    if ((ret = IRCUserParse_Join(strin, &channel, &password)) != IRC_OK)
        return logIntError(ret, "error @ userJoin -> IRCUserParse_Join");
    if ((ret = IRCMsg_Join(&command, NULL, channel, password, NULL)) != IRC_OK)
        return logIntError(ret, "error @ userJoin -> IRCMsg_Join");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userJoin -> send");
    printf("send: %s\n", command);
    IRC_MFree(3, &command, &channel, &password);
    return IRC_OK;
}

/**
 * @brief funcion que envia un comando KICK bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userKick(int socket, char* strin) {
    char *nick, *command, *channel;
    boolean chanProvided = TRUE;

    command = nick = channel = NULL;
    if ((ret = IRCUserParse_Kick(strin, &nick, &channel)) != IRC_OK)
        return logIntError(ret, "error @ userKick -> IRCUserParse_Kick");
    if(channel == NULL){
        chanProvided = FALSE;
        channel = IRCInterface_ActiveChannelName();
    }
    if ((ret = IRCMsg_Kick(&command, NULL, channel, nick, NULL)) != IRC_OK)
        return logIntError(ret, "error @ userKick -> IRCMsg_Kick");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userKick -> send");
    if(chanProvided == TRUE)
        free(channel);
    IRC_MFree(2, &command, &nick);
    return IRC_OK;

}

/**
 * @brief funcion que envia un comando LIST bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userList(int socket, char* strin) {
    char* channel, *target;
    boolean chanProvided = TRUE;

    command = msg = channel = target = NULL;
    if ((ret = IRCUserParse_List(strin, &channel, &target)) != IRC_OK)
        return logIntError(ret, "error @ userList -> IRCUserParse_List");
    if(channel == NULL){
        chanProvided = FALSE;
        channel = IRCInterface_ActiveChannelName();
    }
    if ((ret = IRCMsg_List(&command, NULL, channel, target)) != IRC_OK)
        return logIntError(ret, "error @ userList -> IRCMsg_List");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userList -> send");
    if(chanProvided == TRUE)
        free(channel);
    IRC_MFree(2, &command, &channel);
    return IRC_OK;
}

/**
 * @brief funcion que envia un comando MODE bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userMode(int socket, char* strin) {
    char*mode, *filter;
    boolean chanProvided = TRUE;

    command = msg = mode = filter = NULL;
    if ((ret = IRCUserParse_Mode(strin, &filter, &mode)) != IRC_OK)
        return logIntError(ret, "error @ userMode -> IRCUserParse_Mode");

    if(filter == NULL){
        chanProvided = FALSE;
        filter = IRCInterface_ActiveChannelName();
    }

    if ((ret = IRCMsg_Mode(&command, NULL, filter, mode, NULL)) != IRC_OK)
        return logIntError(ret, "error @ userMode -> IRCMsg_Mode");

    IRCInterface_WriteSystem(NULL, command);

    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userMode -> send");

    if(chanProvided == TRUE)
        free(filter);
    IRC_MFree(2, &command, &mode);
    return IRC_OK;
}

/**
 * @brief funcion que envia un comando MOTD bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userMotd(int socket, char* strin) {
    command = msg = NULL;
    if ((ret = IRCUserParse_Motd(strin, &msg)) != IRC_OK)
        return logIntError(ret, "error @ userMotd -> IRCUserParse_Motd");
    if ((ret = IRCMsg_Motd(&command, NULL, msg)) != IRC_OK)
        return logIntError(ret, "error @ userMotd -> IRCMsg_Motd");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userMotd -> send");
    IRC_MFree(2, &command, &msg);
    return IRC_OK;
}

/**
 * @brief funcion que envia un comando NAMES bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userNames(int socket, char* strin) {
    char* channel = NULL, *target = NULL;
    command = msg = NULL;
    boolean chanProvided = TRUE;

    if ((ret = IRCUserParse_Names(strin, &channel, &target) != IRC_OK))
        return logIntError(ret, "error @ userNames -> IRCUserParse_Names");

    if(channel == NULL){
        chanProvided = FALSE;
        channel = IRCInterface_ActiveChannelName();
    }

    if ((ret = IRCMsg_Names(&command, NULL, channel, target)) != IRC_OK)
        return logIntError(ret, "error @ userNames -> IRCMsg_Names");

    IRCInterface_WriteSystem(NULL, command);

    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userNames -> send");
    if(chanProvided == TRUE)
        free(channel);
    IRC_MFree(2, &command, &target);
    return IRC_OK;

}

/**
 * @brief funcion que envia un comando NICK bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userNick(int socket, char*strin) {
    char* newnick = NULL;
    command = msg = NULL;
    if ((ret = IRCUserParse_Nick(strin, &newnick)) != IRC_OK)
        return logIntError(ret, "error @ userNick -> IRCUserParse_Nick");
    if ((ret = IRCMsg_Nick(&command, NULL, newnick, NULL)) != IRC_OK)
        return logIntError(ret, "error @ userNick -> IRCMsg_Nick");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userNick -> send");
    IRC_MFree(2, &command, &newnick);

    return IRC_OK;
}

/**
 * @brief funcion que envia un comando PART bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userPart(int socket, char* strin) {
    command = msg = NULL;
    boolean chanProvided = TRUE;
    if ((ret = IRCUserParse_Part(strin, &msg)) != IRC_OK)
        return logIntError(ret, "error @ userPart -> IRCUserParse_Part");
    //long IRCMsg_Part (char **command, char *prefix, char *channel, char *msg)
    if (msg == NULL) {
        chanProvided = FALSE;
        msg = IRCInterface_ActiveChannelName();
    }
    if ((ret = IRCMsg_Part(&command, NULL, msg, NULL)) != IRC_OK)
        return logIntError(ret, "error @ userPart -> IRCMsg_Part");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userPart -> send");
    printf("send: %s\n", command);

    if(chanProvided == TRUE)
        free(msg);
    //IRC_MFree(2, &command, &msg);

    return IRC_OK;
}

/**
 * @brief funcion que envia un comando PRIVMSG bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userPriv(int socket,char* strin){
    command = msg = NULL;
    char *nickorchannel, *msg, *myNick;
    boolean chanProvided = TRUE;

    if((ret = IRCUserParse_Priv (strin, &nickorchannel, &msg)) != IRC_OK)
        return logIntError(ret, "error @ userPriv -> IRCUserParse_Priv");
    if(msg == NULL)
        return IRC_OK;
    if(nickorchannel == NULL){
        chanProvided = FALSE;
        nickorchannel = IRCInterface_ActiveChannelName();
    }
    if((ret = IRCMsg_Privmsg(&command, NULL, nickorchannel, msg)) != IRC_OK)
        return logIntError(ret, "error @ userPriv -> IRCMsg_Privmsg");
    if(send(socket, command, strlen(command), 0) <= 0)
        return logIntError(-1, "error @ userPriv -> send");
    myNick = getMyNick();
    if(IRCInterface_QueryChannelExist(nickorchannel) == FALSE)
            IRCInterface_AddNewChannel(nickorchannel, 0);
    IRCInterface_WriteChannel(nickorchannel, myNick, msg);
    if(chanProvided == TRUE)
        free(nickorchannel);
    return IRC_OK;
}

/**
 * @brief funcion que envia un comando QUIT bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userQuit(int socket, char* strin) {
    command = msg = NULL;
    if ((ret = IRCUserParse_Quit(strin, &msg)) != IRC_OK)
        return logIntError(ret, "error @ userQuit -> IRCUserParse_Quit");
    if(msg == NULL){
        msg = (char*)malloc(sizeof(char)*(strlen("Leaving")+1));
        strcpy(msg, "Leaving");
    }
    if ((ret = IRCMsg_Quit(&command, NULL, msg)) != IRC_OK)
        return logIntError(ret, "error @ userQuit -> IRCMsg_Quit");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userQuit -> send");

    close(sockfd);
    sockfd = -1;

    IRC_MFree(2, &command, &msg);
    return IRC_OK;

}

/**
 * @brief funcion que envia un comando TOPIC bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userTopic(int socket, char* strin) {
    //char *prefix, *myUser, *myNick, *myRealname, *myPassword, *myServer;
    //int myPort, mySsl;
    char* command, *msg, *channel;
    command = msg = channel = NULL;
    if ((ret = IRCUserParse_Topic(strin, &msg)) != IRC_OK)
        return logIntError(ret, "error @ userTopic -> IRCUserParse_Topic");
    //IRCInterface_GetMyUserInfo (&myNick, &myUser, &myRealname, &myPassword, &myServer, &myPort, &mySsl);
    //IRC_Prefix(&prefix, myNick, myUser, CLIENTNAME, myServer);
    channel = IRCInterface_ActiveChannelName();
    if ((ret = IRCMsg_Topic(&command, NULL, channel, msg)) != IRC_OK)
        return logIntError(ret, "error @ userTopic -> IRCMsg_Topic");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userTopic -> send");
    printf("send: %s\n", command);
    IRC_MFree(2, &command, &msg);

    return IRC_OK;
}

/**
 * @brief funcion que envia un comando WHOIS bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userWhois(int socket, char* strin) {
    char* nick;
    command = msg = nick = NULL;
    if ((ret = IRCUserParse_Whois(strin, &nick)) != IRC_OK)
        return logIntError(ret, "error @ userWhois -> IRCUserParse_Whois");
    if ((ret = IRCMsg_Whois(&command, NULL, nick, NULL)) != IRC_OK)
        return logIntError(ret, "error @ userWhois -> IRCMsg_Part");
    IRCInterface_WriteSystem(NULL, command);
    if (send(socket, command, strlen(command), 0) == -1)
        return logIntError(-1, "error @ userPart -> send");
    IRC_MFree(2, &command, &nick);
    return IRC_OK;
}
