/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      CommandLinerConfig.cc                       ||
||     Author:    unknown                                    ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_ 
#include "CommandLinerConfig.h"
#endif 

// regular includes here: 
#include <tcclib.h>
#include "environMentor.h"


void ShowConfiguration(void)
{
	printf("\nCurrent Command-Liner Configuration:\n\n");
	printf("Maximum number of arguments: %i\n",MAX_NUM_GUM);
    printf("Maximum length of each argument: %i\n",MAX_NAM_LEN);
	printf("Total amount on commander buffer: %i\n\n",COMMANDER_BUFFER);
}

void SetConfiguration(void)
{
	printf("\nSet maximum number on arguments: ");
	int maxnumgum = 16;
	scanf("%i",&maxnumgum);
	printf("\nSet maximum length of arguments: ");
	int maxnamlen=128;
	scanf("%i",&maxnamlen);
	char buffer[128];
	sprintf( &buffer[0],
			 "#define MAX_NUM_GUM (%i)\n#define MAX_NAM_LEN (%i)\n",
			 maxnumgum, maxnamlen );
	char filename[256];
	sprintf( &filename[0],"%s/persistente/CommandLinerSettings.h",
	         getPathOfTheCommander() );
	FILE* f = fopen(&filename[0],"w+");
	fwrite(&buffer[0],1,strlen(&buffer[0]),f);
	fflush(f);
	fclose(f);
}

int main(int argc,char**argv)
{
    CommandLineArgs(argc,argv);

	if(hasOption('v'))
		showOptions();
	
	if(!numGum())
	    ShowConfiguration();
	
	if(hasOption('s'))
		SetConfiguration();

	
	return 0;
}//eof
