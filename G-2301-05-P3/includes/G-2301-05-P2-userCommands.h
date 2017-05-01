#ifndef _USERCOMMANDS_H
#define _USERCOMMANDS_H
#include "../includes/G-2301-05-P1-commands.h"
#include "../includes/G-2301-05-P2-userTools.h"

/**
 * @brief funcion que comprueba si el entero largo es un comando de usuario implementado por este cliente
 *
 * @param ncommand el entero largo que representa un comando
 *
 * @return TRUE si ese comando es soportado por este cliente, FALSE de lo contrario
 */
boolean isCommand(long ncommand);

/**
 * @brief funcion que envia un comando AWAY bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userAway (int socket, char* strin);
//long userCycle(int socket,char* strin);
//long userHelp(int socket,char* strin);
//long userInvite(int socket,char* strin);

/**
 * @brief funcion que envia un comando JOIN bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userJoin(int socket, char* strin);

/**
 * @brief funcion que envia un comando KICK bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userKick(int socket, char* strin);
//long userLeave(int socket,char* strin);

/**
 * @brief funcion que envia un comando LIST bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userList(int socket, char* strin);
//long userLusers(int socket,char* strin);

/**
 * @brief funcion que envia un comando MODE bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userMode(int socket, char* strin);

/**
 * @brief funcion que envia un comando MOTD bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userMotd(int socket, char* strin);
//long userMsg(int socket,char* strin);

/**
 * @brief funcion que envia un comando NAMES bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userNames(int socket, char* strin);

/**
 * @brief funcion que envia un comando NICK bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userNick(int socket, char* strin);
//long userNotice(int socket,char* strin);

/**
 * @brief funcion que envia un comando PART bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userPart(int socket, char* strin);

/**
 * @brief funcion que envia un comando PRIVMSG bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userPriv(int socket, char* strin);
//long userQuery(int socket,char* strin);

/**
 * @brief funcion que envia un comando QUIT bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userQuit(int socket, char* strin);

/**
 * @brief funcion que envia un comando TOPIC bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userTopic(int socket, char* strin);
//long userWho(int socket,char* strin);

/**
 * @brief funcion que envia un comando WHOIS bien formado al servidor
 *
 * @param strin el comando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 */
long userWhois(int socket, char* strin);
#endif