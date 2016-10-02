/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      environMentor.h                            ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.0                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef using_environMentor
#define using_environMentor environMentor.cc

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#include <fenv.h>

void  changeEnvironment(int);
char  hasVariable(const char*);
char* getVariable(const char*);
void  setVariable(const char*,const char*);
char  setMacroNum(const char*,int);
char  setMacro(const char*,const char*);

#ifdef _OnTheFly_
#include "environMentor.cc"
#endif
#endif
