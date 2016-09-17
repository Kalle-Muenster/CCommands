/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      makeExecutable.cc                          ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.0                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_ 
#include "makeExecutable.h"
#endif 

// regular includes here: 
#include <tcclib.h>
#include <process.h>
#include "commandLiner.h"

//... ...

int main(int argc,char**argv)
{
    pargumnz(argc,argv);
		
	int p=7;
	char buffer[256]={"tcc.exe   "};
	for(int i=0;i<numGum();i++)	{
		char* a = getName(i);
		while((buffer[++p]=*a++)!='\0');
		buffer[p]=' ';
	} buffer[p]='\0';
	
	printf("\nbuilding: %s\n",&buffer[7]);
	
	system(&buffer[0]);
	p=-1;
	strcpy(&buffer[0],getName(0));
	while(buffer[++p]!='\0');
	while(buffer[--p]!='.');
	buffer[p]='\0';
	sprintf(&buffer[128],"%s\\%s.exe",getcwd(),&buffer[0]);
	char success = !_access(&buffer[128],F_OK);
		if(success)
			printf("\n...success!\ncreated file: %s\n\n",&buffer[128]);
	return success;
}//eof
