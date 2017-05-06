#include "../includes/G-2301-05-P2-audio.h"

void die(char *s)
{
    perror(s);
    exit(1);
}

long int initiateSender(){
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    //char buf[BUFLEN];
    char message[BUFLEN+1];
 
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
 
    IRCSound_RecordFormat(PA_SAMPLE_S16BE,2);
    IRCSound_OpenRecord();
    while(1)
    {
        IRCSound_RecordSound(message,BUFLEN);
        if (sendto(s, message, BUFLEN , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }
    }
 	IRCSound_CloseRecord();
    close(s);
    return 0;
}

long int initiateReciever(){
    struct sockaddr_in si_me, si_other;
    int s;
    char buf[BUFLEN+1];
    socklen_t slen = sizeof(si_other);
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    IRCSound_PlayFormat(PA_SAMPLE_S16BE,2);
    IRCSound_OpenPlay();
    while(1)
    {
    	if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }
    	IRCSound_PlaySound(buf,BUFLEN);
    }
    IRCSound_ClosePlay();
    close(s);
    return 0;
}