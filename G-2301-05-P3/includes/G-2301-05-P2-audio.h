#include "G-2301-05-P2-basicCommandsFromServer.h"
#include "G-2301-05-P1-socket.h"
#include <arpa/inet.h>
#include <unistd.h>
 
#define BUFLEN 160  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data
#define SERVER "127.0.0.1"

/** 
 * @defgroup IRCClient client
 *
 */

/** 
 * @defgroup IRCAudio audio
 * @ingroup IRCClient
 *
 */

/** 
 * @addtogroup IRCAudio
 * Funciones que se encargan del envio de audio entre clientes IRC
 *
 * <hr>
 */

/**
 * @ingroup IRCAudio
 *
 * @page initiateReciever initiateReciever
 *
 * @brief rutina que se encarga de escribir el audio recibido en el buffer
 * tambien crea un hilo que se encargara de, con cierto retardo, reproducir lo que haya en dicho buffer
 *
 * @return NULL
 *
 * <hr>
 */
long int initiateReciever();

/**
 * @ingroup IRCAudio
 *
 * @page initiateSender initiateSender
 *
 * @brief rutina que se encarga del envio de audio
 *
 * @return NULL
 *
 * <hr>
 */
long int initiateSender();

/**
 * @ingroup IRCAudio
 *
 * @page canIRead canIRead
 *
 * @brief funcion que se encarga de comprobar si es posible leer len bytes en el buffer de audio sin "pasar" la posicion de escritura
 * es decir, evalua si, al leer len bytes, leeriamos mas alla de  lo que hay escrito
 *
 * @param len el numero de bytes de distancia a comprobar si podemos o no leer
 *
 * @return 0 si no es posible leer len bytes del buffer. !0 si lo es
 *
 * <hr>
 */
int canIRead(int len);

/**
 * @ingroup IRCAudio
 *
 * @page canIWrite canIWrite
 *
 * @brief funcion que se encarga de comprobar si es posible escribir len bytes en el buffer de audio sin "pasar" la posicion de lectura
 * es decir, evalua si, al escribir len bytes, sobreescribiriamos memoria que aun no ha sido leida
 *
 * @param len el numero de bytes de distancia a comprobar si podemos o no escribir
 *
 * @return 0 si no es posible escribir len bytes del buffer. !0 si lo es
 *
 * <hr>
 */
int canIWrite(int len);

/**
 * @ingroup IRCAudio
 *
 * @page writeBuffer writeBuffer
 *
 * @brief escribe, si es posible, en el buffer a partir de la posición de escritura.
 * Si se logro escribir correctamente, se actualiza la posición de escritura del buffer
 *
 * @param buf los bytes a escribir en el buffer
 * @param len la cantidad de bytes a escribir en el buffer
 *
 * <hr>
 */
void writeBuffer(char* buf, int len);

/**
 * @ingroup IRCAudio
 *
 * @page playBuffer playBuffer
 *
 * @brief reproduce, si es posible, len bytes de audio almacenados en el buffer a partir de la posicion de lectura.
 * Ademas, si se logro reproducir correctamente el audio, se actualiza la posicion de lectura del buffer.
 * es necesario que los canales de reproduccion hayan sido abiertos previamente.
 *
 * @param len el numero de bytes que queremos reproducir
 *
 * <hr>
 */
void playBuffer(int len);

/**
 * @ingroup IRCAudio
 *
 * @page alreadyRecordingQuery alreadyRecordingQuery
 *
 * @brief funcion que devuelve si este cliente ya esta grabando audio o no
 *
 * @return TRUE si este cliente ya esta grabando audio. FALSE si no lo esta
 *
 * <hr>
 */
boolean alreadyRecordingQuery();

/**
 * @ingroup IRCAudio
 *
 * @page endAudioTransmission endAudioTransmission
 *
 * @brief funcion que termina la transmision por parte del cliente que envia audio
 *
 * <hr>
 */
void endAudioTransmission();