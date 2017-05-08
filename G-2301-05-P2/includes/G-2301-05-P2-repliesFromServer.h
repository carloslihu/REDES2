#ifndef _REPLIESFROMSERVER_H
#define _REPLIESFROMSERVER_H
#include "../includes/G-2301-05-P2-userTools.h"

/** 
 * @defgroup IRCClient client
 *
 */

/** 
 * @defgroup IRCrepliesFromServer repliesFromServer
 * @ingroup IRCClient
 *
 */

/** 
 * @addtogroup IRCrepliesFromServer
 * Funciones que manejan las respuestas numericas del servidor
 *
 * <hr>
 */

/**
 * @ingroup IRCrepliesFromServer
 *
 * @page reactModeQuery reactModeQuery
 *
 * @brief rutina que maneja la llegada de la respuesta 332 correspondiente a solicitar los modos de un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactModeQuery(char* strin);

/**
 * @ingroup IRCrepliesFromServer
 *
 * @page reactNoTopic reactNoTopic
 *
 * @brief rutina que maneja la llegada de la respuesta 331 correspondiente a notificar que no hay topic en un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactNoTopic(char* strin);

/**
 * @ingroup IRCrepliesFromServer
 *
 * @page reactTopicQuery reactTopicQuery
 *
 * @brief rutina que maneja la llegada de la respuesta 332 correspondiente a una solicitud del topic de un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactTopicQuery(char* strin);

/**
 * @ingroup IRCrepliesFromServer
 *
 * @page reactWhoReply reactWhoReply
 *
 * @brief rutina que maneja la llegada de la respuesta 352 correspondiente a una solicitud de WHO
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 *
 * <hr>
 */
long reactWhoReply(char* strin);

#endif