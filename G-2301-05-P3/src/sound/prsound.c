#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <redes2/ircsound.h>

int main()
{
	char buf[256];
	register int i;
	FILE *f;

	IRCSound_RecordFormat(PA_SAMPLE_S16BE,2); 
	IRCSound_PlayFormat(PA_SAMPLE_S16BE,2); 

	if(IRCSound_OpenRecord()) puts("error");
	f=fopen("pruebasonido","w+b");
	puts("Grabando");
	for(i=0; i < 10000; ++i)
	{
		IRCSound_RecordSound(buf,160);
		fwrite(buf,1,160,f);
	}
	fclose(f);
	IRCSound_CloseRecord();

	if(IRCSound_OpenPlay()) puts("error");
	f=fopen("pruebasonido","rb");
	puts("Reproduciendo");
	for(i=0; i < 10000; ++i)
	{
		fread(buf,1,160,f);
		IRCSound_PlaySound(buf,160);
	}
	fclose(f);
	IRCSound_ClosePlay();
	return 0;
}

