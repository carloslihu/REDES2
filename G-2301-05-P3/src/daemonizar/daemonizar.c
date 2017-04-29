#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>

void daemonizar(){
	int fatherPid = getpid();
	int pid;
	int sid;
	//FILE* fp;
	// Create child process
	//pid = fork();
	// Indication of fork() failure
	//if(pid == -1)
		//exit(1);
	// PARENT PROCESS. Need to kill it.
	//else if(pid != 0)
		//exit(0);
	//child process	
	//else {
		//unmask the file mode
		//umask(0);
		//set new session
/*
		sid = setsid();
		if(sid < 0)
		{
			// Return failure
			printf("Puta vida tt\n");
			exit(1);
		}
*/
		// Change the current working directory to root.
		//chdir("/");
		// Close stdin. stdout and stderr
		/*close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		*/	

		//openlog("servidor: ", LOG_CONS , LOG_USER);
		syslog(LOG_INFO | LOG_USER, "Hola Mundo");	
		//closelog();
		return;
	//}
}
