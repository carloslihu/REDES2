#ifndef _REPLIESFROMSERVER_H
#define _REPLIESFROMSERVER_H
#include "../includes/G-2301-05-P2-userTools.h"


/*
*/

/**
 * @brief rutina que maneja la llegada de la respuesta 332 correspondiente a solicitar los modos de un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactModeQuery(char* strin);

/**
 * @brief rutina que maneja la llegada de la respuesta 331 correspondiente a notificar que no hay topic en un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactNoTopic(char* strin);

/**
 * @brief rutina que maneja la llegada de la respuesta 332 correspondiente a una solicitud del topic de un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactTopicQuery(char* strin);

/*
long reactWelcome(char* strin);
long reactCreated(char* strin);
long reactEndOfWhois(char* strin);
long reactWhoIsUser(char* strin);
long reactWhoIsChannels(char* strin);
//long reactWhoIsOperator(char* strin);
long reactWhoIsServer(char* strin);
long reactWhoIsIdle(char* strin);*/

/**
 * @brief rutina que maneja la llegada de la respuesta 352 correspondiente a una solicitud de WHO
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactWhoReply(char* strin);

/*
long reactYourService(char* strin);
long reactYourHost(char* strin);
long reactMyInfo(char* strin);
long reactEndOfNames(char* strin);
long reactList(char* strin);
long reactListEnd(char* strin);
long reactInfo(char* strin);
long reactMotd(char* strin);
long reactMotdStart(char* strin);
long reactEndOfMotd(char* strin);
long reactLUserChannels(char* strin);
long reactLUserClient(char* strin);
long reactLUserMe(char* strin);
long reactLUserOp(char* strin);
long reactAway(char* strin);
*/
#endif