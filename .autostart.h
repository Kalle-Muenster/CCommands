/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      autostart.h                                ||
||     Author:    autogenerated                              ||
||     Generated: by Command Generator v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_autostart
#define IncludesFor_autostart (1)

#define ARGUM_SLASHING (0)

#define WITH_STRGPOOL
#define WITH_JUNKYARD
#include ".environMentor.h"

void autostart_add(const char* name, const char* command);
void autostart_nxt(const char* command);
cmLn autostart_lst(void);
void autostart_rem(const char* name);

#ifdef _OnTheFly_
#include ".autostart.cc"
#endif
#endif
