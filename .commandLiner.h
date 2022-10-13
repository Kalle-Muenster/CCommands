#ifndef using_commandLiner
#define using_commandLiner (1)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(__cplusplus) && !defined(_OnTheFly_)
 COMMANDLINER_EXTERN_C
#endif

#include ".system4cc.h"

#include "./eszentielle/.CommandlinerSetting.h"

#ifdef _DEBUG
#ifndef COMMANDLINER_DISABLE_DEBUG
#undef  DEBUG
#define DEBUG (1)
#endif
#endif
#ifndef DEBUG
#define DEBUG (0)
#endif

#if !WINDOWS
    #define CONSOLA "$SHELL && "
    #define CONSOLASYNCH "exec -c %s"
#else // if WINDOWS
    #define CONSOLA "%COMSPEC% /k"
    #define CONSOLASYNCH "start \"\" %s"
    #define DOUBLEDASHED_OUT "conout$"
    #define DOUBLEDASHED_INP "conin$"
#endif

#ifndef  EMPTY
 #define EMPTY_(t) ((t)-1)
 #define EMPTY (0xFFFFFFFF)
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#ifndef MIN_ARR_SIZ
#if defined(__TINYC__)
#define MIN_ARR_SIZ (0)
#else
#define MIN_ARR_SIZ (1)
#endif
#endif

#include "./eszentielle/.CommandlinerTypes.h"
#include "./eszentielle/.CommandlinerMacros.h"

#ifndef COMMANDLINER_ESCENTIALS_DEFINED
#define ptval __SIZE_TYPE__
#endif

#ifdef ptdif
#undef ptdif
#undef ptval
#endif


#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_COMMANDLINER )
#define COMMANDLINER_API __declspec(dllexport)
#else
#define COMMANDLINER_API
#endif

typedef const char* cmLn;
typedef short       cmOp;
typedef int         cmIx;
typedef int         cmBl;

typedef struct COMMANDLINER_API ArgVars {
    char*          cmln;
    unsigned short size;
    unsigned short argc;
    char*          argv[MAX_NUM_GUM];
} ArgVars;

typedef struct COMMANDLINER_API Ding Ding;
typedef struct COMMANDLINER_API Ding {
    ulong dasda;
    Ding* dings;
    void* point;
    void(*bumms)(void);
} Ding;

#define NoString getNoString()

typedef struct COMMANDLINER_API CommanderState CommanderState;
typedef struct COMMANDLINER_API CommanderState {
    char  buffer[COMMANDER_BUFFER];
    char  CommanderError[MAX_NAM_LEN];
    char  options[MAX_NUM_GUM];
    char  types[MAX_NUM_GUM];
    char* names[MAX_NUM_GUM];
    Ding* dingens;
    byte  numgum;
    byte  numopt;
    short endOfThePath;
    char* stateOfTheCommander;
    char* namestateOfCommander;
    char* executor;
    char* reserved;
    CommanderState* running;
} CommanderState;

#define ComStateStructSize ( \
     COMMANDER_BUFFER \
   + (MAX_NUM_GUM*2) \
   + MAX_NAM_LEN \
   + (sizeof(char*)*MAX_NUM_GUM) \
   + (sizeof(char*)*4) \
   + sizeof(CommanderState*) \
   + sizeof(Junk*)*MAX_NUM_GUM \
   + 4 \
)


#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <commandLiner.h>
#else

COMMANDLINER_API CommanderState* newComState(void);

COMMANDLINER_API int     InitCommandLiner(int argCount,char** argVar);
COMMANDLINER_API void    QuickCommandInit(void);
COMMANDLINER_API int     BindCommandLine(CommanderState*,int argCount,char** argVar);
COMMANDLINER_API ArgVars RebuildCommandLine(void);
COMMANDLINER_API int     PushCommandLine(char*);
COMMANDLINER_API int     PopCommandLine(bool);

// handling 'tagged' parameters (access via tag letters)
COMMANDLINER_API cmBl    isDefault(cmOp option);
COMMANDLINER_API cmBl    hasOption(cmOp option);
COMMANDLINER_API cmLn    setOption(cmOp option, cmLn name);
COMMANDLINER_API cmLn    switchIdx(cmOp option, cmIx index);
COMMANDLINER_API void    setSwitch(cmOp option);
COMMANDLINER_API void    setModus(cmLn modus);
COMMANDLINER_API void    makeRawName(cmOp option);
COMMANDLINER_API void    noOption(cmOp option);
COMMANDLINER_API cmOp    isSwitch(cmOp option);
COMMANDLINER_API cmOp    isTagged(cmLn name);

// handling 'modus' parameters (access via modus names)
COMMANDLINER_API cmOp    isModus( cmLn name );
COMMANDLINER_API cmOp    isAnyModus( cmLn wildcard );
COMMANDLINER_API cmBl    isAnyOtherModusThen( cmLn thisModus );
COMMANDLINER_API cmBl    isAnyModusAtAll( void );
COMMANDLINER_API cmLn    getModus( cmOp ofOption );
COMMANDLINER_API cmLn    getModusByNumber( uint number );
COMMANDLINER_API int     getNumberOfModus( cmLn mode );
COMMANDLINER_API int     idxOfModusNumber( int number );
COMMANDLINER_API cmBl    hasModus( cmLn modus, cmLn thisName );

// handling 'index' parameters (for access via index numbers)
COMMANDLINER_API cmIx    indexOf( cmOp option );
COMMANDLINER_API cmIx    indexOfName( cmLn name );
COMMANDLINER_API cmIx    indexOfFirst( cmLn part );
COMMANDLINER_API cmLn    getNameByIndex( cmIx index );
COMMANDLINER_API cmOp    byIndexTheOption( cmIx index );
COMMANDLINER_API cmLn    getName( cmOp option );

// raw parameters (wich are neither modus nor tagged)
COMMANDLINER_API cmLn    rawNext( cmOp option );
COMMANDLINER_API cmLn    nextRaw( void );
COMMANDLINER_API cmLn    rawName( int number );

COMMANDLINER_API cmIx    rawNum(void); // count on undashed, untagged parameters
COMMANDLINER_API cmIx    optNum(void); // count on option letter tagged parameters   
COMMANDLINER_API cmIx    numGum(void); // count on tokken at all: optNum() + rawNum()

// parameter validity and error handling
COMMANDLINER_API cmBl    isValidArg(cmLn);
COMMANDLINER_API cmBl    isEmptyArg(cmLn);
COMMANDLINER_API cmBl    wasError(void);
COMMANDLINER_API cmLn    getError(void);
COMMANDLINER_API int     getErrorCode(void);
COMMANDLINER_API int     setError(cmLn,int);
COMMANDLINER_API cmLn    setErrorText(cmLn);
COMMANDLINER_API cmLn    setErrorCode(int);
COMMANDLINER_API cmBl    catchError(const char*);
COMMANDLINER_API cmBl    catchErrorCode(int);
COMMANDLINER_API void    clearAllErrors(void);
COMMANDLINER_API cmBl    CheckForError(void);
COMMANDLINER_API void    ExitOnError(const char*);

// 'path' to the file which contains main() (or path to executable if not c-script but compiled code instead)
COMMANDLINER_API cmLn    getCommander(void);
// 'path' to the folder which contains that file which contains main()
COMMANDLINER_API cmLn    getPathOfTheCommander(void);
// 'name' of that file which contains main() (without full path)
COMMANDLINER_API cmLn    getNameOfTheCommander(void);
// decide if 'Commander' path/name functions should return filename
// with or without file extention (in most cases extension is .c)
COMMANDLINER_API cmLn    commanderUseExtension(bool);

COMMANDLINER_API cmLn    getNoString(void);
COMMANDLINER_API char*   setTemp(const char*);
COMMANDLINER_API char*   setTempf(const char*,const char*);
COMMANDLINER_API char*   getTemp(void);
COMMANDLINER_API int     tempf(const char*,const char*);
COMMANDLINER_API void    setBaseCommander(void);
COMMANDLINER_API cmBl    isBaseCommander(void);
COMMANDLINER_API void    showOptions(void);
COMMANDLINER_API cmLn    parameterCopy( char* dst, cmLn src );
// stringCompare() return false on identical strings or
// position of first missmatch + 1 on different strings.
COMMANDLINER_API uint    stringCompare(cmLn,cmLn);
COMMANDLINER_API cmLn    unQuoted(cmLn);
COMMANDLINER_API cmLn    toQuoted(cmLn);
COMMANDLINER_API cmBl    isQuoted(cmLn);

// get a named 'dingens' from the commander which has 
// previously been attatached via addDingens() function 
COMMANDLINER_API void*   getDingens( const char* named );
// add a 'dingens' to the commander which later can be 
// retrieved at any time again via calling getDingens()
// a 'dingens' can be anything thats pointed by a void*
// when process ends, commander disposes all dingenses
// properly if still some dingenses are attached maybe.
COMMANDLINER_API void    addDingens( const char* named, void* dings, void(*bumms)(void) );
// remove a named dingens from the commander which previously
// was attached to the commander by calling addDingens() 
COMMANDLINER_API void    remDingens( const char* named );

// convert the given, either ':', ';', ',', '\n' or ' '
// separated list string to a '\n' separated list string
COMMANDLINER_API char*   toPrintList(char* list);

// split a string at contained 'fromSep' chars
// to a list of 'toSep' separated substrings by
// returning count on so created split part elements
COMMANDLINER_API uint    toSplitList(char* sepList, char* fromTo);

COMMANDLINER_API void    DestructCommander(void);
COMMANDLINER_API uint    isCleansening(void);

#endif

#ifdef _OnTheFly_
 #ifdef WITH_STRGPOOL
  #include ".stringPool.h"
 #endif
 #ifdef WITH_JUNKYARD
  #include ".junkYard.h"
 #endif
 #include ".commandLiner.cc"
 #ifdef WITH_STRGPOOL
  #include ".stringPool.cc"
  #undef  Pool
  #define Pool DEFAULT_POOLBOTTOM
 #endif
 #ifdef WITH_JUNKYARD
  #include ".junkYard.cc"
 #endif
#elif defined(__cplusplus)
 COMMANDCLOSER_EXTERN_C
#endif
#endif
