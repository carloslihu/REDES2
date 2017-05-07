#include "../includes/G-2301-05-P1-socket.h"
#include "../includes/G-2301-05-P1-tools.h"

/**
 * @brief daemoniza el proceso
 * el proceso queda en segundo plano como proceso de init 
 *
 */
void daemonizar() {
    int pid;
    int sid;
    // Create child process
    pid = fork();
    // Indication of fork() failure
    if (pid == -1)
        exit(1);
        // PARENT PROCESS. Need to kill it.
    else if (pid != 0)
        exit(0);
        //child process	
    else {
        //unmask the file mode
        umask(0);
        //set new session
        sid = setsid();
        if (sid < 0)
            exit(1);
        // Change the current working directory to root.
        chdir("/");
        // Close stdin. stdout and stderr
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        return;
    }
}

/**
 * @brief identifica y registra un error a la vez que devuelve ese mismo error
 * esta función es específica para errores de tipo long int
 *
 * @param returnValue el error a registrar y devolver
 * @param msg el mensaje de error previo al nombre del error
 *
 * @return el error registrado en returnValue
 */
long int logIntError(long int returnValue, char * msg) {
    IRC_syslog(msg, returnValue, LOG_INFO | LOG_USER);
    //IRC_perror(msg, returnValue);
    //printf("%s\n", msg);
    return returnValue;
}

/**
 * @brief identifica y registra un error a la vez que devuelve ese mismo error
 * esta funcion es especifica para los errores de tipo puntero
 *
 * @param returnValue el error a devolver
 * @param msg el mensaje de error
 *
 * @return el error registrado en returnValue
 */
void* logPointerError(void* returnValue, char * msg) {
    //printf("%s\n", msg);
    syslog(LOG_INFO | LOG_USER, "%s", (const char*) msg);
    return returnValue;
}

/**
 * @brief registra un mensaje de error
 * 
 * @param msg el mensaje de error a registrar
 */
void logVoidError(char * msg) {
    syslog(LOG_INFO | LOG_USER, "%s", (const char*) msg);
}

/**
 * @brief reserva memoria para una String que contiene le dirección IP de un cliente en formato estandar: xxx.xxx.xxx.xxx
 *
 * @param client la estructura que contiene la IP a encontrar
 * @param ip la dirección de una String donde guardaremos el resultado
 *
 * @return el puntero a la String resultado
 */
char* getIP(struct sockaddr_in *client, char** ip) {
    *ip = malloc(16 * sizeof (char));
    char aux[16] = "";
    if (*ip == NULL)
        return NULL;
    sprintf(aux, "%d.%d.%d.%d",
            (int) ((*client).sin_addr.s_addr & 0xFF),
            (int) (((*client).sin_addr.s_addr & 0xFF00) >> 8),
            (int) (((*client).sin_addr.s_addr & 0xFF0000) >> 16),
            (int) (((*client).sin_addr.s_addr & 0xFF000000) >> 24));
    aux[15] = 0;
    strcpy(*ip, aux);
    return *ip;
}

/**
 * @brief guarda en hostname el nombre del dispositivo dev
 *
 * @param dev el dispositivo cuyo nombre deseamos encontrar
 * @param hostname la dirección de la String donde escribir el nombre del dispositivo
 *
 * @return IRC_OK si todo va bien
 */
int getHostName(struct sockaddr_in *dev, char** hostname) {
    //no hace falta liberar lo que devuelve esta función
    *hostname = gethostbyaddr(dev, 4, AF_INET)->h_name; //ese 4 es por ser IPv4 (creo ?)
    return IRC_OK;
}

/**
 * @brief guarda en nick el nickname del usuario asociado con el socket
 *
 * @param socket el socket que identifica la conexión con el usuario
 * @param nick la dirección de la String donde guardar
 *
 * @return IRC_OK si todo fue bien. El error si no
 */
int getNickFromSocket(int socket, char** nick) {
    int s = socket;
    long int id = 0;
    char* user, *real, *host, *IP, *away;
    long int creationTs = 0, actionTs = 0, retVal;
    *nick = NULL;
    user = real = host = IP = away = NULL;
    if ((retVal = IRCTADUser_GetData(&id, &user, nick, &real, &host, &IP, &s, &creationTs, &actionTs, &away)) != IRC_OK)
        return logIntError(retVal, "error @ getNickFromSocket -> IRCTADUser_GetData");
    IRC_MFree(5, &user, &real, &host, &IP, &away);
    return IRC_OK;
}

/**
 * @brief obtiene el socket del nick
 *
 * @param socket el socket a obtener
 * @param nick nick a comprobar
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
int getSocketFromNick(int* socket, char* nick) {
    long int id = 0;
    char* user, *real, *host, *IP, *away;
    long int creationTs = 0, actionTs = 0, retVal;
    *socket = 0;
    user = real = host = IP = away = NULL;
    if ((retVal = IRCTADUser_GetData(&id, &user, &nick, &real, &host, &IP, socket, &creationTs, &actionTs, &away)) != IRC_OK)
        return logIntError(retVal, "error @ getSocketFromNick -> IRCTADUser_GetData");
    //printf("retrieved socket: %d\n",*socket);
    IRC_MFree(5, &user, &real, &host, &IP, &away);
    return IRC_OK;
}

/**
 * @brief obtiene el away del socket
 *
 * @param socket el socket a obtener
 * @param away cadena away
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
int getAwayFromSocket(int socket, char** away) {
    long int id = 0;
    char* user, *real, *host, *IP, *nick;
    long int creationTs = 0, actionTs = 0, retVal;
    user = real = host = IP = nick = NULL;
    if ((retVal = IRCTADUser_GetData(&id, &user, &nick, &real, &host, &IP, &socket, &creationTs, &actionTs, away)) != IRC_OK)
        return logIntError(retVal, "error @ getAwayFromSocket -> IRCTADUser_GetData");
    IRC_MFree(5, &user, &real, &host, &IP, &nick);
    return IRC_OK;
}