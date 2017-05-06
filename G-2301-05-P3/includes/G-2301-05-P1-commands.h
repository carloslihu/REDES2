#ifndef _COMMANDS_H
#define _COMMANDS_H

#include "G-2301-05-P1-socket.h"
/**
 * @brief commando por defecto
 * envía un mensaje de error al cliente, indicando que el comando introducido es invalido
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandDefault(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //DEFAULT 0

/**
 * @brief commando nick
 * permite al usuario cambiar de nickname
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandNick(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //NICK 2

/**
 * @brief commando mode
 * permite al usuario cambiar el modo un canal especificado
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandMode(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //MODE 5 (?)

/**
 * @brief commando quit
 * permite al usuario abandonar el servidor
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandQuit(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //QUIT 7 advanced

/**
 * @brief commando join
 * permite al usuario unirse a un canal. En caso de que se requiera una password para unirse,
 * el usuario debería incluirla en el comando tal y como se indica en el RFC. Tratar de unirse a un
 * canal inexistente, creará dicho canal
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandJoin(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //JOIN 9

/**
 * @brief commando part
 * permite al usuario abandonar el canal indicado. Si se trata de abandonar un canal invalido
 * se le enviará un mensaje de notificación al cliente
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandPart(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //PART 10

/**
 * @brief commando topic
 * permite al usuario consultar el topic de un canal especificado o bien cambiar el mismo
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server  los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandTopic(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //TOPIC 11

/**
 * @brief commando mode
 * permite al usuario obtener la lista de nicknames de usuarios que pertenezcan a un canal
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server  los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandNames(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //NAMES 12

/**
 * @brief commando list
 * permite al usuario obtener la lista de canales del servidor o bien información sobre un canal en partiular
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server  los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandList(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //LIST 13

/**
 * @brief commando kick
 * permite al usuario expulsar a un usuario de un canal (siempre y cuando se sea operador del canal)
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server  los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandKick(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //KICK 15

/**
 * @brief commando Privmsg
 * permite al usuario enviar mensajes tanto a un usuario en concreto como a todos los usuarios de un canal
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandPrivmsg(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //PRIVMSG 16

/**
 * @brief commando Message of the Day
 * permite al usuario consultar el mensaje del día
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandMotd(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //MOTD 18 advanced

/**
 * @brief commando Whois
 * permite al usuario obtener información de cierto usuario en el servidor
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandWhois(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //WHOIS 31

/**
 * @brief commando Ping
 * permite al usuario comprobar que la conexion con el servidor sigue estando activa
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param client los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandPing(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //PING 34

/**
 * @brief commando Away
 * permite al usuario marcarse como ausente
 *
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandAway(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin); //AWAY 37 advanced
/**
 * 
 * @param socket el socket del usuario a ver
 * @param client los datos del dispositivo cliente
 * @param server los datos del dispositivo servidor
 * @param strin el commando recibido
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int commandWho(int socket, struct sockaddr_in *client, struct sockaddr_in *server, char* strin);
/**
 * @brief envia un comando en masa
 * envia un comando a cada uno de los clientes especificados en la lista de nicks
 *
 * @param command el comando a enviar en masa
 * @param nicks la lista con los nicks a quienes hemos de enviar el comando
 * @param num la longitud de la lista de nicks
 *
 * @return IRC_OK si fue bien, otra cosa si no
 */
long int megaSend(char* command, char** nicks, long int num);

#endif
