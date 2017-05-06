#include "G-2301-05-P1-tools.h"

#define SIZE 10000
#define PORT_RECORD 8888
#define PORT_SEND 6669
int sockfd;

/**
 * @brief imprime el mensaje del final del reply por la interfaz
 *
 * @param channel canal donde se escribe, o null si es en system
 * @param nick nick del usuario que lo manda
 * @param msg reply de la cual imprimimos el ultimo campo
 * @param thread T si es la version con threads, false, si no
 * 
 */
void printXchat(char* channel, char* nick, char* msg, boolean thread);

/**
 * @brief funcion que devuelve el nick asociado a este cliente. version para ser llamada en algun callback de la interfaz
 *
 * @return el nick asociado a este cliente
 */
char* getMyNick();

/**
 * @brief funcion que devuelve el nick asociado a este cliente. version para ser llamada por los hilos del cliente
 *
 * @return el nick asociado a este cliente
 */
char* getMyNickThread();


//long prefixParse(char* prefix, char** nick, char** user, char** host);