/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      dynDnUpter.h                               ||
||     Author:    Kalle                                      ||
||     Generated: 30.09.2017                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_dynDnUpter
#define IncludesFor_dynDnUpter (1)

#include <.environMentor.h>

#ifndef DYN_UPDATE_SOCKETIER 
#define DYN_UPDATE_SOCKETIER "undefined"
#endif
#ifndef DYN_SERVICE_ADRESS
#define DYN_SERVICE_ADRESS "undefined"
#endif
#ifndef DYN_SERVICE_SESSION 
#define DYN_SERVICE_SESSION "undefined"
#endif

const char* dynDnUpter_PrepareSlot(const char* updateSlot);
int         dynDnUpter_DoUpdate(const char* updateSlot);
const char* dynDnUpter_CheckForSettingChanges(CommanderState* cmndstate);

#ifdef _OnTheFly_
#include ".dynDnUpter.cc"
#endif
#endif
