#ifndef _OnTheFly_
#include "arguments.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include "commandLiner.h"

void showUsage(void)
{
	printf("\ncommand:\nArguments\n\n\
usage:\nArguments [<contain/get> <argument>] < whitespace separated arglist / *% >\n\n\
When called without either \"contain\" or \"get\" as first parameter, it will count\n\
the numnber of arguments passed and return it as ERRORLEVEL.\n\n");
	printf("<contain> <argument> :\n\
		will return the index of first occurance of the second parrameter\n\
		given <argument> in the following argument list as ERRORLEVEL. if <argument> can't\n\
		be found, 0 will be returned.\n\n\n");
	printf("<get> <-identifier> :\n\
		will find the next argument following <-identifier> and then set's it to a environment\n\
		variable called \"identifier\" (!without leading \"-\"sign!) and return it's index to ERRORLEVEL. \n\n"); 
}

int main(int argc, char** argv)
{
	CommandLineArgs(argc,argv);

//	showOptions();
	
	if(isModus("help"))
	{
		showUsage();
		return 0;
	}
	
	int result = 0;
	if(argc<3)
	{	
		printf("%i",argc-1);
		fprintf(stdin,"%i",argc-1);
		return argc-1;
	}

	if(strcmp(argv[1],"contain")==0)
	{
		for(int i=3;i<argc;i++)
		{
			if(strcmp(argv[2],argv[i]) == 0)
			{
				result=i-2;
				printf("%i",result);
				fprintf(stdin,"%i",result);
				return result;
			}
		}
	}
	else if(strcmp(argv[1],"get")==0)
	{
		char setString[64] = {'\0'};
		int beginn=-1;
		for(int i=3;i<argc-1;i++)
		{
			if(strcmp(argv[2],argv[i]) == 0)
			{
				result=i-1;
				while(argv[2][++beginn]=='-');
				sprintf(&setString[0],"%s=%s",&argv[2][beginn],argv[i+1]);
			//	system("exit");
				putenv(&setString[0]);
			//	system("start /B cmd.exe /k");
				fprintf(stdin,"set %s\n",&setString[0]);
				return result;
			}
		}
	}
	else
	{	
		printf("%i",argc-1);
		fprintf(stdin,"%i",argc-1);
		return argc-1;
	}
	
	return result;
}

