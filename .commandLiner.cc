#ifdef  _OnTheFly_
 #ifdef using_commandLiner
  #if   using_commandLiner > 1
#define NOT_COMPILE_FILE
  #endif
#undef  using_commandLiner
#define using_commandLiner (2)
 #endif
#else
 #include ".CheckForProjectSettings.h"
 #include ".commandLiner.h"
 #ifdef using_stringPool
 StringPool* Pool;
 #endif
#endif

#ifndef NOT_COMPILE_FILE

#include <string.h>
#include <stdio.h>

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#if MAX_NAM_LEN < 256
#define TERMI_POS (MAX_NAM_LEN-1)
#define TERMI_TYP byte*
#else
#define TERMI_POS (MAX_NAM_LEN-2)
#define TERMI_TYP short*
#endif

const static unsigned long long nstr = 0x000000000000ffff & (32 + (8 << 8));

#define Termi(cmLnString) *(TERMI_TYP)&cmLnString[TERMI_POS]
#define setReserved(t,i,v) ((t*)c.running->reserved)[i] = v
#define getReserved(t,i)   ((t*)c.running->reserved)[i]
#define ERROR_ARGUM_COUNT "increase number of maximum arguments"
#define ERROR_ARGUM_LENTH "increase maximum length for arguments"
#define ERROR_MUST_RELOAD (1330398546)
#define ERROR_NO_FEATURES (0xfffff830)
#define lastRawquested     getReserved(uint,(MAX_NAM_LEN/4)-2)

char systemFourCC[5] = {"NONE"};
const char* SystemFourCC = &systemFourCC[0];
#define SysFourCC *(unsigned*)&systemFourCC[0]

typedef const ulong cmDn;
cmDn DingensMask = 0x00000000ffffffff;
#define Dingens(namstr) (DingensMask & *(cmDn*)namstr)
#define Maskens(dadas) ( (const char*)&dadas )

CommanderState c = {
    { '\0' }, { '\0' }, { '\0' }, { 'v' },
    { NULL }, NULL, (uint)0, (uint)0, (uint)0,
    NULL, NULL, NULL, NULL, NULL
};

int  commandLineArgs( int count, char** argv );


#if !WINDOWS
void DestructCommander_ALTERNATIVE(int rStat, void *arg)
     { DEBUGFMT("return state %i",rStat) printf("\n"); DestructCommander(); }
#define atexit(func) on_exit( func ## _ALTERNATIVE, &c.running->CommanderError[0] )
#endif


cmLn getNoString()
{
    return (cmLn)&nstr;
}

int isValidArg( cmLn argument )
{
    return argument == (cmLn)&nstr
        || argument == NULL ? false
        : *((short*)argument) != (short)nstr;
}

// means the token is either: not valid OR first character is '\0'
int isEmptyArg( cmLn argument )
{
    return (!isValidArg(argument)) || (!*argument);
}


// allocates and initializes a new commandline state
CommanderState* newComState(void)
{
    CommanderState* comState = (CommanderState*)malloc( sizeof(CommanderState) );
    memcpy( comState, &c, sizeof(CommanderState) );
    comState->running = NULL;
    comState->numgum = 0;
    comState->numopt = 0;
    return comState;
}

// pops any states which have been pushed before
void setBaseCommander(void)
{
    if(c.running != &c && c.running != NULL)
        free(c.running);
    c.running = &c;
}

// will parse parameters and use the buffers
// allocated in the local declared CommanderState "c".
int InitCommandLiner(int count, char** argv)
{
    if( !count ) {
        QuickCommandInit();
        return 0;
    }
    if( c.running == NULL )
        c.running = &c;
    else
        return c.running->numgum;

    AsInt(SystemFourCC) = argv[0][0]=='/'
                        ? LNX
                        : WIN;

    if(count >= MAX_NUM_GUM) {
        setErrorText(ERROR_ARGUM_COUNT);
        AsInt(&c.running->CommanderError[0]) = -1000;
        return getErrorCode();
    }
    #if using_stringPool
        DEBUGLOG("Initializing wirlvar")
        pool_InitializeCycle();
    #endif
    atexit(&DestructCommander);
    return commandLineArgs(count,argv);
}

// will push a given CommanderState and then parses the given parameters into it's buffers.
int BindCommandLine( CommanderState* cmdLine, int count, char** argv )
{
    cmdLine->running = c.running;
    c.running = cmdLine;
    return commandLineArgs( count, argv );
}

// rebuilds a command line string from current state buffers
// so it's parameters are representing actual buffer states
// of the currently executing commandline (maybe manipulated)
// or the virtual (via PushCommandLine()) pushed commadline
ArgVars RebuildCommandLine(void)
{
    ArgVars rebuilt = { c.running->reserved, (unsigned short)0,
                        c.running->numgum+1, {c.running->reserved} };

    rebuilt.size = sprintf( &rebuilt.cmln[0],"\"%s/%s\"",
                            getPathOfTheCommander(),
                            commanderUseExtension(true) );

    rebuilt.argv[0] = rebuilt.cmln;
    for(int i=0; i < c.running->numgum; i++) {
        rebuilt.argv[i+1] = rebuilt.cmln+rebuilt.size;
        char opt = c.running->options[i];
        if( opt ) {
            if( isSwitch( opt ) ){
                AsInt(&rebuilt.cmln[rebuilt.size++]) = AsInt(" -X\0");
                rebuilt.cmln[++rebuilt.size]=opt;
                rebuilt.size++;
            } else {
                rebuilt.size += sprintf( &rebuilt.cmln[rebuilt.size],
                                            " --%c-\"%s\"", opt, getName(opt) );
            }
        } else {
            rebuilt.size += sprintf( &rebuilt.cmln[rebuilt.size]," %s",
                                     getNameByIndex(i) );
        }
    } return rebuilt;
}

// pushes a new, empty commandline state
// and parses the given string as if it
// would be executed command. it indexes
// any tokken contained within the line.
// prior instance will be inaccessible
// till new state will be 'poped' again
int PushCommandLine( char* newCommandLine )
{
    CommanderState* newState = newComState();
    const uint length = (uint)strlen(newCommandLine);
    DEBUGLOG(getCommander())
#ifdef __TINYC__
    char linebuffer[length+strlen(getCommander())+1];
#else
    char* linebuffer = (char*)malloc(length+strlen(getCommander())+1);
    //yard_addDestructor( linebuffer );
#endif
    strcpy(&linebuffer[0],getCommander());
    strcat(&linebuffer[0]," ");
    strcat(&linebuffer[0],newCommandLine);
    char* newLine = &linebuffer[0];
    DEBUGLOG(newLine)
    int i = -1;
    while( newLine[++i] != '\0' ) {
        if( newLine[i] == ' ' ) {
            newLine[i] = '\0';
            newState->numgum++;
        }
    } if (i) newState->numgum++;
    DEBUGFMT("new num gum: %i",newState->numgum)

    if( c.running->numgum >= MAX_NUM_GUM ) {
        setErrorText(ERROR_ARGUM_COUNT);
        *(int*)&c.running->CommanderError[0] = -1000;
        free(newState);
        return getErrorCode();
    }
    char* nextArg = newLine;
#ifdef __TINYC__
    char* arglist[newState->numgum];
#else
    char** arglist = (char**)malloc(newState->numgum);
    //junk_installCleansenerFor(arglist);
#endif
    for (i=0;i<newState->numgum;++i) {
        DEBUGFMT("nextArg is %s",nextArg);
        arglist[i] = nextArg;
        while(*(nextArg++));
    }
    char** argvars = &arglist[0];
    for(int i=0;i<newState->numgum;++i) {
        printf("ARGUM NUM %i is %s\n",i,argvars[i]);
    }
    int returncode = BindCommandLine( newState, newState->numgum, argvars );
#ifndef __TINYC__
    free(linebuffer);
    free(arglist);
#endif
    return returncode;
}

// frees the 'pushed' commandline state and restores
// the prior instance's option and name buffers back.
int PopCommandLine( bool collectErrors )
{
    if( !isBaseCommander() ) {
        int errcode = collectErrors
                    ? ERROR_CODE(c.running)
                    : EXIT_SUCCESS;
        char lastError[MAX_NAM_LEN];
        if( collectErrors && errcode != EXIT_SUCCESS ) {
            strcpy( &lastError[0], ERROR_NAME(c.running) );
        }
        CommanderState* top = c.running;
        c.running = top->running;
//----------------------------------------- switched back
        if( top->dingens ) {
            Ding* dingens = top->dingens;
            Ding* dings = dingens;
            do {
#ifdef using_junkYard
                if ( dings->dasda == *(uint*)"junk" ) {
                    Junk* junkyard = (Junk*)dings->bumms;
                    junkyard->drop( junkyard );
                    free( junkyard );
                } else
#endif
#ifdef using_stringPool
                if ( dings->dasda == *(uint*)"pool" )
                    dings->bumms = NULL;
#endif
                ; // ansonsten nich else
                dings = dingens->dings;
                dingens->dings = NULL;
                free( dingens );
            } while ( dingens = dings );
        } free( top );
        if( errcode != EXIT_SUCCESS ) {
            setErrorText( &lastError[0] );
            setErrorCode( errcode );
        } return errcode + 1;
    }
    return EXIT_SUCCESS;
}

cmBl isBaseCommander(void)
{
    return c.running == &c;
}

char* checkForSpaces(char* name)
{
#ifndef NO_SPACE_CHECKING
    int reserver = c.running->reserved[MAX_NAM_LEN]+1;
    int shouldEscapeSpace = 0;
    while( reserver )
        shouldEscapeSpace += c.running->reserved[--reserver];
    if( shouldEscapeSpace )
        return name;
    int i = -1;
    char containsSpaces = false;
    while( name[++i] )
        if( name[i] == ' ' ) {
            if (i == 0) name[i] = '^';
            else containsSpaces = true;
        break; }
    if( containsSpaces ) {
        sprintf( c.running->reserved, "\"%s\"", name );
        return c.running->reserved;
    } else if ( name[0] == '-' ) {
        sprintf( c.running->reserved, "^-%s", &name[1] );
        return c.running->reserved;
    }
#endif
    return name;
}

// don't parse anything, just initialize buffers..
void QuickCommandInit(void)
{
    if (c.running == NULL)
        c.running = &c;
    else return;
    for (int i = 0; i<MAX_NUM_GUM; i++)
    {
        int str = (i*MAX_NAM_LEN);
        c.running->names[i] = &c.running->buffer[str];
        c.running->buffer[str + TERMI_POS] =
            c.running->names[i][0] = '\0';

        
    }
    c.running->reserved = &c.running->buffer[MAX_NUM_GUM * MAX_NAM_LEN];
    c.running->buffer[(MAX_NUM_GUM * MAX_NAM_LEN) + TERMI_POS] =
    c.running->reserved[0] = '\0';
    c.running->reserved[MAX_NAM_LEN - 1] = 0;

#if using_stringPool
    pool_InitializeCycle();
#endif
    atexit( &DestructCommander );
}

// local function which parses the arguments.
int commandLineArgs( int count, char** argv )
{
    DEBUGLOG("begin parsing commandline...")

    clearAllErrors();

    if( count >= MAX_NUM_GUM ) {
        setErrorText(ERROR_ARGUM_COUNT);
        ERROR_CODE(c.running) = -(1000+(count-(MAX_NUM_GUM+5)));
    }

    const int isPushedState = c.running->numgum > 0;
    c.running->executor = argv[0];
    c.running->endOfThePath = -1;
    while( c.running->executor[++c.running->endOfThePath] != '\0' )
        c.running->executor[c.running->endOfThePath] = c.running->executor[c.running->endOfThePath]!='\\'
                                                     ? c.running->executor[c.running->endOfThePath]
                                                     : '/';

    c.running->namestateOfCommander = &c.running->executor[c.running->endOfThePath-2];
    do{ --c.running->endOfThePath;
    } while( c.running->executor[c.running->endOfThePath] != '/' && c.running->endOfThePath >= 0 );
    c.running->stateOfTheCommander = &c.running->executor[c.running->endOfThePath];

    for(int i = 0;i<COMMANDER_BUFFER;i++)
        c.running->buffer[i]=17;
    c.running->buffer[COMMANDER_BUFFER - 1] = '\0';
    c.running->numgum=0;
    c.running->numopt=0;

    if(wasError())
        return ERROR_CODE(c.running);

    for(int i=0;i<MAX_NUM_GUM;i++)
    {
        int str = (i*MAX_NAM_LEN);
        c.running->names[i]=&c.running->buffer[str];
        c.running->buffer[str + TERMI_POS] =
        c.running->names[i][0] = '\0';
        c.running->types[i] = 'd';
    }

    c.running->reserved = &c.running->buffer[ MAX_NUM_GUM * MAX_NAM_LEN ];
    c.running->buffer[(MAX_NUM_GUM * MAX_NAM_LEN) + TERMI_POS] =
    c.running->reserved[0] = '\0';

#ifdef using_environMentor
    if( !isPushedState )
        InitializeEnvironMentor();
#endif
    DEBUGFMT("first arg is: '%s'",argv[0])
    DEBUGLOG("entering parser loop")
    for(int i=1;i<count;i++)
    {
        char* current = argv[i];
        DEBUGFMT("next arg is: '%s'",argv[i])
        if(strlen(current)>TERMI_POS) {
            setErrorText(ERROR_ARGUM_LENTH);
            ERROR_CODE(c.running) = -i;
            return ERROR_CODE(c.running);
        }

  #ifdef __TINYC__
        int find;
        c.running->reserved[MAX_NAM_LEN]=-1;
     #if ARGUM_SLASHING
            find = -1; while((argv[i][++find]=argv[i][find]=='\\'?'/':argv[i][find])!='\0');
     #endif
     #if QUOTE_ENDOUBLING
            find = -1; while((argv[i][++find]=argv[i][find]=='\''?'\"':argv[i][find])!='\0');
     #endif
     #if SPACEN_DASHING
            find = -1; while((argv[i][++find]=argv[i][find]=='_'?' ':argv[i][find])!='\0');
     #endif
     #if BRACKET_ECKING
            find = -1;
        int fund = -1;
        do{ while( (argv[i][++find] = argv[i][find] == '[' ? '(' : argv[i][fund=find]) != '\0' && (fund == find) );
            if( c.running->reserved[++c.running->reserved[MAX_NAM_LEN]] = find - fund ) {
                int bind = find;
                while( (argv[i][++fund] = argv[i][fund] == ']' ? ')' : argv[i][bind=fund]) != '\0' && (bind == fund) );
                if( c.running->reserved[++c.running->reserved[MAX_NAM_LEN]] = fund - bind ) find = bind;
            }
        } while( c.running->reserved[c.running->reserved[MAX_NAM_LEN]] );
     #endif
  #endif
        if(argv[i][0]=='-')
        {
            if(argv[i][1]=='-')
            {
                if(current[2]=='+') {
         #ifdef using_environMentor
                    if(!environMentor_CheckAndEnable())
                        return 0;
                    c.running->names[c.running->numgum][0] = '\0';
                    c.running->options[c.running->numgum] = 'v';
                    if(beginPersistChange(LOCAL)) {
                        char* fcc = current+3;
                        if( (i+1==count) && (FourCC(fcc)==FourCC("DEBU"))) {
                            setPersistEntry((current+3),"(1)");
                        } else
                            setPersistEntry((current+3),checkForSpaces(argv[++i]));
                       commitPersistChange();
                       c.running->names[c.running->numgum+1][c.running->options[c.running->numgum+1] = 0] = '\0';
                       i++;
                       printf( "Macro-Feature %s has been add to %s\n",
                               (current+3), getNameOfTheCommander() );
                       ERROR_CODE(c.running)=ERROR_MUST_RELOAD;
                    }
         #else
                    ERROR_CODE(c.running)=ERROR_NO_FEATURES;
         #endif
                } else if(current[2]=='-') {
         #ifdef using_environMentor
                    if(!hasFeatures())
                        return 0;
                    c.running->names[c.running->numgum][0] = '\0';
                    c.running->options[c.running->numgum] = 'v';
                    if(beginPersistChange(LOCAL)) {
                        removePersistEntry(current+3);
                       commitPersistChange(); i++;
                       printf( "%s has been removed from %s\n",
                               (current+3), getNameOfTheCommander() );
                       ERROR_CODE(c.running)=ERROR_MUST_RELOAD;
                    }
         #else
                    ERROR_CODE(c.running)=ERROR_NO_FEATURES;
         #endif
                // } else if (current[2]==' ') {
					// printf("XXXXXXXXXXXXXXXXXXXX doubledash!!!\n");
					// parameterCopy( c.running->names[c.running->numgum], DOUBLEDASHED_OUT );
                    // c.running->types[c.running->numgum++] = 'R';
				} else {
                    c.running->options[c.running->numgum]=current[2];
                    if(argv[i][3]=='-') {
                        parameterCopy( c.running->names[c.running->numgum],
                                       checkForSpaces( current+4 ) );
                        c.running->types[c.running->numgum] = 'N';
                    } else {
                        parameterCopy( c.running->names[c.running->numgum],
                                       checkForSpaces( current+2 ) );
                        c.running->types[c.running->numgum] = 'M';
                    } ++c.running->numgum;
                }
            } else {
                c.running->options[c.running->numgum]=argv[i][1];
                c.running->names[c.running->numgum][0] = '\0';
                c.running->types[c.running->numgum] = 'S';
                ++c.running->numgum;
            } ++c.running->numopt;
        } else {
            parameterCopy( c.running->names[c.running->numgum],
                           checkForSpaces( current ) );
            c.running->types[c.running->numgum++] = 'R';
        }
        while(c.running->reserved[MAX_NAM_LEN]>=0)
              c.running->reserved[c.running->reserved[MAX_NAM_LEN]--]=0;
        c.running->reserved[MAX_NAM_LEN] = 0;
    }
    DEBUGFMT("parsed %i arguments",c.running->numgum)
    switch( ERROR_CODE( c.running ) ) {
        case ERROR_MUST_RELOAD: {
            printf("Source of '%s' has changed...\n", commanderUseExtension(true) );
            exit( EXIT_SUCCESS );
        } break;
        case ERROR_NO_FEATURES: {
            printf( "Command '%s' not supports FEATURED macros!\n",
                     commanderUseExtension(false) );
            exit( ERROR_NO_FEATURES );
        } break;
        default: if( ERROR_CODE( c.running ) )
            return ERROR_CODE( c.running );
        else break;
    }
   #if LINUX
    commanderUseExtension( true );
   #endif

    lastRawquested = 0;
    setReserved( bool, MAX_NAM_LEN-1, 0 );
    return c.running->numgum;
}


// gets the currently executing command's 'name'. (in
// most cases this would be filename of .c file which
// declares 'main(int,char**)' symbol for entrypoint
// into the currently executing, loaded memory image
cmLn getCommander(void)
{
    *c.running->stateOfTheCommander = '/';
    *c.running->namestateOfCommander = '.';
    return c.running->executor;
}
// get path to currently executing .c file
cmLn getPathOfTheCommander(void)
{
    *c.stateOfTheCommander = '\0';
    return c.executor;
}
// get filename (.c file) of currently executing command
cmLn getNameOfTheCommander(void)
{
    return c.running->stateOfTheCommander+1;
}

// sets if 'getNameOfTheCommander()' retrieves full filenam
// (e.g. base64.c) or just command 'name' (base64) when
// it's identifying itself (may be relevant in some cases
// when executed on Unix-bash based commandline interpreters
// which don't resolve names if extension would be ommitted.
cmLn commanderUseExtension(bool shouldCommanderUseExtension)
{
    *c.running->namestateOfCommander =
         shouldCommanderUseExtension ?  '.' : '\0';
    return getNameOfTheCommander();
}

// number of present arguments at all.
int numGum(void)
{
    return c.running->numgum;
}
// count on just 'tagged' arguments (it's numgum - number of raw names)
int optNum(void)
{
    return c.running->numopt;
}
// count on just raw, untagged arguments ( it's numGum()-optNum() )
int rawNum(void)
{
    return c.running->numgum - c.running->numopt;
}

// gets positional index of the argument tagged by 'option'
int indexOf(cmOp option)
{
    int i = c.running->numgum+1;
    while((i--)>0)
        if(option == c.running->options[i])
            return i;
    return i;
}

// gets the index position of given 'name'
// or -1 if 'name' doesn't exist.
int indexOfName( cmLn name )
{
    for(int i=0;i<c.running->numgum;i++)
        if( !stringCompare( name, c.running->names[i] ) )
            return i;
    return -1;
}
// gets the first index position where given wildcard
// could match or -1 if wildcard doesn't match nowhere.
int indexOfFirst( cmLn part )
{
    if(*part) {
        char* p=(char*)part;
        do{ if( *p++ == '*' )break;
            } while(*p);
        if(--p == part)
            part++;
        else if(*p=='*')
           *p--='\0';

        if( part <= p ) {
		 // means part was ending on '*'
            for( int i=0; i<c.running->numgum; ++i ) {
                uint cmp = stringCompare( part, c.running->names[i] );
				if ( cmp > strlen( part ) || !cmp ) {
					return i;
				}
			}
        } else {
		 // means '*' was in the beginning
            const int len = (int)strlen(part);
            char rvrs[MAX_NAM_LEN];
            char trap[MAX_NAM_LEN];
            char* cur = (char*)(part + len);
            char* rev = &trap[0]-1;
            while( *++rev = *--cur );
            for(int i=0;i<c.running->numgum;i++) {
                rev = &rvrs[0]-1;
                cur = &c.running->names[i][0]
                    + strlen(&c.running->names[i][0]);
                while( *++rev = *--cur );
                if(strcmp(&trap[0],&rvrs[0])>=len)
                    return i;
            }
        }
    }
	printf( "%s(): returns: -1\n", __FUNCTION__ );
	return -1;
}

cmOp byIndexTheOption(int index)
 {
    if( index < MAX_NUM_GUM && index >= 0 ) {
        return c.running->options[index];
    } else {
        setErrorText("index out of range");
        return '\0';
    }
}

cmLn getNameByIndex(int index)
{
    return index < 0 || index > MAX_NUM_GUM
         ? NoString : c.running->names[index];
}

cmLn getName( cmOp option )
{
    return getNameByIndex( indexOf(option) );
}

// finds the n'th raw name within the cmdLine...
// (here 'number' is not real index, but index
// in context of list of just 'raw' names only)
cmLn rawName( int number )
{
    int currentSearch = number;
    for(int i=0;i<c.running->numgum;i++){
        if(!isTagged(c.running->names[i])) {
            if(!(--number)) {
                lastRawquested = currentSearch;
                return c.running->names[i];
            }
        }
    } lastRawquested = 0;
    return NoString;
}

// iteration over just raw names
cmLn nextRaw(void)
{
    return rawName(lastRawquested+1);
}

// tries to find first untagged raw parameter located neares
// to some 'option' (e.g. rawNext('f') called on command lines:
//
// --foo bar
//    -> would retreive 'bar'
//
// bar --foo
//     <- would also retreive 'bar'.
//
// --foo -x --plup -7 bar
//    -> as well... would return 'bar'
//
// --z-100 bar -p -x --foo -u moo 
//                      -> returns 'moo'
//
// --z-100 bar --foo -u moo
//             <- returns 'bar'
//
cmLn rawNext( cmOp option )
{
    lastRawquested = 0;
    int  optex = indexOf(option);
    cmLn search = "\0";
    bool direction = true;
    do { search = nextRaw();
        if(!lastRawquested) direction = false;
    } while( direction ? (optex > indexOfName(search))
                       : (optex < indexOfName(search)) );
    return search;
}

// hasOption('f') returns true if cmdLine containes tagged
// parameter --f-barfoo, just switch -f, modus --foo as well on
// --foo-bar (uncommon but valid, would be modus 'foo-bar')
cmBl hasOption( cmOp option )
     { return 1 + indexOf(option); }

// returns true if argument 'name' is double dashed, or
// if 'name' is tagged by same letter it's beginning with
//     --foo as well as --f-foo would be treated being modus 'foo'.
cmOp isModus( cmLn name )
     { return name[0] == c.running->options[indexOfName(name)]; }

// The 'name' parameter supports resolving '*' in wildcards, so:
//     --f-fooBar as well --fooBong would be treated being modus foo*
//     --flup as well as --f-flump would be treated being modus flu*
cmOp isAnyModus( cmLn wild )
     { return wild[0] == c.running->options[indexOfFirst(wild)]; }

// are any token bound to option letters matching their first character?
cmBl isAnyModusAtAll( void )
{
    int modi=0;
    for(int i=0;i<c.running->numgum;i++) {
        modi += c.running->options[i] == c.running->names[i][0];
    } return modi;
}

// check if 'modus' is not 'solo' (not the the only modus set)
cmBl isAnyOtherModusThen( cmLn modus )
{
    return ( isAnyModusAtAll() - ( isModus(modus) ? 1 : 0 ) ) > 0;
}

// return that modus which first character equals given 'option'
// or, if given 'option' doesn't defines any modus, or if 'option' 
// maybe doesn't exists at all,... then 'NoString' is returned.
cmLn getModus( cmOp option )
{
    int optex = indexOf(option);
    if( optex >= 0 ) {
        return c.running->names[optex][0] == option
             ? c.running->names[optex]
             : NoString;
    } return NoString;
}

int idxOfModusNumber( int number )
{
    if (!number) return c.running->numgum;
    int modex=0;
    for( int i=0; i<c.running->numgum; ++i ) {
        if( ( modex += (c.running->options[i] == c.running->names[i][0])
             ) == number )
            return i;
    } return c.running->numgum;
}

// return the modus at modus-(one-based)-index 'number'
// (the n'th modus given - not index of tokken at all!)
cmLn getModusNumber( uint number )
{
    if (!number) return NoString;
    return c.running->names[ idxOfModusNumber(number) ];
}


int modusNumber( cmLn mode ) {
    int modex=0;
    for(int i=0;i<c.running->numgum;++i) {
        int ismode = (c.running->options[i]==c.running->names[i][0]);
        if( ismode && mode[0] == c.running->options[i] )
            return modex+1;
        else modex += ismode;
    } return 0;
}

// is there <parameter> (*wildcard allowed) given
// between <modus> and the next modus or (if <modus>
// may be the last modus given) between <modus> and
// the end of the command line 
cmBl hasModus( cmLn modus, cmLn parameter )
{
    cmOp group = isAnyModus( modus );
    DEBUGFMT("searching through modus: %s",modus)
    if ( group ) { int member = indexOfFirst( parameter );
        if ( member > indexOf( group ) ) {
            int modex = modusNumber( modus );
            DEBUGFMT("modex is %i",modex)
            return isAnyModusAtAll() >  modex
                 ? member < idxOfModusNumber( modex + 1 )
                 : true;
        }
    } return false;
}

// disable any other modi beside the given one
void setSolo( cmLn modus )
{
    if( !( modus[0] && indexOfName(modus) + 1 ) )
        setOption( modus[0], modus );
    for( int i=0; i<c.running->numgum; ++i ) {
        cmOp opt = c.running->options[i];
        if( opt == c.running->names[i][0] )
            if( opt != modus[0] )
                switchIdx( opt, EMPTY );
    }
}

// returns true if option is indexed, but got no
// name value assigned. asume following: -x --y-530
// isSwitch('x') returns true, but
// isSwitch('y') should return false. ('y' got value 530)
cmOp isSwitch( cmOp option )
     { cmOp o = hasOption( option );
       return o ?( (c.running->names[o-1][0] =='\0')
                || (c.running->names[o-1][0] ==' ')
                || (c.running->names[o-1][0] == 8 )
                ): false; }

// returns true if requested name is 'tagged' by some option letter
cmOp isTagged( cmLn name )
     { return c.running->options[indexOfName(name)]; }

// sets or over writes the name corresponding to 'option'
// if 'option' does not exist yet, a new parameter will be added
cmLn setOption( cmOp option, cmLn name )
{
    int index = indexOf(option);
    index = (index<0) ? c.running->numgum++ : index;
    c.running->options[index] = (char)option;
    parameterCopy(c.running->names[index],name);
#if DEBUG
    printf("%s('%c'): to \"%s\"\n", __FUNCTION__,option,c.running->names[index]);
#endif
    return c.running->names[index];
}

// set new Modus 'modus'.   will overwrite any option,
// switch or already existing modus which could exist
// for this tag letter which is first char of 'modus'
void setModus( cmLn modus )
{
    if(!isEmptyArg(modus)) {
        setOption( modus[0], modus );
        c.running->types[ indexOf(modus[0]) ] = 'M';
    } else {
        setErrorText( "modus" );
    }
}

// sets an option to be switch (will remove
// any name which could be bound already!)
void setSwitch( cmOp option )
{
    setOption(option,NoString);
}

// Changes the index of a given tag/value pair.
// If 'newOpt' not exist yet, a new option will be add,
// and, if at 'newIdx' already a name value exists,
// that name value will be new name at given index.
// if at these position where newIdx points to, values
// exist already, these values then will be swaped
// against the new ones. when 'newIdx' is
// given 'EMPTY' and 'newOpt' doesn't exist yet,
// 'newOpt' will be new switch at first option-slot
// which not is in use yet. (like 'setSwitch(newOpt)')
cmLn switchIdx( cmOp newOpt, cmIx newIdx)
{
#if DEBUG
    printf( "switchIdx('%c',%i)->", newOpt, newIdx );
#endif
    lastRawquested = 0;
    int curIdx = indexOf(newOpt);
    curIdx = curIdx < 0
	       ? c.running->numgum++
		   : curIdx;
    if( newIdx == EMPTY ) {
        if ( curIdx == ( c.running->numgum - 1 ) ) {
            while( c.running->options[ ++newIdx ] );
#if DEBUG
            printf( "shifted option '%c' to index '%i' (prior '%c')\n",
                    newOpt, newIdx, c.running->options[newIdx] );
#endif
        } else return getName( newOpt );
    }

    c.running->options[curIdx] =
     curIdx>=c.running->numgum ? newOpt
     :newIdx<c.running->numgum ? c.running->options[newIdx]
                               : NULL;
    c.running->options[newIdx] = (char)newOpt;
    c.running->numgum += (newIdx >= c.running->numgum);
    cmLn nameOfNewOpt = getName(newOpt);
    if( isValidArg(nameOfNewOpt) )
        c.running->types[newIdx] = nameOfNewOpt[0]==newOpt
                                 ? 'M' : 'N';
    else c.running->types[newIdx] = 'S';
    #if DEBUG
            printf( "shift option '%c' to name '%s' at index '%i'\n",
                    newOpt, nameOfNewOpt, newIdx );
    #endif
    return nameOfNewOpt;
}

// check if the argument tagged <option> was added by the programm
// after parsing, and so wasn't a 'really' user given commandline parameter
cmBl isDefault( cmOp option )
{
    return c.running->types[ indexOf(option) ] > 'Z';
}

// removes the option tag from an option token.
// If it has a corresponding name value, it's
// name will become 'raw' parameter then. If the
// token is a switch it's removed completely.
void makeRawName(cmOp option)
{
    int idx = indexOf(option);
    if (idx>=0)
        c.running->options[idx] = NULL;
}

// removes an option completely from the cmdlineState.
// if it has a corresponding name value assigned,
// this will also be removed from the cmdlineState.
void noOption(cmOp option)
{
    int idx = indexOf(option);
    if (idx>=0) {
        c.running->options[idx] = NULL;
        parameterCopy(c.running->names[idx],NoString);
    }
}

void showOptions(void)
{
    printf( "\n%s recognized input tokken:\n\n",
             getNameOfTheCommander() );

    for(unsigned i=0;i<c.running->numgum;i++) {
        if( !isEmptyArg( c.running->names[i] ) ) {
            char tag[5] = { '"', '-', c.running->options[i], '"', 0 };
            printf( "%i. %s %s : %s\n", i,
                    (c.running->types[i] <= 'Z')
                  ? (c.running->types[i] == 'M')
                    ? "   modus  ":"   name   "
                  :   "  default ",
                    c.running->options[i] ? &tag[0] : "raw ",
                    c.running->names[i] );
        } else
            printf( "%i. %s \"-%c\"\n", i,
                    (c.running->types[i] <= 'Z')
                  ? "  switch  " : "  default ",
                    c.running->options[i] );
    } printf("\n");
#ifdef using_environMentor
    showPersistente(LOCAL);
#endif
}

uint stringCompareInternal( cmLn proof, cmLn probe, uint index )
{ ++index;
    const char A=*probe++;
    const char B=*proof++;
    return !(A||B) ? EMPTY
           :(A!=B) ? index
           : stringCompareInternal(
	   proof, probe, index
    );
}

uint stringCompare( cmLn proof, cmLn probe )
{
    return stringCompareInternal( proof, probe, EMPTY ) + 1;
}

cmLn parameterCopy( char* dst, cmLn src )
{
    if ( (void*)src != (void*)dst ) {
#ifdef __TINYC__
        Termi( dst ) = -1;
        while( dst[ ++Termi(dst) ] = src[ Termi(dst) ] );
#else
        strcpy(dst,src);
#endif
    } return (cmLn)dst;
}

cmBl wasError(void)
     { return ERROR_CODE(c.running) != EXIT_SUCCESS; }


cmLn getError(void)
{
    return ERROR_NAME( c.running );
}
int getErrorCode(void)
{
    return ERROR_CODE( c.running );
}
int setError( cmLn text, int code )
{
    setErrorText( text );
    setErrorCode( code );
    return ERROR_CODE( c.running );
}
cmLn setErrorText( cmLn text )
{
	strcpy( &c.running->CommanderError[4], text );
    ++ERROR_CODE( c.running );
    return ERROR_NAME( c.running );
}
cmLn setErrorCode( int code )
{
    if ( ERROR_CODE( c.running ) == EXIT_SUCCESS )
        strcpy( &c.running->CommanderError[4], "unknown error" );
    ERROR_CODE( c.running ) = code;
    return ERROR_NAME( c.running );
}
cmBl CheckForError(void)
{
    if(wasError())
        printf( "ERROR_%i: %s\n",
                 ERROR_CODE(c.running),
                 ERROR_NAME(c.running) );
    return ERROR_CODE(c.running);
}

cmBl catchErrorCode( int error )
{
    if( error == ERROR_CODE(c.running) ) {
  #if DEBUG
        printf( "catchErrorCode(%i): '%s'\n",
                ERROR_CODE(c.running),
                ERROR_NAME(c.running) );
  #endif
        clearAllErrors();
        return error;
    } return false;
}

cmBl catchError( const char* errorstring )
{
    if( !stringCompare( ERROR_NAME(c.running), errorstring ) )
        return catchErrorCode( ERROR_CODE(c.running) );
    return false;
}

void clearAllErrors( void )
{
    ERROR_CODE(c.running) = EXIT_SUCCESS;
   *ERROR_NAME(c.running) = *"OK";
}

uint isCleansening(void)
{
    if ( c.running->reserved[MAX_NAM_LEN - 1] != 0 ) {
        return true;
    } else {
        return false;
    }
}

void setCleansening(void)
{
    c.running->reserved[MAX_NAM_LEN - 1] = 100;
}

void DestructCommander( void )
{
    setCleansening();
  #if DEBUG
    printf( "%s(): begin cleansening!\n", (cmLn)__FUNCTION__ );
  #endif
#if defined( using_environMentor )
    FeatureGet autocommit = getFeatured( "ENVIRONMENTOR_AUTO_COMMIT" );
    if ( autocommit.type.flags & Type_NUMBER ) {
        if ( autocommit.value.number ) {
            if ( nonCommittedChanges() )
                commitPersistChange();
        }
    }
#endif

    while( PopCommandLine( false ) );
    while( c.running->dingens ) {
        Ding* weg = c.running->dingens;
        ulong dingsbumsId = weg->dasda;
        if( weg->which ) weg->which();
        weg->bumms = 0;
        c.running->dingens = weg->dings;
        free( weg );
        #if DEBUG
        printf( "%s(): %s dingens removed\n",
                (cmLn)__FUNCTION__, (cmLn)&dingsbumsId );
        #endif
    }
  #if DEBUG
    printf("%s(): cleansening done!\n",__FUNCTION__);
  #endif
}

void ExitOnError( const char* where )
{
    if( wasError() ) {
        printf("%s error: %s\n", where, ERROR_NAME(c.running) );
        exit( ERROR_CODE(c.running) );
    }
}

char* getTemp() {
    return c.running->reserved;
}
char* setTemp(const char* name) {
    return strcpy(c.running->reserved,name);
}
char* setTempf( const char* fmt, const char* txt ) {
    tempf( fmt, txt );
    return getTemp();
}
int tempf(const char* fmt, const char* name) {
    sprintf(c.running->reserved,fmt,name);
    return (int)strlen(c.running->reserved);
}

cmLn unQuoted(cmLn arg)
{
    const int end = (int)strlen(arg)-1;
    if( arg[0]=='\"' && arg[end] == '\"') {
        setTemp( arg+1 )[end-1] = '\0';
        return getTemp();
    } else {
        return arg;
    }
}
cmLn toQuoted(cmLn arg)
{
    const int end = (int)strlen(arg)-1;
    if( arg[0]!='\"' && arg[end]!='\"' ) {
        char* chk = (char*)arg;
        char* out = setTemp("\"")+1;
        *out=*chk;
        while( *++out = *++chk ) {
            if(*chk=='\"') {
               *out++='\\';
               *out='\"'; }
        } *out++='\"';
        *out='\0';
        return getTemp();
    } else {
        return arg;
    }
}

uint toSplitList(char* sepList, char* fromTo)
{
    uint elms = *sepList-- ? 1 : 0;
    char seppel = fromTo ? fromTo[0] : NULL;
    if( !seppel )
         seppel = WINDOWS ? ';' : ':';
    char splitl = fromTo ? fromTo[1] : NULL;
    int ganz = false;
    while(*++sepList ) {
        if( *sepList == '\"' && seppel == ' ' )
            ganz = !ganz; else
        if( *sepList == seppel && (!ganz) ) {
            *sepList = splitl;
           ++elms; }
    } return elms;
}

const char* printSpList( char* list )
{
    uint elmcount = toSplitList(list, "\0\n");
    DEBUGFMT("%i elements", elmcount)
    return list;
}

#ifdef __TINYC__
#define SCRIPT_DEBUG DEBUG
#else
#define SCRIPT_DEBUG 0
#endif
void* getDingens( const char* named )
{
    #if SCRIPT_DEBUG
    printf( "%s(): looking for a '%s' dingens\n", __FUNCTION__, named);
    #endif
    ulong get = Dingens( named );
    if ( c.running->dingens ) {
        Ding* dings = c.running->dingens;
        do{
            #if SCRIPT_DEBUG
            printf( "%s(): trying das da: %s\n", __FUNCTION__, Maskens( dings->dasda ) );
            #endif
            if( dings->dasda == get ) {
                #if SCRIPT_DEBUG
                printf( "%s(): found it :)\n", __FUNCTION__ );
                #endif
                return dings->bumms;
            }
        } while( dings = dings->dings );
    }
    #if SCRIPT_DEBUG
    printf( "%s(): NOT found the %s :(\n", __FUNCTION__, Maskens(get) );
    #endif
    return NULL;
}

void remDingens( const char* named )
{
    if( isCleansening() ) {
    DEBUGFMT("%s not possible,... already cleansening!",named)
        setErrorText("Cleansening");
        return; }
    DEBUGLOG( named )
    cmDn get = Dingens( named );
    if ( c.running->dingens ) {
        Ding  baseDings;
        baseDings.dings = c.running->dingens;
        Ding* base = &baseDings;
        while (base->dings) {
           if (base->dings->dasda == get) {
               base->dings = base->dings->dings;
               break;
            } base = base->dings;
        }
    }
}

void addDingens( const char* named, void* dingens, void(*bumms)(void) )
{
  #if DEBUG
    printf("%s(): create '%s' dingens\n",__FUNCTION__,named);
  #endif
    cmDn add = Dingens( named );
    Ding* dings = c.running->dingens;
    if ( !c.running->dingens ) {
        if ( add ) {
            c.running->dingens = (Ding*)malloc( sizeof(Ding) );
            dings = c.running->dingens;
            dings->dasda = add;
            dings->dings = NULL;
        } else return;
    } else while( dings ) {
        if( dings->dasda == add ) break;
        dings = dings->dings;
    } if( !dings ) {
        if ( dingens ) {
            Ding* das = c.running->dingens;
            while( das ) if( !das->dings ) {
                das->dings = (Ding*)malloc( sizeof(Ding) );
                das->dings->dasda = add;
                das->dings->dings = NULL;
                dings = das->dings; break;
            } else das = das->dings;
        }
    } dings->bumms = dingens;
    dings->which = bumms;
}

#undef SCRIPT_DEBUG

#else
#undef NOT_COMPILE_FILE
#endif