/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      environMentor.h                            ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.0                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef NOT_USE_ENVIRONMENTOR
#ifndef using_environMentor
#define using_environMentor (1)

#ifdef using_commandLiner
#error '.environMentor.h' must be included before '.commandLiner.h'
/*
    When including both, ".environMentor.h" and ".commandLiner.h"
    to some source, then "environMentor.h" header always needs
    to be included at first, BEFOR the ".commandLiner.h" header.
    If ".commandLiner.h" would be included before this file, the
    environMentor would not function propperly anymore.
*/
#endif

typedef unsigned char  byte;
//typedef unsigned short word;

#ifndef MIN_ARR_SIZ
#if defined(__TINYC__)
#define MIN_ARR_SIZ (0)
#else
#define MIN_ARR_SIZ (1)
#endif
#endif

typedef enum REFLECTOR {
    DISABLED =  -1,
    GLOBAL   =  0,
    LOCAL    =  0x01,
    ANY      =  0x02,
    VARIABLE =  DISABLED,
    MACRO    =  GLOBAL,
    FEATURE  =  LOCAL,
    SYMBOL   =  0x04,
    SOLIDE   =  0x08,
    FUNCTIONAL= 0x10,
} REFLECTOR;

typedef enum FeatureType {
    Type_NOTHIN = 0,
    Type_STRING = 1,
    Type_NUMBER = 2,
    Type_PREPRO = 4,
    Type_LISTER = 8,
    Type_ESCAPE = 0x0010,
	Type_PREFIX = 0x0020,
    Type_ENVVAR = 0x1000,
    Type_MACROG = 0x2000,
    Type_MACROL = 0x4000,
} FeatureType;

typedef struct FeatureGet {
    union {
    unsigned int flags;
    unsigned short size[2];
    } type;
    union {
        long double  number;
        const char*  string;
        const char** lister;
    } value;
	const char* name;
} FeatureGet;

#include "./eszentielle/.CommandLinerEndian.h"
#include "./eszentielle/.CommandLinerSetting.h"
#include "./eszentielle/.CommandLinerMacros.h"
#include "./persistente/Macros.h"
#include "./persistente/CommandoFeatures.h"

typedef enum FeatureList {
	SET_AS_LIST = FOURCC('s','e','t','\0'),
	ADD_TO_LIST = FOURCC('a','d','d','\0'),
	REMOVE_FROM = FOURCC('r','e','m','\0')
} FeatureList;

typedef struct MentorState {
   short   isInitialized;
   short   removalsCount;
   int     featuresAdd;
   char    globalsFile[MAX_NAM_LEN];
   char    featureFile[MAX_NAM_LEN];
 REFLECTOR scope;
   char**  reflector[2];
   char**  additionals;
} MentorState;

#define FeaturesFile(commandname) "persistente/" ## commandname ## "_features.h"

#if defined(__TINYC__)
 #define  COMMAND_DEPENDENT_HEADER  FeaturesFile(_OnTheFly_)
#else
 #define  COMMAND_DEPENDENT_HEADER  <persistente/_features.h>
#endif
#ifndef   NO_COMMAND_DEPENDENCIES
 #include COMMAND_DEPENDENT_HEADER
#endif

#ifndef DEBUG
#define DEBUG (0)
#endif
#ifdef  PREFER_LOCAL
#if    !PREFER_LOCAL
#undef  PREFER_LOCAL
#define PREFER_LOCAL (0)
#endif
#endif
#ifndef PREFER_GLOBAL
 #ifdef PREFER_LOCAL
#define PREFER_GLOBAL (!PREFER_LOCAL)
 #else
#define PREFER_GLOBAL (1)
 #endif
#endif
#ifndef PREFER_LOCAL
#define PREFER_LOCAL (!PREFER_GLOBAL)
#endif

void        InitializeEnvironMentor(void);
int         hasFeatures(void);
char        environMentor_CheckAndEnable(void);
void        showPersistente(REFLECTOR);
void        enableFeaturesForCommand(const char*);
void        setLocalMacro(char* featureString);
unsigned    isDefined(const char*);
unsigned    isReflected(REFLECTOR,const char*);
const char* getReflected(REFLECTOR,const char*);

// checks if given "name" can be found somehow..
// first checks environmet. if name was not found,
// it then goes through global reflectors, then 
// through a commands local reflectors, to see if
// macro named "name" is defined for the currently
// executing code...
int         hasVariable(const char*);
const char* getVariable(const char*);

// first checks by calling hasVariable("name") if "name"
// is defined. if is, retrieves its value from either
// environment, or from one of the macro reflectors.
FeatureGet  getFeatured(const char*);
// stores a value contained in a (previously via getFeatured() fetched) FeatureGet structure back to it's source
// where it's originating from (means when getFeatured() found a matching entry in the Environmen, and has returnd
// that as a 'FeatureGet' then the in that FeatureGet contained value after changing it gets passed to setFeatured(),
// it then will update that environment variable where getFeature() before has fetched the value from. If it instead
// was fetched from a macro reflector, then that reflector enry where it was fetched from will be updated therefore)
int setFeatured( FeatureGet* store );

// set environment variable
void        setVariable(const char*, const char*);
// stores a c-code macro to either the global reflector or a local reflector
void        setMacroRaw(const char*, const char*);
// stores a numeric expression macro to either the global reflector or a local reflector
void        setMacroNum(const char*, int);
// stores a string expression macro to either the global reflector or a local reflector
void        setMacroStr(const char*, const char*);
// removes a macro from either the global reflector or from a local reflector
void        removeMacro(const char*);
// export a whole reflector as environment variables
void        exportMacros(REFLECTOR);
// export all currently included reflectors (the global reflector + any maybe included local reflectors) as environment variables
void        exportAll(void);
// sellects a reflector and sets up for apply changes to it
char        beginPersistChange(REFLECTOR);
// set an entry in the currently selected reflector
void        setPersistEntry(const char*,const char*);
// remove an entry from the currently selected reflactor
void        removePersistEntry(const char*);
// writes the actually selected reflector entries back into the reflector's related feature.h header (to become 'real' macro #defines at least)
void        commitPersistChange(void);
// check if there's an actually change operation going on, where changes might have been made which are not written back to their header files yet.  
int         nonCommittedChanges(void);

const char* getPrefixed( REFLECTOR, const char* );
int         setPrefixed( const char*, const char* );
int         listedFeature( unsigned operation, const char* name, char* value );

#define WITH_JUNKYARD
#define WITH_STRGPOOL
#include ".commandLiner.h"

#ifdef _OnTheFly_
#include ".environMentor.cc"
#endif
#endif
#endif