#ifndef _BASICCOMMANDSFROMSERVER_H
#define _BASICCOMMANDSFROMSERVER_H
#include "../includes/G-2301-05-P2-userTools.h"

#define MSG_DEFAULT 0
#define MSG_PASS 1
#define MSG_NICK 2
#define MSG_MODE 5
#define MSG_SERVICE 6
#define MSG_QUIT 7
#define MSG_JOIN 9
#define MSG_PART 10
#define MSG_TOPIC 11
#define MSG_NAMES 12
#define MSG_KICK 15
#define MSG_PRIVMSG 16
#define MSG_WHO 30
#define MSG_WHOIS 31
#define MSG_PING 34
#define MSG_SETNAME 58

#define FILE_BUFLEN 1024

/** 
 * @defgroup IRCClient client
 *
 */

/** 
 * @defgroup IRCCommandsFromServer commandsFromServer
 * @ingroup IRCClient
 *
 */

/** 
 * @addtogroup IRCCommandsFromServer
 * Funciones que manejan los comandos del servidor
 *
 * <hr>
 */

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactDefault reactDefault
 *
 * @brief rutina por defecto ante un mensaje no reconocido por este cliente
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactDefault(char*strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactPrint reactPrint
 *
 * @brief maneja cualquier mensaje en general que envie el servidor, imprimiendo en xchat strin
 *
 * @param strin el commando recibido
 *
 * @return IRC_OK si fue bien, otra cosa si no
 *
 * <hr>
 */
long reactPrint(char*strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactPass reactPass
 *
 * @brief rutina que maneja la llegada de un comando PASS
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactPass(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactNick reactNick
 *
 * @brief rutina que maneja la llegada de un comando NICK. Esta rutina cambia el nick del usuario
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactNick(char *strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactMode reactMode
 *
 * @brief rutina que maneja la llegada de un comando MODE. Este comando cambia los modos del canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactMode(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactService reactService
 *
 * @brief rutina que maneja la llegada de un comando SERVICE
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactService(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactJoin reactJoin
 *
 * @brief rutina que maneja la llegada de un comando JOIN.
 * Si este cliente es quien ha solicitado unirse a un canal, se crea una pestana con este nuevo canal
 * en este caso, para actualizar la lista de usuarios en el canal, se envia al servidor un comando who
 * tambien se envia un comando mode para preguntar por los modos actuales del canal
 * Si no ha sido este cliente quien se unio, se actualiza la lista de usuarios del canal con el nuevo usuario
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactJoin(char *strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactPart reactPart
 *
 * @brief rutina que maneja la llegada de un comando PART.
 * Si este cliente ha solicitado salir del canal, la pestaña correspondiente a este canal desaparecerá
 * Si ha sido otro cliente el que ha hecho el part, se le elimina de la lista de usuarios en el canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactPart(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactTopic reactTopic
 *
 * @brief rutina que maneja la llegada de un comando TOPIC. Cambia el topico del canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactTopic(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactKick reactKick
 *
 * @brief rutina que maneja la llegada de un comando KICK
 * Si el objetivo del kick es este cliente, entonces se elimina la pestana del canal del cual le han expulsado
 * de lo contrario, se elimina al usuario expulsado de la lista de usuarios del canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactKick(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactPrivmsg reactPrivmsg
 *
 * @brief rutina que maneja la llegada de un comando PRIVMSG. maneja envio de mensajes tanto a canales como a usuarios
 * si en la interfaz no existe la conversacion en la que se participa, se crea una nueva pestana con esta.
 * tambien maneja un caso particular de PrivMsg que sirve para identificar una peticion de envio de ficheros
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactPrivmsg(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactPing reactPing
 *
 * @brief rutina que maneja la llegada de un comando PING
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactPing(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactSetName reactSetName
 *
 * @brief rutina que maneja la llegada de un comando SETNAME
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactSetName(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactNames reactNames
 *
 * @brief rutina que maneja la llegada de un comando NAMES
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactNames(char* strin);

/**
 * @ingroup IRCCommandsFromServer
 *
 * @page reactQuit reactQuit
 *
 * @brief rutina que maneja la llegada de un comando QUIT. Se asume que solo se recibirá un quit de otro cliente
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactQuit(char* strin);

#endif