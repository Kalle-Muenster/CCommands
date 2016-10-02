/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      environMentor.cc                           ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.0                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_ 
#include "environMentor.h"
#endif 

// regular includes here: 
#include "commandLiner.h"

//Todo: ...
char setMacroNum(const char* name,int value)
{
	char buffer[16];
	printf("%i",value);
	return setMacro(name,(const)&buffer[0]);
}

char setMacro(const char* name,const char* value)
{
	char ok = false;
	FILE* f = fopen("persistente/Macros.h","r+");
	char macro[256];
	unsigned position = 0;
	
	while(fscanf(f,"%s\n",&macro[0]))
	{
		char match = true;
		int i=0;
		while(name[i]!='\0'&&macro[i+8]!=' ')
		{
			if(!(match = (name[i]==macro[i+8])))
				break;
			i++;
		}			
		
		if(match)
		{
			fseek(f,position,SEEK_SET);
			i = sprintf(&macro[0],"#define %s (%s)\n",name,value);
			fwrite(&macro[0],1,i,f);
			ok=true;
			break;
		}
		else
			position=ftell(f);
	}
	
	fflush(f);
	fclose(f);
	return ok;
}



void changeEnvironment(int id)
{
	//Todo: ...
}

char hasVariable(const char* name)
{
	return strlen(getenv(name))?1:0;
}

char* getVariable(const char* name)
{
	return getenv(name);
}

void setVariable(const char* name,const char* value)
{
	char buffer[128];
	sprintf(&buffer[0],"%s=%s",name,value);
	putenv(&buffer[0]);
}
