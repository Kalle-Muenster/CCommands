#ifndef COMMAND_LINER 
#define COMMAND_LINER commandLiner.cc
#include <stdbool.h>

typedef char 			BOOL;
typedef unsigned char 	byte;

char* 	stringCopy(char* dst,char* src);
int 	pargumnz(int count, char** argv);
int     numGum(void);
short	hasOption(short option);
void	setOption(short option, char* name);
void	setSwitch(short option);
short	isModus(char* name);
short 	indexOf(short option);
short 	indexOfName(char* name);
char* 	getNameByIndex(int index);
char* 	getName(short option);
void 	showOptions(void);
char* 	getCommander(void);
char*   getPathOfTheCommander(void);
char* 	getNameOfTheCommander(void);
char*	getError(void);
void	setError(char* error);
char	wasError(void);
char 	CheckForError(void);
int 	getErrorCode(void);


#ifdef _OnTheFly_
#include "commandLiner.cc"
#endif
#endif