/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      persist.h                                  ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_persist
#define IncludesFor_persist (1)

#include ".environMentor.h"

int persist_commandCall(const char* cmdline);

#ifdef _OnTheFly_
#include ".persist.cc"
#endif

#endif
