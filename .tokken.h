/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      tokken.h                                   ||
||     Author:    Kalle                                      ||
||     Generated: 26.05.2020                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_tokken
#define IncludesFor_tokken (1)

#include ".CheckForProjectSettings.h"

#define WITH_STRGPOOL
#define WITH_JUNKYARD

#if defined(__TINYC__)
#include <.environMentor.h>
#else
#include <.commandLiner.h>
#endif

#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_TOKKEN_API )
#define TOKKEN_API __declspec(dllexport)
#endif

#if !defined(_OnTheFly_)
#if defined(__cplusplus)
extern "C"{
#endif
#endif

#ifndef TOKKEN_API
#define TOKKEN_API
#endif

    typedef enum TOKKEN_API tokken_CharSet {
        tokken_HEX = 16, tokken_B64 = 64
    } tokken_CharSet;

    typedef struct TOKKEN_API tokken_Define {
        const byte*   grouping;
        tokken_CharSet charset;
        int               size;
    } tokken_Define;

#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <tokken.h>
#else

    TOKKEN_API tokken_Define tokken_define( const char* grouping, tokken_CharSet mode );
    TOKKEN_API const char*   tokken_create( const tokken_Define* mode );

#endif

#ifdef _OnTheFly_
#include ".tokken.cc"
#elif defined(__cplusplus)
}
#endif
#endif
