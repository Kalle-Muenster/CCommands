/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      CommandlinerTests.h                        ||
||     Author:    Kalle                                      ||
||     Generated: 18.08.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_CommandlinerTests
#define IncludesFor_CommandlinerTests (1)

#include <.environMentor.h>
#include <.stringPool.h>
#include <.base64.h>
#include <.crypt64.h>

#if defined(__TINYC__)
#if defined(__LONG_UNSIGNED__)
#define __LONG_LONG_TYPE__ "UNSIGNED"
#else
#define __LONG_LONG_TYPE__ "SIGNED"
#endif
#define PrintCompiler() printf("Compiler used: TinyC %i\n", __TINYC__ ); \
printf("long long int is '%s' type!\n", __LONG_LONG_TYPE__ )
#elif defined(_MSC_BUILD)
#define PrintCompiler() printf("Compiler used: MSVC %i\n", _MSC_VER )
#endif

#define PrintValues(typ,fmt) \
 pool_set(" exponent-bits: %i\n mantisse-bits: %i\n minimum: %+");\
 pool_set(fmt);\
 pool_set("\n maximum: %+");\
 pool_set(fmt);\
 pool_set("\n\n");\
printf( pool_merge(5), typ ## _DIG, typ ## _MANT_DIG, - ## typ ## _MAX, typ ## _MAX )

void tests_printStructSizes(void);
void tests_architectureTest(int which);
void tests_base64Conversion(int B64);
int  tests_crypsingKeyTests(ulong beg,ulong end,int boolean);

#ifdef _OnTheFly_
#include ".CommandlinerTests.cc"
#else
#include <.commandLiner.h>	
#endif
#endif
