#include "../includes/G-2301-05-P1-socket.h"
#include "../includes/G-2301-05-P1-commands.h"
#include "../includes/G-2301-05-P1-tools.h"
#include "../includes/G-2301-05-P2-userTools.h"
#include "../includes/G-2301-05-P1-types.h"
#define THREAD_NUMBER 10

char* hostname = "JohnTitor";
pthread_mutex_t mutex;

struct threadArgs {
    int socket;
    struct sockaddr_in *client;
    struct sockaddr_in *server;
};

typedef long int (*pFuncs)(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin);
pFuncs functs[100];

/**
 * @brief libera los recursos de usuario asociados al hilo
 *
 * @param socket el socket que es manejado por el hilo
 */
void freeThreadResources(int socket) {
    char* nick;
    long int retVal;
    if ((retVal = getNickFromSocket(socket, &nick)) != IRC_OK) {
        logIntError(retVal, "error @ freeThreadResources -> getNickFromSocket");
        return;
    }
    IRCTADUser_Delete(0, NULL, nick, NULL);
    free(nick);
}

/**
 * @brief maneja el inicio de sesión del cliente
 *
 * @param sockfd el socket a enlazar
 * @param client los datos del dispositivo cliente
 * @param client los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
int connectAndRegister(int sockfd, struct sockaddr_in *client, struct sockaddr_in *server) {
    int state = 0, oSocket;
    char *command = NULL, *nickname = NULL, *username = NULL;
    char *prefix = NULL, *msg = NULL, *modeHost = NULL, *serverOther = NULL, *realname = NULL;
    char *strPos, *myIP, *otherIP, *rpl;
    char buffer[256];
    long int retVal;

    getIP(server, &myIP);
    getIP(client, &otherIP);
    memset(buffer, 0, 256);

    while (state == 0) {//state 0 implica esperar a recibir un par de comandos NICK USER validos
        while (nickname == NULL) {//en este bucle esperaremos a que nos llegue un NICK bien formado
            memset(buffer, 0, 256);
            if (recv(sockfd, buffer, 255, 0) <= 0)
                return logIntError(-1, "error @ recv NICK\n");
            //printf("*****************************\n%s\n*****************************\n", buffer);
            strPos = buffer;
            strPos = IRC_UnPipelineCommands(strPos, &command);
            if (IRC_CommandQuery(command) == 2) {//hemos recibido un NICK
                if (IRCParse_Nick(command, &prefix, &nickname, &msg) != IRC_OK) {//en caso de que no se pueda parsear bien el comando
                    logVoidError("error @ connectAndRegister -> IRCParse_Nick");
                    IRC_MFree(2, &nickname, &command);
                }
            } else
                logVoidError("no es un mensaje NICK");
            IRC_MFree(2, &prefix, &command);
        }
        while (username == NULL) {//en este bucle esperaremos a que nos llegue un USER bien formado
            if (strPos == NULL) {//quiere decir que el mensaje anterior no tenia los dos comandos
                memset(buffer, 0, 256);
                if (recv(sockfd, (char*) buffer, 255, 0) <= 0)//asi que esperamos hasta recibir otro mensaje con otro comando
                    return logIntError(-1, "error @ recv USER");
                //printf("*****************************\n%s\n*****************************\n", buffer);
                strPos = buffer;
            }
            strPos = IRC_UnPipelineCommands(strPos, &command);
            if (IRC_CommandQuery(command) == 3) {//hemos recibido un USER
                //en caso de que no se pueda parsear bien el comando USER
                if (IRCParse_User(command, &prefix, &username, &modeHost, &serverOther, &realname) != IRC_OK) {
                    logVoidError("error @ connectAndRegister -> IRCParse_User");
                    free(username);
                    username = NULL;
                }
            } else
                logIntError(0, "no es un mensaje USER");
            IRC_MFree(4, &command, &prefix, &modeHost, &serverOther);
        }
        //ahora que hemos recibido tanto un comando NICK como un comando USER, vamos a ver si los NICK y USER introducidos son válidos
        if (1) {//TODO: poner en esta condición la función que comprueba que el NICK y el USER no están ya usados.
            if ((retVal = IRCTADUser_New(username, nickname, realname, NULL, SERVERNAME, otherIP, sockfd)) != IRC_OK) {
                if (retVal == IRCERR_NICKUSED) {
                    if ((retVal = getSocketFromNick(&oSocket, nickname)) != IRC_OK)
                        return logIntError(retVal, "error @ connectAndRegister -> getSocketFromNick");
                    if ((retVal = IRCTADUser_Delete(0, username, nickname, realname)) != IRC_OK)
                        return logIntError(retVal, "error @ connectAndRegister -> IRCTADUser_Delete");
                    //close(oSocket);
                    retVal = IRCTADUser_New(username, nickname, realname, NULL, SERVERNAME, otherIP, sockfd);
                } else
                    return logIntError(retVal, "error @ connectAndRegister -> IRCTADUser_New\n");
            }
            IRCMsg_RplWelcome(&rpl, myIP, nickname, "msg", username, otherIP);
            send(sockfd, rpl, strlen(rpl) + 1, 0);
            state = 1;
        } else {
            state = 0;
        }
        IRC_MFree(7, &nickname, &username, &msg, &realname, &rpl, &myIP, &otherIP);
    }
    return IRC_OK;
}

/**
 * @brief se encarga de recibir mensajes, separar los comandos y llamar a la función que maneje el comando pertinente en cada caso
 *
 * @param sockfd el socket a enlazar
 * @param client los datos del dispositivo cliente
 * @param client los datos del dispositivo servidor
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int retrieveMsg(int sockfd, struct sockaddr_in *server, struct sockaddr_in *client) {
    char buffer[512];
    char *command = NULL;
    char *strPos;
    int byteCount;
    long int commandNumber, retVal;
    while (1) {
        //printf("se ingresa en el bucle de retrieveMsg\n");
        memset(buffer, 0, 512);
        byteCount = recv(sockfd, (char*) buffer, 511, 0);
        if (byteCount == 0) {
            close(sockfd);
            return logIntError(0, "retrieveMsg exited");
        } else if (byteCount == -1)
            return logIntError(-1, "error @ retrieveMsg -> recv");
        //printf("*****************************\n%s\n*****************************\n", buffer);
        strPos = buffer;
        while (strPos != NULL) {
            strPos = IRC_UnPipelineCommands(strPos, &command);
            if (command != NULL) {
                commandNumber = IRC_CommandQuery(command);
                if (commandNumber < 0) {
                    if (commandDefault(sockfd, server, client, command) != IRC_OK)
                        return logIntError(-1, "error @ retrieveMsg -> commandUnknown");
                } else if ((retVal = functs[commandNumber](sockfd, server, client, command)) != IRC_OK)
                    return logIntError(retVal, "error @ retrieveMsg -> pFuncs");
            }
            IRC_MFree(1, &command);
        }
    }
    return IRC_OK;
}

void* threadPing(void* args) {
    char **nicks;
    char *user, *nick, *real, *host, *IP, *away;
    char buf[10] = "PING";
    int socket, i;
    long num, id, creationTS, actionTS;
    nicks = NULL;
    user = nick = real = host = IP = away = NULL;
    socket = num = id = creationTS = 0;

    while (1) {
        if (IRCTADUser_GetNickList(&nicks, &num) != IRC_OK)
            return logPointerError(NULL, "error @ threadPing: IRCTADUser_GetNickList");
        for (i = 0; i < num; i++) {
            if (IRCTADUser_GetData(&id, &user, &nicks[i], &real, &host, &IP, &socket, &creationTS, &actionTS, &away) != IRC_OK)
                return logPointerError(NULL, "error @ threadPing: IRCTADUser_GetData");
            if ((time(NULL) - actionTS) > 30) {
                close(socket);
            }
            if (send(socket, buf, strlen(buf), 0) == -1)
                return logPointerError(NULL, "error @ threadPing: send");

            IRC_MFree(6, &user, &nicks[i], &real, &host, &IP, &away);
            id = socket = creationTS = actionTS = 0;
        }
        IRC_MFree(1, &nicks);
        sleep(20);
    }
}

/**
 * @brief el comportamiento del hilo tras ser creado.
 *
 * @param args deberá ser un puntero a la estructura threadArgs
 *
 * @return NULL al acabar la sesión del hilo
 */
void* threadRoutine(void* args) {
    long int retVal;

    pthread_detach(pthread_self());
    if (args == NULL)
        return logPointerError(NULL, "error @ threadRoutine: NULL pointer argument");

    struct threadArgs* aux = (struct threadArgs*) args;
    int socket = aux->socket;
    struct sockaddr_in server;
    struct sockaddr_in client;

    memset(&server, 0, sizeof (struct sockaddr_in));
    memset(&client, 0, sizeof (struct sockaddr_in));
    memcpy(&server, aux->server, sizeof (struct sockaddr_in));
    memcpy(&client, aux->client, sizeof (struct sockaddr_in));
    pthread_mutex_unlock(&mutex);

    connectAndRegister(socket, &client, &server);
    while ((retVal = retrieveMsg(socket, &server, &client)) > 0);
    if (retVal != MACRO_QUIT)
        freeThreadResources(socket);
    close(socket);
    free(args);
    return NULL;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t th;
    struct threadArgs* args;

    //daemonizamos. perderemos el rastro del proceso en la terminal. todos los errores se escribiran en el syslog
    //daemonizar();

    functs[0] = commandDefault;
    functs[1] = commandDefault;
    functs[2] = commandNick;
    functs[3] = commandDefault;
    functs[4] = commandDefault;
    functs[5] = commandMode;
    functs[6] = commandDefault;
    functs[7] = commandQuit;
    functs[8] = commandDefault;
    functs[9] = commandJoin;
    functs[10] = commandPart;
    functs[11] = commandTopic;
    functs[12] = commandNames;
    functs[13] = commandList;
    functs[14] = commandDefault;
    functs[15] = commandKick;
    functs[16] = commandPrivmsg;
    functs[18] = commandMotd;
    functs[31] = commandWhois;
    functs[34] = commandPing;
    functs[37] = commandAway;
    functs[WHO] = commandWho;
    if (argc < 2)
        portno = 6667;
    else
        portno = atoi(argv[1]);
    if (pthread_mutex_init(&mutex, NULL)) {
        logVoidError("error @ main -> pthread_mutex_init");
        return -1;
    }
    sockfd = openSocket_TCP();
    if (bindSocket_TCP(sockfd, portno, &serv_addr) == -1) {
        close(sockfd);
        exit(1);
    }
    if (listen(sockfd, 50) != 0)//mark sockfd as a socket that will be used to accept incoming connection requests
        return logIntError(1, "error @ main -> listen");

    pthread_create(&th, NULL, &threadPing, NULL);
    while (1) {
        args = (struct threadArgs*) malloc(sizeof (struct threadArgs));
        if (args == NULL)
            return logIntError(1, "error @ main -> malloc");
        pthread_mutex_lock(&mutex);
        newsockfd = acceptConnection(sockfd, &cli_addr);
        if (newsockfd == -1)
            logIntError(-1, "error @ main -> acceptConnection");
        args->socket = newsockfd;
        args->server = &serv_addr;
        args->client = &cli_addr;
        pthread_create(&th, NULL, &threadRoutine, args);
    }

    close(sockfd);
    return 0;
}
