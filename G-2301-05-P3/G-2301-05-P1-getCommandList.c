#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <redes2/irc.h>
#include "../includes/G-2301-05-P1-socket.h"

int main(int argc, char* argv[]){
	int i;
	//FILE* fp;

	if(argc < 1)
		return 1;
	for(i = 1; i < argc; i++)
		printf("%ld\t%s\n",IRC_CommandQuery(argv[i]),argv[i]);
	return 0;
}
