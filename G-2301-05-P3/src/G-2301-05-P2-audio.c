#include "../includes/G-2301-05-P2-audio.h"

boolean grabandoAudio;
#define AUDIO_BUFLEN 4096
char audioBuffer[AUDIO_BUFLEN];//almacena hasta 600 trozos de 160 bytes
int writePos;
int readPos;

/**
 * @brief rutina que maneja el hilo que reproduce el audio
 * el hilo empezara a reproducir el audio almacenado en el buffer con cierto retardo para permitir que haya
 * algo escrito antes de empezar a intentar a reproducir. de la misma manera, si por algun motivo este hilo
 * alcanza al que escribe en el buffer, esperara cierto tiempo para dejar que el que escribe le adelante
 *
 * @param args los argumentos que el hilo requiere para detectar cuando debe terminar de reproducir audio
 *
 * @return NULL
 */
void* playThread(void * args){
    boolean *stillRunning;
    //useconds_t time = 20;

    if(args == NULL)
        return NULL;
    usleep(20000);
    stillRunning = (boolean*)args;
    IRCSound_PlayFormat(PA_SAMPLE_ALAW,1);
    IRCSound_OpenPlay();
    while(*stillRunning == TRUE){
        if(canIRead(BUFLEN) != 0)
            playBuffer(BUFLEN);
        else
            usleep(20000);
    }
    IRCSound_ClosePlay();
    return NULL;
}

/**
 * @brief rutina que se encarga de escribir el audio recibido en el buffer
 * tambien crea un hilo que se encargara de, con cierto retardo, reproducir lo que haya en dicho buffer
 *
 * @return NULL
 */
long int initiateReciever(){
    struct sockaddr_in si_me, si_other;
    pthread_t player;
    void *threadReturn;
    boolean *stillRunning = (boolean*)malloc(sizeof(boolean));
    if(stillRunning == NULL){
        return logIntError(-1, "error @ initiateReciever -> malloc");
    }
    *stillRunning = TRUE;
    int s;
    char buf[BUFLEN+1];
    memset(audioBuffer, 0, AUDIO_BUFLEN);
    socklen_t slen = sizeof(si_other);
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        return logIntError(-1, "error @ initiateReciever -> socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        close(s);
        return logIntError(-1, "error @ initiateReciever -> bind");
    }
    writePos = 0;
    readPos = 0;
    pthread_create(&player, NULL, &playThread, stillRunning);
    while(1)
    {
        memset(buf, 0, BUFLEN);
    	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            close(s);
            stillRunning = FALSE;
            pthread_join(player, &threadReturn);
            return logIntError(-1, "error @ initiateReciever -> recvFrom");
        }
    	if(canIWrite(BUFLEN) != 0){
            writeBuffer(buf, BUFLEN);
        }
    }
    
    close(s);
    return 0;
}

/**
 * @brief rutina que se encarga del envio de audio
 *
 * @return NULL
 */
long int initiateSender(){
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    //char buf[BUFLEN];
    char message[BUFLEN+1];
    memset(audioBuffer, 0, AUDIO_BUFLEN);
 
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        return logIntError(-1, "error @ initiateSender -> socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        close(s);
        return logIntError(-1, "error @ initiateSender -> inet_aton");
    }
 
    IRCSound_RecordFormat(PA_SAMPLE_ALAW,1);
    IRCSound_OpenRecord();
    grabandoAudio = TRUE;
    while(grabandoAudio == TRUE)
    {
        IRCSound_RecordSound(message,BUFLEN);
        if (sendto(s, message, BUFLEN , 0 , (struct sockaddr *) &si_other, slen)==-1)
            break;
    }
    IRCSound_CloseRecord();
    close(s);
    return 0;
}

/**
 * @brief funcion que se encarga de comprobar si es posible leer len bytes en el buffer de audio sin "pasar" la posicion de escritura
 * es decir, evalua si, al leer len bytes, leeriamos mas alla de  lo que hay escrito
 *
 * @param len el numero de bytes de distancia a comprobar si podemos o no leer
 *
 * @return 0 si no es posible leer len bytes del buffer. !0 si lo es
 */
int canIRead(int len){//puedo leer si la distancia entre readPos y writePos, es menor que len
    int beforeMoving, afterMoving;
    beforeMoving = readPos;
    afterMoving = (readPos + len)%AUDIO_BUFLEN;
    //no puedo leer si beforeMoving < writePos < afterMoving
    return !((beforeMoving < writePos && writePos <= afterMoving) 
        || (writePos <= afterMoving && afterMoving < beforeMoving));
}

/**
 * @brief funcion que se encarga de comprobar si es posible escribir len bytes en el buffer de audio sin "pasar" la posicion de lectura
 * es decir, evalua si, al escribir len bytes, sobreescribiriamos memoria que aun no ha sido leida
 *
 * @param len el numero de bytes de distancia a comprobar si podemos o no escribir
 *
 * @return 0 si no es posible escribir len bytes del buffer. !0 si lo es
 */
int canIWrite(int len){
    int beforeMoving, afterMoving;
    beforeMoving = writePos;
    afterMoving = (writePos + len)%AUDIO_BUFLEN;
    //no puedo leer si beforeMoving < writePos < afterMoving
    return !((beforeMoving < readPos && readPos <= afterMoving) 
        || (readPos <= afterMoving && afterMoving < beforeMoving));
}

/**
 * @brief escribe, si es posible, en el buffer a partir de la posición de escritura.
 * Si se logro escribir correctamente, se actualiza la posición de escritura del buffer
 *
 * @param buf los bytes a escribir en el buffer
 * @param len la cantidad de bytes a escribir en el buffer
 */
void writeBuffer(char* buf, int len){
    int topper;
    /*
    if(canIWrite(len) == 0)
        return;
    */
    //memcpy(audioBuffer, buf, len);
    if(writePos + len < AUDIO_BUFLEN){
        memcpy(audioBuffer + writePos, buf, len);
        writePos += len;
    } else {
    topper = AUDIO_BUFLEN - writePos;
    memcpy(audioBuffer + writePos, buf, topper);
    memcpy(audioBuffer, buf + topper, len - topper);
    writePos = (writePos + len + 1) % AUDIO_BUFLEN;
    }
}

/**
 * @brief reproduce, si es posible, len bytes de audio almacenados en el buffer a partir de la posicion de lectura.
 * Ademas, si se logro reproducir correctamente el audio, se actualiza la posicion de lectura del buffer.
 * es necesario que los canales de reproduccion hayan sido abiertos previamente.
 *
 * @param len el numero de bytes que queremos reproducir
 */
void playBuffer(int len){
    char myBuffer[BUFLEN+1];
    int topper;
    if(canIRead(len) == 0)
        return;
    if(readPos + len < AUDIO_BUFLEN){
        memcpy(myBuffer, audioBuffer + readPos, len);
        readPos += len;
    } else {
        topper = AUDIO_BUFLEN - readPos;
        memcpy(myBuffer, audioBuffer + readPos, topper);
        memcpy(myBuffer + topper, audioBuffer, len - topper);
        readPos = (readPos + len + 1) % AUDIO_BUFLEN;
    }
    IRCSound_PlaySound(myBuffer,BUFLEN);
}

/**
 * @brief funcion que devuelve si este cliente ya esta grabando audio o no
 *
 * @return TRUE si este cliente ya esta grabando audio. FALSE si no lo esta
 */
boolean alreadyRecordingQuery(){
    return grabandoAudio;
}

/**
 * @brief funcion que termina la transmision por parte del cliente que envia audio
 */
void endAudioTransmission(){
    grabandoAudio = FALSE;
}