/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      cLine.h                                    ||
||     Author:    Kalle Muenster                             ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_cLine
#define IncludesFor_cLine (1)

#ifndef __TINYC__
#define MIN_ARR_SIZ 1
#endif

#define SPACEN_DASHING (0)

#define WITH_STRGPOOL
#define WITH_JUNKYARD
#include ".environMentor.h"


#ifndef CLINE_OUTER_BUFFER
#define CLINE_OUTER_BUFFER (1024)
#endif

#ifndef CLINE_INNER_BUFFER
#define CLINE_INNER_BUFFER (1024)
#endif

#ifndef CLINE_ECHO
#define CLINE_ECHO (0)
#endif

#ifndef CLINE_ASYNCHRON
#define CLINE_ASYNCHRON (0)
#endif

#define FILTER_RAWNAMES (1)

#define CLineEcho(chunk) cLine_echoCompiledCall(chunk)
#define CLineExec(chunk) cLine_execCompiledCall(chunk)

typedef enum INCLUDESTYLE {
    LIB=FOURCC('<','%','s','>'),
    CWD=FOURCC('\"','%','s','\"')
} IncludeStyle;

typedef const char*(*CLineCallable)(void);
typedef const void*(*CLineFunction)(void*);

typedef struct ExecutableCmd {
    CLineCallable exec;
    char*         done;
    ulong         size;
} ExecutableCmd;

const char*   cLine_echoCompiledCall( ExecutableCmd* chunk );
const char*   cLine_execCompiledCall( ExecutableCmd* chunk );

ExecutableCmd cLine_compileExpresion( void* state,
                                      const char* /*IN*/ cCode,
                                      const char* /*IN*/ cIncl,
                                      const char* /*IN*/ fragmentName,
                                      uint* /*OUT*/ fragmentId );

#ifdef _OnTheFly_
#include ".cLine.cc"
#endif
#endif
