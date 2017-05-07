#include "../includes/G-2301-05-P2-repliesFromServer.h"
//##############################################################################################
//##############################################################################################
//---------------------------------------RESPUESTAS---------------------------------------------
//##############################################################################################
//##############################################################################################

/**
 * @brief rutina que maneja la llegada de la respuesta 332 correspondiente a solicitar los modos de un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactModeQuery(char* strin) {
    long ret;
    char *prefix, *nick, *channel, *modetxt, info[512];
    if ((ret = IRCParse_RplChannelModeIs(strin, &prefix, &nick, &channel, &modetxt)) != IRC_OK)//rpl 324
        return logIntError(ret, "error @ reactModeQuery -> IRCParse_RplChannelModeIs");
    sprintf(info, "canal %s modos: %s", channel, modetxt);
    info[511] = 0;
    printXchat(channel, NULL, info, TRUE);
    IRCInterface_AddModeChannelThread (channel, IRCInterface_ModeToIntModeThread(modetxt));
    IRC_MFree(4, &prefix, &nick, &channel, &modetxt);
    return IRC_OK;
}
/**
 * @brief rutina que maneja la llegada de la respuesta 331 correspondiente a notificar que no hay topic en un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactNoTopic(char* strin) {
    long ret;
    char *prefix, *nick, *channel, *topic, info[512];
    if ((ret = IRCParse_RplNoTopic(strin, &prefix, &nick, &channel, &topic)) != IRC_OK)
        return logIntError(ret, "error @ reactNoTopic -> IRCParse_RplNoTopic");
    sprintf(info, "%s: no topic is set", channel);
    printXchat(channel, NULL, info, TRUE);
    IRC_MFree(4, &prefix, &nick, &channel, &topic);

    return IRC_OK;
}

/**
 * @brief rutina que maneja la llegada de la respuesta 332 correspondiente a una solicitud del topic de un canal
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactTopicQuery(char* strin) {
    long ret;
    char*prefix, *nick, *channel, *topic;
    if ((ret = IRCParse_RplTopic(strin, &prefix, &nick, &channel, &topic)) != IRC_OK)
        return logIntError(ret, "error @ reactTopicQuery -> IRCParse_RplTopic");
    printXchat(channel, NULL, strin, TRUE);
    IRCInterface_SetTopicThread(topic);
    IRC_MFree(4, &prefix, &nick, &channel, &topic);
    return IRC_OK;
}

/**
 * @brief rutina que maneja la llegada de la respuesta 352 correspondiente a una solicitud de WHO
 *
 * @param strin el mensaje que envia el servidor
 *
 * @return IRC_OK
 */
long reactWhoReply(char* strin) {
    nickstate ns;
    long ret = 0;
    char *channel, *nick, *user, *realname, *host, *prefix, *nick2, *type, *msg, *server;
    int hopcount = 0;
    prefix = nick = channel = user = host = server = nick2 = type = msg = realname = NULL;
    if ((ret = IRCParse_RplWhoReply(strin, &prefix, &nick, &channel, &user,
            &host, &server, &nick2, &type, &msg, &hopcount, &realname)) != IRC_OK)
        return logIntError(ret, "error @ reactWhoIsUser -> IRCParse_RplWhoIsUser");
    if (strchr(type, '@'))
        ns = OPERATOR;
    else if (strchr(type, '+'))
        ns = VOICE;
    else
        ns = NONE;
    IRCInterface_AddNickChannelThread(channel, nick2, user, realname, host, ns);
    IRC_MFree(10, &prefix, &nick, &channel, &user, &host, &server, &nick2, &type, &msg, &realname);
    return IRC_OK;
}
