#include "../includes/G-2301-05-P2-audio.h"

boolean grabandoAudio;
#define AUDIO_BUFLEN 96000
char audioBuffer[AUDIO_BUFLEN];//almacena hasta 6 trozos de 160 bytes
int writePos;
int readPos;

void* playThread(void * args){
    boolean *stillRunning;
    //useconds_t time = 20;

    if(args == NULL)
        return NULL;
    usleep(20000);
    stillRunning = (boolean*)args;
    IRCSound_PlayFormat(PA_SAMPLE_S16BE,2);
    IRCSound_OpenPlay();
    //printf("soy el hilo que reproduce audio\n");
    while(*stillRunning == TRUE){
        if(canIRead(BUFLEN) != 0)
            playBuffer(BUFLEN);
        else
            usleep(20000);
    }
    IRCSound_ClosePlay();
    return NULL;
}

long int initiateReciever(){
    struct sockaddr_in si_me, si_other;
    pthread_t player;
    void *threadReturn;
    boolean stillRunning = TRUE;
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
    pthread_create(&player, NULL, &playThread, &stillRunning);
    while(1)
    {
    	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            close(s);
            stillRunning = FALSE;
            pthread_join(player, &threadReturn);
            return logIntError(-1, "error @ initiateReciever -> recvFrom");
        }
    	if(canIWrite(BUFLEN) != 0){
            //printf("I'm writing shit\n");
            writeBuffer(buf, BUFLEN);
        }
    }
    
    close(s);
    return 0;
}

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
 
    IRCSound_RecordFormat(PA_SAMPLE_S16BE,2);
    IRCSound_OpenRecord();
    grabandoAudio = TRUE;
    while(grabandoAudio == TRUE)
    {
        IRCSound_RecordSound(message,BUFLEN);
        if (sendto(s, message, BUFLEN , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            break;
        }
    }
    IRCSound_CloseRecord();
    close(s);
    return 0;
}

int canIRead(int len){//puedo leer si la distancia entre readPos y writePos, es menor que len
    int beforeMoving, afterMoving;
    beforeMoving = readPos;
    afterMoving = (readPos + len)%AUDIO_BUFLEN;
    //no puedo leer si beforeMoving < writePos < afterMoving
    return !((beforeMoving < writePos && writePos <= afterMoving) 
        || (writePos <= afterMoving && afterMoving < beforeMoving));
}

int canIWrite(int len){
    int beforeMoving, afterMoving;
    beforeMoving = writePos;
    afterMoving = (writePos + len)%AUDIO_BUFLEN;
    //no puedo leer si beforeMoving < writePos < afterMoving
    return !((beforeMoving < readPos && readPos <= afterMoving) 
        || (readPos <= afterMoving && afterMoving < beforeMoving));
}

void writeBuffer(char* buf, int len){
    int topper;
    if(canIWrite(len) == 0)
        return;
    //memcpy(audioBuffer, buf, len);
    if(writePos + len < AUDIO_BUFLEN){
        memcpy(audioBuffer + writePos, buf, len);
        writePos += len;
    } else {
    topper = AUDIO_BUFLEN - writePos;
    memcpy(audioBuffer + writePos, buf, topper);
    len -= topper;
    memcpy(audioBuffer, buf, len);
    writePos = len;
    }
}

void playBuffer(int len){
    char myBuffer[BUFLEN+1];
    int topper;
    if(readPos + len < AUDIO_BUFLEN){
        memcpy(myBuffer, audioBuffer + readPos, len);
        readPos += len;
    } else {
        topper = AUDIO_BUFLEN - readPos;
        memcpy(myBuffer, audioBuffer + readPos, topper);
        len -= topper;
        memcpy(myBuffer + topper, audioBuffer, len);
        readPos = len;
    }
    IRCSound_PlaySound(myBuffer,BUFLEN);
}

boolean alreadyRecordingQuery(){
    return grabandoAudio;
}

void endAudioTransmission(){
    grabandoAudio = FALSE;
}