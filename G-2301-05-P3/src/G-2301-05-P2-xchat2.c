#include <redes2/ircxchat.h>
#include "../includes/G-2301-05-P1-socket.h"
#include "../includes/G-2301-05-P2-audio.h"
#include "../includes/G-2301-05-P1-tools.h"
#include "../includes/G-2301-05-P1-types.h"
#include "../includes/G-2301-05-P1-commands.h"
#include "../includes/G-2301-05-P2-userCommands.h"
#include "../includes/G-2301-05-P2-userTools.h"
#include "../includes/G-2301-05-P2-basicCommandsFromServer.h"
#include "../includes/G-2301-05-P2-repliesFromServer.h"

struct threadSendArgs {
	char* filename;
	char* nick;
	char* data;
	long unsigned int length;
};
typedef long int (*pFuncs)(char* strin);
typedef long int (*pUserFuncs)(int socket, char* strin);
pFuncs functs[IRC_MAX_COMMANDS];
pUserFuncs userFuncts[IRC_MAX_USER_COMMANDS];
char* miNick;
/**
 * @defgroup IRCInterface Interface
 *
 */

/**
 * @defgroup IRCInterfaceCallbacks Callbaks
 * @ingroup IRCInterface
 *
 */

/**
 * @addtogroup IRCInterfaceCallbacks
 * Funciones que van a ser llamadas desde el interface y que deben ser implementadas por el usuario.
 * Todas estas funciones pertenecen al hilo del interfaz.
 *
 * El programador puede, por supuesto, separar todas estas funciones en múltiples ficheros a
 * efectos de desarrollo y modularización.
 *
 * <hr>
 */

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateChannelKey IRCInterface_ActivateChannelKey
 *
 * @brief Llamada por el botón de activación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateChannelKey (char *channel, char * key)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la clave del canal. El segundo parámetro es
 * la clave del canal que se desea poner. Si es NULL deberá impedirse la activación
 * con la función implementada a tal efecto. En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a activar la clave.
 * @param[in] key clave para el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateChannelKey(char *channel, char *key) {
	char* command = NULL;
	if (key) {
		if (IRCMsg_Mode(&command, NULL, channel, "+k", key) != IRC_OK)
			logVoidError("IRCInterface_ActivateChannelKey -> IRCMsg_Mode");
		if (send(sockfd, command, strlen(command), 0) == -1)
			logVoidError("IRCInterface_ActivateChannelKey -> send");
		IRCInterface_DeleteModeChannel(channel, IRCMODE_CHANNELPASSWORD);
		free(command);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateExternalMessages IRCInterface_ActivateExternalMessages
 *
 * @brief Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateExternalMessages (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la recepción de mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateExternalMessages(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+n", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivateExternalMessages -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateExternalMessages -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_NOEXTERNALMESSAGES);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateInvite IRCInterface_ActivateInvite
 *
 * @brief Llamada por el botón de activación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateInvite (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateInvite(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+i", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivateInvite -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateInvite -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_INVITEONLY);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateModerated IRCInterface_ActivateModerated
 *
 * @brief Llamada por el botón de activación de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateModerated (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateModerated(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+m", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivateModerated -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateModerated -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_MODERATED);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateNicksLimit IRCInterface_ActivateNicksLimit
 *
 * @brief Llamada por el botón de activación del límite de usuarios en el canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateNicksLimit (char *channel, int * limit)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación del límite de usuarios en el canal. El segundo es el
 * límite de usuarios que se desea poner. Si el valor es 0 se sobrentiende que se desea eliminar
 * este límite.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se activará el límite de usuarios.
 * @param[in] limit límite de usuarios en el canal indicado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateNicksLimit(char *channel, int limit) {
	char* command = NULL;
	char limite[20];

	snprintf(limite, 20, "%d", limit);
	if (IRCMsg_Mode(&command, NULL, channel, "+l", limite) != IRC_OK)
		logVoidError("IRCInterface_ActivateNicksLimit -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateNicksLimit -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_USERSLIMIT);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivatePrivate IRCInterface_ActivatePrivate
 *
 * @brief Llamada por el botón de activación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivatePrivate (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivatePrivate(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+p", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivatePrivate -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivatePrivate -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_PRIVATE);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateProtectTopic IRCInterface_ActivateProtectTopic
 *
 * @brief Llamada por el botón de activación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateProtectTopic (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de la protección de tópico.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar la protección de tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateProtectTopic(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+t", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivateProtectTopic -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateProtectTopic -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_TOPICOP);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ActivateSecret IRCInterface_ActivateSecret
 *
 * @brief Llamada por el botón de activación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ActivateSecret (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de activación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a activar el estado de secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_ActivateSecret(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+s", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivateSecret -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateSecret -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_SECRET);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_BanNick IRCInterface_BanNick
 *
 * @brief Llamada por el botón "Banear".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_BanNick (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Banear". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a realizar el baneo. En principio es un valor innecesario.
 * @param[in] nick nick del usuario que va a ser baneado
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_BanNick(char *channel, char *nick) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+b", nick) != IRC_OK)
		logVoidError("IRCInterface_BanNick -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_BanNick -> send");
	IRCInterface_DeleteModeChannel(channel, IRCMODE_BANMASK);
	free(command);

}

void* whoThread(void * args) {
	int socket, num, i;
	char**channels, *command;
	if (args == NULL)
		return NULL;
	socket = *((int*) args);
	pthread_detach(pthread_self());
	while (1) {
		IRCInterface_ListAllChannelsThread(&channels, &num);
		for (i = 0; i < num; i++) {
			IRCMsg_Who(&command, NULL, channels[i], NULL);
			if (send(socket, command, strlen(command), 0) < 0) {
				IRCInterface_FreeListAllChannelsThread(channels, num);
				free(command);
				free(args);
				return logPointerError(NULL, "error @ whoThread -> send");
			}
			free(command);
		}
		IRCInterface_FreeListAllChannelsThread(channels, num);
		sleep(10);
	}
	free(args);
	return NULL;
}

void* clientThread(void* args) {
	int byteCount, sockfd, *aux;
	char* command = NULL;
	char buffer[10000];
	char *strPos;
	long int commandNumber;

	pthread_detach(pthread_self());

	if (args == NULL)
		return logPointerError(NULL, "error @ clientRoutine: NULL pointer argument");
	aux = (int*) args;
	sockfd = *aux;
	free(aux);

	while (1) {
		bzero(buffer, 10000);
		byteCount = recv(sockfd, buffer, 10000, 0);
		if (byteCount == 0)
			return logPointerError(NULL, "retrieveMsg exited");
		else if (byteCount == -1)
			return logPointerError(NULL, "error @ retrieveMsg -> recv");

		strPos = buffer;
		while (strPos != NULL) {
			strPos = IRC_UnPipelineCommands(strPos, &command);
			IRCInterface_WriteSystemThread(NULL, command);
			if (command != NULL) {
				commandNumber = IRC_CommandQuery(command);
				IRCInterface_PlaneRegisterOutMessageThread(command);
				if (commandNumber >= 0 && functs[commandNumber](command) != IRC_OK)//funcion que reacciona al mensaje de un servidor
					logPointerError(NULL, "error @ retrieveMsg -> pFuncs");
			}
			IRC_MFree(1, &command);
		}
	}
	return NULL;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_Connect IRCInterface_Connect
 *
 * @brief Llamada por los distintos botones de conexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	long IRCInterface_Connect (char *nick, char * user, char * realname, char * password, char * server, int port, boolean ssl)
 * @endcode
 *
 * @description
 * Función a implementar por el programador.
 * Llamada por los distintos botones de conexión. Si implementará la comunicación completa, incluido
 * el registro del usuario en el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leída.
 *
 *
 * @param[in] nick nick con el que se va a realizar la conexíón.
 * @param[in] user usuario con el que se va a realizar la conexión.
 * @param[in] realname nombre real con el que se va a realizar la conexión.
 * @param[in] password password del usuario si es necesaria, puede valer NULL.
 * @param[in] server nombre o ip del servidor con el que se va a realizar la conexión.
 * @param[in] port puerto del servidor con el que se va a realizar la conexión.
 * @param[in] ssl puede ser TRUE si la conexión tiene que ser segura y FALSE si no es así.
 *
 * @retval IRC_OK si todo ha sido correcto (debe devolverlo).
 * @retval IRCERR_NOSSL si el valor de SSL es TRUE y no se puede activar la conexión SSL pero sí una
 * conexión no protegida (debe devolverlo).
 * @retval IRCERR_NOCONNECT en caso de que no se pueda realizar la comunicación (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */
long IRCInterface_Connect(char *nick, char *user, char *realname, char *password, char *server, int port, boolean ssl) {
	pthread_t th, thWho;
	int *args;
	long ret = 0;
	char* command, *prefix, commandIn[255];


	/*Creo socket*/
	//CONEXION DEL SOCKET
	sockfd = openSocket_TCP();
	if (connectTo(sockfd, server, port) == -1)
		return logIntError(-1, "Error @ IRCInterface_Connect -> connectTo");

	prefix = CLIENTNAME;
	//mando pass, nick y user
	//PASS
	if (password != NULL && *password != 0) {
		//README al introducir cualquier contraseña, el servidor nos devuelve Bad Password ?
		if ((ret = IRCMsg_Pass(&command, prefix, password)) != IRC_OK)
			return logIntError(ret, "Error @ IRCInterface_Connect -> IRCMsg_Nick");
		if (send(sockfd, command, strlen(command), 0) == -1)
			return logIntError(-1, "Error @ IRCInterface_Connect -> send");
		free(command);
	}

	//NICK
	if ((ret = IRCMsg_Nick(&command, prefix, nick, NULL)) != IRC_OK)
		return logIntError(ret, "Error @ IRCInterface_Connect -> IRCMsg_Nick");
	if (send(sockfd, command, strlen(command), 0) == -1)
		return logIntError(-1, "Error @ IRCInterface_Connect -> send");
	free(command);

	//USER
	if ((ret = IRCMsg_User(&command, prefix, user, "o", realname)) != IRC_OK)
		return logIntError(ret, "Error @ IRCInterface_Connect -> IRCMsg_User");
	if (send(sockfd, command, strlen(command), 0) == -1)
		return logIntError(-1, "Error @ IRCInterface_Connect -> send");
	free(command);

	do {
		if (recv(sockfd, commandIn, 255, 0) <= 0)
			return logIntError(-1, "Error @ IRCInterface_Connect -> recv");
	} while (IRC_CommandQuery(commandIn) == 17);

	if (IRC_CommandQuery(commandIn) != 183) {
		IRCInterface_ErrorDialog(commandIn);
		return -1;
	}

	args = (int*) malloc(sizeof (int));
	*args = sockfd;
	pthread_create(&th, NULL, &clientThread, args); //este hilo se encargará de recibir los mensajes posteriores del servidor
	args = (int*) malloc(sizeof (int));
	*args = sockfd;
	pthread_create(&thWho, NULL, &whoThread, args); //este hilo se encargará de ir enviando comandos WHO al servidor
	return IRC_OK;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateChannelKey IRCInterface_DeactivateChannelKey
 *
 * @brief Llamada por el botón de desactivación de la clave del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateChannelKey (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la clave del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la clave.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateChannelKey(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-k", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivateChannelKey -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivateChannelKey -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_CHANNELPASSWORD);
	free(command);

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateExternalMessages IRCInterface_DeactivateExternalMessages
 *
 * @brief Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateExternalMessages (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la recepción de mensajes externos.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a deactivar la recepción de mensajes externos.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateExternalMessages(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-n", NULL) != IRC_OK)
		logVoidError("IRCInterface_ActivateExternalMessages -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_ActivateExternalMessages -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_NOEXTERNALMESSAGES);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateInvite IRCInterface_DeactivateInvite
 *
 * @brief Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateInvite (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de canal de sólo invitación.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la invitación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateInvite(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-i", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivateInvite -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivateInvite -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_INVITEONLY);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateModerated IRCInterface_DeactivateModerated
 *
 * @brief Llamada por el botón de desactivación  de la moderación del canal.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateModerated (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación  de la moderación del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la moderación.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateModerated(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-m", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivateModerated -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivateModerated -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_MODERATED);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateNicksLimit IRCInterface_DeactivateNicksLimit
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateNicksLimit (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación  del límite de usuarios en el canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar el límite de usuarios.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateNicksLimit(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-l", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivateNicksLimit -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivateNicksLimit -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_USERSLIMIT);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivatePrivate IRCInterface_DeactivatePrivate
 *
 * @brief Llamada por el botón de desactivación del modo privado.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivatePrivate (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación del modo privado.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @param[in] channel canal sobre el que se va a desactivar la privacidad.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivatePrivate(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-p", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivatePrivate -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivatePrivate -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_PRIVATE);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateProtectTopic IRCInterface_DeactivateProtectTopic
 *
 * @brief Llamada por el botón de desactivación de la protección de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateProtectTopic (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de la protección de tópico.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la protección de tópico.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateProtectTopic(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-t", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivateProtectTopic -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivateProtectTopic -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_TOPICOP);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DeactivateSecret IRCInterface_DeactivateSecret
 *
 * @brief Llamada por el botón de desactivación de canal secreto.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DeactivateSecret (char *channel)
 * @endcode
 *
 * @description
 * Llamada por el botón de desactivación de canal secreto.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] channel canal sobre el que se va a desactivar la propiedad de canal secreto.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_DeactivateSecret(char *channel) {
	char* command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-s", NULL) != IRC_OK)
		logVoidError("IRCInterface_DeactivateSecret -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_DeactivateSecret -> send");
	IRCInterface_AddModeChannel(channel, IRCMODE_SECRET);
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_DisconnectServer IRCInterface_DisconnectServer
 *
 * @brief Llamada por los distintos botones de desconexión.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_DisconnectServer (char * server, int port)
 * @endcode
 *
 * @description
 * Llamada por los distintos botones de desconexión. Debe cerrar la conexión con el servidor.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.

 * @param[in] server nombre o ip del servidor del que se va a realizar la desconexión.
 * @param[in] port puerto sobre el que se va a realizar la desconexión.
 *
 * @retval TRUE si se ha cerrado la conexión (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_DisconnectServer(char *server, int port) {
	userQuit(sockfd, "/quit");
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_ExitAudioChat IRCInterface_ExitAudioChat
 *
 * @brief Llamada por el botón "Cancelar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_ExitAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función cierrala comunicación. Evidentemente tiene que
 * actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario que solicita la parada del chat de audio.
 *
 * @retval TRUE si se ha cerrado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_ExitAudioChat(char *nick) {
	if (alreadyRecordingQuery() == TRUE)
		endAudioTransmission();
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveOp IRCInterface_GiveOp
 *
 * @brief Llamada por el botón "Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveOp (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Op". Previamente debe seleccionarse un nick del
 * canal para darle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va dar op al usuario.
 * @param[in] nick nick al que se le va a dar el nivel de op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_GiveOp(char *channel, char *nick) {
	char*command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+o", nick) != IRC_OK)
		logVoidError("IRCInterface_GiveOp -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_GiveOp -> send");
	free(command);

}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_GiveVoice IRCInterface_GiveVoice
 *
 * @brief Llamada por el botón "Dar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_GiveVoice (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Dar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va dar voz al usuario.
 * @param[in] nick nick al que se le va a dar voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_GiveVoice(char *channel, char *nick) {
	char*command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "+v", nick) != IRC_OK)
		logVoidError("IRCInterface_GiveOp -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_GiveOp -> send");
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_KickNick IRCInterface_KickNick
 *
 * @brief Llamada por el botón "Echar".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_KickNick (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Echar". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a expulsar al usuario.
 * @param[in] nick nick del usuario que va a ser expulsado.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_KickNick(char *channel, char *nick) {
	char *command;
	if (channel == NULL || nick == NULL) return;
	if (IRCMsg_Kick(&command, NULL, channel, nick, "has sido kickeado!") != IRC_OK) {
		logVoidError("error @ IRCInterface_KickNick -> IRCMsg_kick");
		return;
	}
	if (send(sockfd, command, strlen(command), 0) < 0)
		logVoidError("error @ IRCInterface_KickNick -> send");
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewCommandText IRCInterface_NewCommandText
 *
 * @brief Llamada la tecla ENTER en el campo de texto y comandos.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewCommandText (char *command)
 * @endcode
 *
 * @description
 * Llamada de la tecla ENTER en el campo de texto y comandos. El texto deberá ser
 * enviado y el comando procesado por las funciones de "parseo" de comandos de
 * usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] comando introducido por el usuario.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_NewCommandText(char *command) {
	char* target; //punteros que no hay que liberar
	long ret;
	char *Xcom, *msg, *myNick; //punteros que si hay que liberar
	Xcom = msg = NULL;

	if (command[0] != '/') {//significa que el usuario queria enviar un privmsg
		target = IRCInterface_ActiveChannelName();
		if ((ret = IRCMsg_Privmsg(&Xcom, NULL, target, command)) != IRC_OK)
			return logVoidError("error @ IRCInterface_NewCommandText -> IRCMsg_Privmsg");
		send(sockfd, Xcom, strlen(Xcom), 0);
		myNick = getMyNick();
		IRCInterface_WriteChannel(target, myNick, command);
	} else { //significa que el usuario quería enviar un comando
		IRCInterface_WriteSystem(NULL, command);
		if ((ret = IRCUser_CommandQuery(command)) >= 0)
			userFuncts[ret](sockfd, command);
	}
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_NewTopicEnter IRCInterface_NewTopicEnter
 *
 * @brief Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_NewTopicEnter (char * topicdata)
 * @endcode
 *
 * @description
 * Llamada cuando se pulsa la tecla ENTER en el campo de tópico.
 * Deberá intentarse cambiar el tópico del canal.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * param[in] topicdata string con el tópico que se desea poner en el canal.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_NewTopicEnter(char *topicdata) {
	char *command, *channel;
	if (topicdata == NULL) return;
	channel = IRCInterface_ActiveChannelName();
	if (channel == NULL) {
		logVoidError("error @ IRCInterface_NewTopicEnter -> IRCInterface_ActiveChannelName");
		return;
	}
	if (IRCMsg_Topic(&command, NULL, channel, topicdata) != IRC_OK) {
		logVoidError("error @ IRCInterface_NewTopicEnter -> IRCMsg_Topic");
		return;
	}
	if (send(sockfd, command, strlen(command), 0) < 0)
		logVoidError("error @ IRCInterface_NewTopicEnter -> send");
}

/**
 * @brief rutina que se encarga del envio de ficheros siguiendo un "protocolo" particular de este cliente
 * este protocolo funciona de la siguiente manera:
 * primero se envia un privmsg a quien deseemos enviar un fichero (en adelante, el receptor)
 * este privmsg tendra la siguiente forma: \002FSEND \001<<filename>>\001 <<hostname>> <<port>> <<length>>
 *      -donde \002 es un caracter no imprimible que indica que es un privmsg especial que declara que se desea enviar un fichero
 *      -los \001 son tambien caracteres no imprimibles que indican el comienzo y final del nombre del fichero a enviar
 *      -<<filename>> es el nombre del fichero a enviar (puede contener espacios)
 *      -<<hostname>> es el nombre dns de quien desea enviar el fichero (en adelante, el emisor)
 *      -<<port>> es un entero donde se indica el puerto al que el receptor debera conectarse para recibir el fichero
 *      -<<length>> es la longitud del fichero
 * una vez que el receptor haya recibido este mensaje, obtendra los datos del mismo y mediante un prompt, el usuario receptor decidira
 * si quiere o no recibir este fichero. La respuesta a este prompt se enviara a traves de la conexion declarada en el privmsg (hostname + port)
 * en caso de respuesta negativa, esta conexion se acaba para ambas partes.
 * en caso de respuesta afirmativa, el emisor procede a enviar todos los datos del fichero mediante la conexcion declarada
 * por su parte, el receptor reconstruira el fichero con los datos que el emisor envie
 * una vez acabada la transmision, ambas partes cierran la conexion
 *
 * @param args los argumentos necesarios para que el hilo lleve a cabo el envio del fichero. son del tipo "struct threadSendArgs"
 *
 * @return NULL
 */
void* threadSend(void* args) {
	char *data, buffer[512], *command, *nick, *filename, *myHost;
	int port, socket, newSockfd;
	socklen_t slen;
	unsigned long length, index = 0;
	struct threadSendArgs * aux = (struct threadSendArgs *) args;
	struct sockaddr_in serv, client;
	boolean answer;

	pthread_detach(pthread_self());
	if (aux == NULL)
		return NULL;
	data = aux->data;
	length = aux->length;
	nick = aux->nick;
	filename = aux->filename;
	//TODO: cambiar la siguiente linea para hacerla mas general (?)
	myHost = "localhost";
	socket = openSocket_TCP();
	if (socket < 0) {
		IRC_MFree(2, &data, &args);
		return NULL;
	}
	listen(socket, 1);
	slen = sizeof (serv);
	getsockname(socket, (struct sockaddr*) &serv, &slen);
	port = ntohs(serv.sin_port);

	sprintf(buffer, "\002FSEND \001%s\001 %s %d %lu", filename, myHost, port, length);
	IRCMsg_Privmsg(&command, NULL, nick, buffer);
	if (send(sockfd, command, strlen(command), 0) < 0) {
		IRC_MFree(3, &data, &args, &command);
		close(socket);
		return NULL;
	}
	newSockfd = acceptConnection(socket, &client);
	if (newSockfd < 0) {
		logVoidError("error @ threadSend -> acceptConnection");
		IRC_MFree(3, &data, &args, &command);
		close(socket);
		return NULL;
	}
	if (recv(newSockfd, &answer, sizeof (answer), 0) <= 0) {
		IRC_MFree(3, &data, &args, &command);
		close(socket);
		return NULL;
	}
	if (answer == FALSE) {
		IRC_MFree(3, &data, &args, &command);
		close(socket);
		return NULL;
	}
	while (length - index >= FILE_BUFLEN) {
		send(newSockfd, data + index, FILE_BUFLEN, 0);
		index += FILE_BUFLEN;
	}
	if (length > index)
		send(newSockfd, data + index, length - index, 0);
	IRC_MFree(3, &data, &args, &command);
	close(socket);
	return NULL;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_SendFile IRCInterface_SendFile
 *
 * @brief Llamada por el botón "Enviar Archivo".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_SendFile (char * filename, char *nick, char *data, long unsigned int length)
 * @endcode
 *
 * @description
 * Llamada por el botón "Enviar Archivo". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto es el programador el que debe determinar si crea un nuevo hilo para enviar el archivo o
 * no lo hace.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] filename nombre del fichero a enviar.
 * @param[in] nick nick del usuario que enviará el fichero.
 * @param[in] data datos a ser enviados.
 * @param[in] length longitud de los datos a ser enviados.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_SendFile(char *filename, char *nick, char *data, long unsigned int length) {
	struct threadSendArgs *args;
	pthread_t th;

	args = (struct threadSendArgs*) malloc(sizeof (struct threadSendArgs));
	if (args == NULL) return FALSE;

	args->data = data;
	args->length = length;
	args->filename = filename;
	args->nick = nick;
	pthread_create(&th, NULL, threadSend, args);
	return TRUE;
}

/**
 * @brief rutina que maneja el hilo que graba la voz del emisor en un envio de audio
 *
 * @param args los argumentos que el hilo requiere. Como no requiere ninguno, este parametro puede ser NULL
 *
 * @return NULL
 */
void * threadRecord(void * aux) {
	int socket, port, newSockfd;
	socklen_t slen;
	struct sockaddr_in serv, client;
	boolean answer;
	char *nick, buffer[512], *command;

	pthread_detach(pthread_self());
	if (aux == NULL)
		return NULL;
	if (alreadyRecordingQuery() == TRUE)
		return NULL;

	socket = openSocket_TCP();
	if (socket < 0) {
		IRC_MFree(1, &aux);
		return NULL;
	}
	listen(socket, 1);
	slen = sizeof (serv);
	getsockname(socket, (struct sockaddr*) &serv, &slen);
	port = ntohs(serv.sin_port);

	nick = (char*) aux;
	sprintf(buffer, "\001FAUDIO %s %d", "localhost", port);
	IRCMsg_Privmsg(&command, NULL, nick, buffer);
	//enviamos el privmsg al servidor para que lo reenvie al otro cliente. Si falla el send, no podemos continuar
	if (send(sockfd, command, strlen(command), 0) < 0) {
		close(socket);
		return NULL;
	}
	newSockfd = acceptConnection(socket, &client);
	if (newSockfd < 0) {
		logVoidError("error @ threadRecord -> acceptConnection");
		close(socket);
		return NULL;
	}
	if (recv(newSockfd, &answer, sizeof (boolean), 0) <= 0) {
		close(socket);
		close(newSockfd);
		return NULL;
	}
	if (ntohs(answer) == FALSE) {
		close(socket);
		close(newSockfd);
		return NULL;
	}
	close(socket);
	close(newSockfd);
	initiateSender();
	return NULL;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StartAudioChat IRCInterface_StartAudioChat
 *
 * @brief Llamada por el botón "Iniciar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StartAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Iniciar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función como todos los demás callbacks bloquea el interface
 * y por tanto para mantener la funcionalidad del chat de voz es imprescindible crear un hilo a efectos
 * de comunicación de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario con el que se desea conectar.
 *
 * @retval TRUE si se ha establecido la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_StartAudioChat(char *nick) {
	pthread_t th;
	pthread_create(&th, NULL, threadRecord, nick);
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_StopAudioChat IRCInterface_StopAudioChat
 *
 * @brief Llamada por el botón "Parar" del diálogo de chat de voz.
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_StopAudioChat (char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Parar" del diálogo de chat de voz. Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario. Esta función sólo para la comunicación que puede ser reiniciada.
 * Evidentemente tiene que actuar sobre el hilo de chat de voz.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * La string recibida no debe ser manipulada por el programador, sólo leída.
 *
 * @param[in] nick nick del usuario con el que se quiere parar el chat de voz.
 *
 * @retval TRUE si se ha parado la comunicación (debe devolverlo).
 * @retval FALSE en caso contrario (debe devolverlo).
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

boolean IRCInterface_StopAudioChat(char *nick) {
	if (alreadyRecordingQuery())
		endAudioTransmission();
	return TRUE;
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeOp IRCInterface_TakeOp
 *
 * @brief Llamada por el botón "Quitar Op".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeOp (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Quitar Op". Previamente debe seleccionarse un nick del
 * canal para quitarle "op" a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se va a quitar op al usuario.
 * @param[in] nick nick del usuario al que se le va a quitar op.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_TakeOp(char *channel, char *nick) {
	char*command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-o", nick) != IRC_OK)
		logVoidError("IRCInterface_TakeOp -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_TakeOp -> send");
	free(command);
}

/**
 * @ingroup IRCInterfaceCallbacks
 *
 * @page IRCInterface_TakeVoice IRCInterface_TakeVoice
 *
 * @brief Llamada por el botón "Quitar voz".
 *
 * @synopsis
 * @code
 *	#include <redes2/ircxchat.h>
 *
 * 	void IRCInterface_TakeVoice (char *channel, char *nick)
 * @endcode
 *
 * @description
 * Llamada por el botón "Quitar voz". Previamente debe seleccionarse un nick del
 * canal para darle voz a dicho usuario.
 *
 * En cualquier caso sólo se puede realizar si el servidor acepta la orden.
 * Las strings recibidas no deben ser manipuladas por el programador, sólo leídas.
 *
 * @param[in] channel canal sobre el que se le va a quitar voz al usuario.
 * @param[in] nick nick del usuario al que se va a quitar la voz.
 *
 * @warning Esta función debe ser implementada por el alumno.
 *
 * @author
 * Eloy Anguiano (eloy.anguiano@uam.es)
 *
 *<hr>
 */

void IRCInterface_TakeVoice(char *channel, char *nick) {
	char*command = NULL;
	if (IRCMsg_Mode(&command, NULL, channel, "-v", nick) != IRC_OK)
		logVoidError("IRCInterface_GiveOp -> IRCMsg_Mode");
	if (send(sockfd, command, strlen(command), 0) == -1)
		logVoidError("IRCInterface_GiveOp -> send");
	free(command);
}


/***************************************************************************************************/
/***************************************************************************************************/
/**                                                                                               **/
/** MMMMMMMMMM               MMMMM           AAAAAAA           IIIIIII NNNNNNNNNN          NNNNNN **/
/**  MMMMMMMMMM             MMMMM            AAAAAAAA           IIIII   NNNNNNNNNN          NNNN  **/
/**   MMMMM MMMM           MM MM            AAAAA   AA           III     NNNNN NNNN          NN   **/
/**   MMMMM  MMMM         MM  MM            AAAAA   AA           III     NNNNN  NNNN         NN   **/
/**   MMMMM   MMMM       MM   MM           AAAAA     AA          III     NNNNN   NNNN        NN   **/
/**   MMMMM    MMMM     MM    MM           AAAAA     AA          III     NNNNN    NNNN       NN   **/
/**   MMMMM     MMMM   MM     MM          AAAAA       AA         III     NNNNN     NNNN      NN   **/
/**   MMMMM      MMMM MM      MM          AAAAAAAAAAAAAA         III     NNNNN      NNNN     NN   **/
/**   MMMMM       MMMMM       MM         AAAAA         AA        III     NNNNN       NNNN    NN   **/
/**   MMMMM        MMM        MM         AAAAA         AA        III     NNNNN        NNNN   NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN         NNNN  NN   **/
/**   MMMMM                   MM        AAAAA           AA       III     NNNNN          NNNN NN   **/
/**  MMMMMMM                 MMMM     AAAAAA            AAAA    IIIII   NNNNNN           NNNNNNN  **/
/** MMMMMMMMM               MMMMMM  AAAAAAAA           AAAAAA  IIIIIII NNNNNNN            NNNNNNN **/
/**                                                                                               **/
/***************************************************************************************************/

/***************************************************************************************************/



int main(int argc, char *argv[]) {
	/* La función IRCInterface_Run debe ser llamada al final      */
	/* del main y es la que activa el interfaz gráfico quedándose */
	/* en esta función hasta que se pulsa alguna salida del       */
	/* interfaz gráfico.
	 *                                          */
	int i;
	//por default imprime en el rawlog y ya
	for (i = 0; i < IRC_MAX_COMMANDS; i++)
		functs[i] = reactDefault;
	//basic
	functs[MSG_PASS] = reactPass;
	functs[MSG_NICK] = reactNick;
	functs[MSG_MODE] = reactMode;
	functs[MSG_SERVICE] = reactService;
	functs[MSG_QUIT] = reactQuit;
	functs[MSG_JOIN] = reactJoin;
	functs[MSG_PART] = reactPart;
	functs[MSG_TOPIC] = reactTopic;
	functs[MSG_NAMES] = reactNames;
	functs[MSG_KICK] = reactKick;
	functs[MSG_PRIVMSG] = reactPrivmsg;
	functs[MSG_PING] = reactPing;
	functs[MSG_SETNAME] = reactSetName;

	//replies

	functs[RPL_CHANNELMODEIS] = reactModeQuery;
	functs[RPL_NOTOPIC] = reactNoTopic;
	functs[RPL_TOPIC] = reactTopicQuery;
	functs[RPL_WHOREPLY] = reactWhoReply;

	functs[RPL_YOURHOST] = reactPrint;
	functs[RPL_YOURESERVICE ] = reactPrint;
	functs[RPL_MOTD] = reactPrint;
	functs[RPL_CREATED] = reactPrint;
	functs[RPL_WELCOME] = reactPrint;

	//RPL_MYINFO    RPL_INFO    RPL_ENDOFINFO
	//LUSER??

	functs[RPL_WHOISUSER] = reactPrint;
	functs[RPL_WHOISCHANNELS] = reactPrint;
	functs[RPL_WHOISOPERATOR] = reactPrint;
	functs[RPL_WHOISSERVER] = reactPrint;
	functs[RPL_WHOISIDLE] = reactPrint;

	functs[RPL_LISTSTART ] = reactPrint;
	functs[RPL_MOTDSTART] = reactPrint;

	functs[RPL_ENDOFMOTD] = reactPrint;
	functs[RPL_ENDOFWHO] = reactPrint;
	functs[RPL_ENDOFWHOIS ] = reactPrint;
	functs[RPL_ENDOFNAMES ] = reactPrint;
	functs[RPL_LISTEND] = reactPrint;
	for (i = 0; i < IRC_MAX_USER_COMMANDS; i++)
		userFuncts[i] = userDefault;

	userFuncts[UJOIN] = userJoin;
	userFuncts[UMSG] = userPriv;
	userFuncts[UAWAY] = userAway;
	userFuncts[UKICK] = userKick;
	userFuncts[ULIST] = userList;
	userFuncts[UMODE] = userMode;
	userFuncts[UMOTD] = userMotd;
	userFuncts[UNAMES] = userNames;
	userFuncts[UNICK] = userNick;
	userFuncts[UPART] = userPart;
	userFuncts[UQUIT] = userQuit;
	userFuncts[UTOPIC] = userTopic;
	userFuncts[UWHOIS] = userWhois;
	userFuncts[UWHO] = userWho;

	IRCInterface_Run(argc, argv);

	return 0;
}