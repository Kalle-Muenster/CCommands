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
#include <tcclib.h>
#include <direct.h>
#include <fenv.h>

//Todo: ...


void changeEnvironment(int id)
{
	//Todo: ...
}

char hasVariable(char* name)
{
	return strlen(fegetenv(name))?1:0;
}

void setVariable(char* name,char* value)
{
	fesetenv(name,value);
}