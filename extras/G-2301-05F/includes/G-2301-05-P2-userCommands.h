#ifndef _USERCOMMANDS_H
#define _USERCOMMANDS_H
#include "../includes/G-2301-05-P1-commands.h"
#include "../includes/G-2301-05-P2-userTools.h"

/** 
 * @defgroup IRCClient client
 *
 */

/** 
 * @defgroup IRCUserCommands userCommands
 * @ingroup IRCClient
 *
 */

/** 
 * @addtogroup IRCUserCommands
 * Funciones que se encargan del envio de Commandos al servidor
 *
 * <hr>
 */

/**
 * @ingroup IRCUserCommands
 *
 * @page isCommand isCommand
 *
 * @brief funcion que comprueba si el entero largo es un Commando de usuario implementado por este cliente
 *
 * @param ncommand el entero largo que representa un Commando
 *
 * @return TRUE si ese Commando es soportado por este cliente, FALSE de lo contrario
 *
 * <hr>
 */
boolean isCommand(long ncommand);

/**
 * @ingroup IRCUserCommands
 *
 * @page userDefault userDefault
 *
 * @brief funcion por defecto para valores de Commandos no implementados
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userDefault(int socket, char*strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userAway userAway
 *
 * @brief funcion que envia un Commando AWAY bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userAway (int socket, char* strin);


/**
 * @ingroup IRCUserCommands
 *
 * @page userJoin userJoin
 *
 * @brief funcion que envia un Commando JOIN bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userJoin(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userKick userKick
 *
 * @brief funcion que envia un Commando KICK bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userKick(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userList userList
 *
 * @brief funcion que envia un Commando LIST bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userList(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userMode userMode
 *
 * @brief funcion que envia un Commando MODE bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userMode(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userMotd userMotd
 *
 * @brief funcion que envia un Commando MOTD bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userMotd(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userNames userNames
 *
 * @brief funcion que envia un Commando NAMES bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userNames(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userNick userNick
 *
 * @brief funcion que envia un Commando NICK bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userNick(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userPart userPart
 *
 * @brief funcion que envia un Commando PART bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userPart(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userPriv userPriv
 *
 * @brief funcion que envia un Commando PRIVMSG bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userPriv(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userQuit userQuit
 *
 * @brief funcion que envia un Commando QUIT bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userQuit(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userTopic userTopic
 *
 * @brief funcion que envia un Commando TOPIC bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userTopic(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userWhois userWhois
 *
 * @brief funcion que envia un Commando WHOIS bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userWhois(int socket, char* strin);

/**
 * @ingroup IRCUserCommands
 *
 * @page userWho userWho
 *
 * @brief funcion que envia un Commando WHO bien formado al servidor
 *
 * @param strin el Commando escrito por el usuario
 * @param socket el socket de conexion con el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long userWho(int socket, char* strin);
#endif