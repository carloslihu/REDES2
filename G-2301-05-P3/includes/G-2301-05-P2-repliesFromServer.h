#ifndef _REPLIESFROMSERVER_H
#define _REPLIESFROMSERVER_H
#include "../includes/G-2301-05-P2-userTools.h"

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

/**
 * @brief rutina que maneja la llegada de la respuesta 352 correspondiente a una solicitud de WHO
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactWhoReply(char* strin);

#endif