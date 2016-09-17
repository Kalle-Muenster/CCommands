#ifndef _OnTheFly_
#include "commandLiner.h"
#endif

#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH (128) 
#define TERMINATOR (127)
#define COMMANDER_BUFFER 2048

char _argumzBuffer[COMMANDER_BUFFER] = {'\0'};
byte _numberOfArgum = 0;
char _options[8] = {'\0'};
char* _names[8];
char* _executor=NULL;
short _endOfThePath = 0;
char* _stateOfTheCommander;


char* stringCopy(char* dst,char* src)
{
	dst[TERMINATOR]=-1;
	while('\0'!=(dst[++dst[TERMINATOR]]=src[dst[TERMINATOR]]));
	return &dst[0];
}
	 
int pargumnz(int count, char** argv)
{
	_executor=argv[0];
	
	_endOfThePath = -1;
	while( _executor[++_endOfThePath] != '\0' );
	while( _executor[--_endOfThePath] != '\\' );
	_stateOfTheCommander = &_executor[_endOfThePath];
	
	
	for(int i = 0;i<COMMANDER_BUFFER;i++)
		_argumzBuffer[i]='x';
	_argumzBuffer[COMMANDER_BUFFER-1]='\0';
	_numberOfArgum=0;
	
	for(int i=0;i<8;i++)
	{
		int str = (i*MAX_NAME_LENGTH);
		_names[i]=&_argumzBuffer[str];
		_names[i][str+TERMINATOR] = '\0';
	}
	
	for(int i=1;i<count;i++)
	{
		char* current = argv[i];
		if(argv[i][0]=='-')
		{
			if(argv[i][1]=='-')
			{
				_options[_numberOfArgum]=current[2];
				if(argv[i][3]=='-')
					stringCopy(_names[_numberOfArgum],(current+4));
				else
					stringCopy(_names[_numberOfArgum],(current+2));
			}else{
				_options[_numberOfArgum]=argv[i][1]; 
			_names[_numberOfArgum]=""; }
		}else
			stringCopy(_names[_numberOfArgum],current);	
		
		++_numberOfArgum;
	}
	return _numberOfArgum;
}

int numGum(void)
{
	return _numberOfArgum;
}

short indexOf(short option)
{
	char i = _numberOfArgum;
	while((i--)>0)
		if(option == _options[i])
			return i;
	return i;
}
short indexOfName(char* name)
{
	for(int i=0;i<_numberOfArgum;i++)
		if(!strcmp(name,&_names[i][0]))
			return i;
	return -1;
}
char nameWasSwitched(char* name)
{
	return _options[indexOfName(name)];
}
char* getNameByIndex(int index)
{
	return _names[index];
}
char* getName(short option)
{
	return getNameByIndex(indexOf(option));
}

short hasOption(short option)
	 { return (indexOf(option)+1); }

short isModus(char* name)
	 { return name[0]&&indexOfName(name)+1; }

void setOption(short option,char* name)
{
	short index = indexOf(option);
	index = (index<0) ? _numberOfArgum : index;
	_options[index]=option;
	stringCopy(&_names[index][0],&name[0]);
	_numberOfArgum++;
}

void setSwitch(short option)
{
	setOption(option,"");
}


void showOptions(void)
{
	printf("\n\nExecuting commando: %s\nrecognized input tokken:\n\n",_executor);
	for(unsigned i=0;i<_numberOfArgum;i++)
	{
		if(*_names[i]!='\0')
			printf("%i. \"-%c\" associated names: %s\n",i,_options[i],_names[i]);
		else
			printf("%i. \"-%c\"\n",i,_options[i]);
	}
	printf("\n\n");
}


char _ERROR[68] = {0,0,0,0,'O','K','\0'};

char* getError(void)
{
	return &_ERROR[0];
}
int getErrorCode(void)
{
	return (*(int*)&_ERROR[0]);
}
void setError(char* error)
{
	stringCopy(&_ERROR[4],error);
	(*(int*)&_ERROR[0])++;
}
	
char wasError(void)
{
	return !(_ERROR[4]=='O' && _ERROR[5]=='K'); 
}

char CheckForError(void)
{
	if(wasError())
		printf("ERROR: %s",&_ERROR[4]);
	return wasError();
}
	
	
	
char* getCommander(void)
{
	*_stateOfTheCommander='\\';
	return _executor; 
}
	
char* getPathOfTheCommander(void)
{
	*_stateOfTheCommander = '\0';
	return _executor;
}

char* getNameOfTheCommander(void)
{
	return 	_stateOfTheCommander+1;
}