/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      cLine.cc                                   ||
||     Author:    Kalle Muenster                             ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include ".junkYard.h"
#include ".stringPool.h"
#include ".commandLiner.h"
#include ".cLine.h"
#endif

#include <stdlib.h>
#if SYSTEM==WIN
#include <process.h>
#endif
#include <libtcc.h>



const char* DocumentationFor_cLine = "\n \
builds a commandline from all given tokken and executes.\n\n \
resolve C-Macros by leading '='-sign \n \
merge any tokken by leading '~'-sign\n\n \
Use --f-<commands> to access featured macros\n \
defined in a command's' feature-header\n \
Use --i-<filenames> to include c-header\n \
and resolve any symbols defined in these.\n \
Use --d-<variables> to declare variables\n \
'type name value' so token can refere to.\n\n";

const char* more_help = "\nPersistent Options:\n\n \
--setEcho :\n\n \
  Permanently enables or disables echoing commands before executing these.\n \
\n \
            --setEcho   <on|off>\n \
\n \
--setAsync :\n\n \
  Enables or disables asynchronous execution. (If the commandline\n \
  should wait till execution finished, or if it should 'spawn'\n \
  commands as new task, without awaiting any result)\n \
\n \
            --setAsync   <on|off>\n \
\n \
--setBuffer :\n\n \
  Set buffer sizes in bytes for different usage:\n \
\n \
            --setBuffer   <[inner|outer|cycle]> \n \
\n \
  cycle: work buffer for generating code fragments, parsing and \n \
     pasting argument and for passing function parameters.. \n \
  outer: buffer which is used for storage of generated autocode.\n \
     a compiler instance will get this passed for compiling \n \
     code fragments which will generate a command line when executed.\n \
  inner: buffer used by the executing autocode for creating the actual\n \
     command line - which is passed back to system for being executed.\n \
\n \
--setNumType :\n\n \
  Set used number format for evaluating mathematical expresions:\n \
\n \
            --setNumType  <int|float> \n \
\n \
  can be defined either to <int> or to <float> number format.\n \
  For any argument token prefixed by '=' signs, autocode will\n \
  be generated for resolving these. By this 'NumType' setting\n \
  can be controlled how code generator handeles solving of\n \
  numerical expressions within token. following expression:\n \
\n \
            '=MACRO(3.7/&fileName[5])'\n \
\n \
  would leed to different results depending on the number format\n \
  which was choosen.\n \
\n \
--setIncHdr/--addIncHdr :\n\n \
  set coma separated list (or add to existing list) of c-header files\n \
  which any autocode fragments always should include when compiled...\n \
  (this does same like the --i- parameter does, but permanently, so every\n \
  cLine commandline which is executed will include these)\n \
\n \
            --setIncHdr \"math.h,stdio.h\"\n \
\n \
  would make cLine always including 'math.h' and 'stdio.h' when generating code\n \
\n \
--setIncPth/--addIncPth :\n\n \
  set coma separated list (or add to the existing list) of filesystem\n \
  directories which cLine command will look through when resolving #include\n \
  statements contained within autocode fragments - e.g. this defines where to\n \
  lookup include headers given by --i- parameter and/or these contained in the\n \
  persistent IncHdr list which can be configured via --setIncHdr/--addIncHdr\n \
\n \
--setDefine/--addDefine :\n\n \
  set coma separated list (or add to existing list) of #define statements which\n \
  will be accessible by generated code fragments. (each given macronym=macrocode)\n\n\
  ";

int USAGE(cLine)
{
    if(isSwitch('v'))
        showOptions();

    Synopsis("[options] < [~]CommandTokken | [~]=CMacro | <~|=>~CExpression >");
    printf( DOKU(cLine) );

    if( isModus("help") ) {
        printf( "%s", more_help );
    } else {
        printf("See [--help] for more details.\n\n");
        setErrorText("no arguments given!");
    }

    return CheckForError();
}

int setBufferSize(char* buffersize)
{
    char* innerbuffer = buffersize;
    while(*(++buffersize)!='\0')
        if(*buffersize==',') {
            *buffersize = '\0';
            buffersize++;
            break; }

    if( atoi(innerbuffer)<256 || atoi(buffersize)<256 ) {
        setErrorText("buffer sizes (',' separated) must be at least minimum of 256 byte!");
        return false;
    } else {
        setPersistEntry("CLINE_INNER_BUFFER",pool_setf("(%s)",innerbuffer));
        setPersistEntry("CLINE_OUTER_BUFFER",pool_setf("(%s)",buffersize));
        printf( "Buffer sizes will be set to: inner buffer: %s, outer buffer: %s\n",
                innerbuffer, buffersize );
        return true;
    }
}

int setEchoMode( char* echoMode )
{
    char val[4] = {"(x)\0"};
    if(echoMode[0]=='o') {
       val[1] = echoMode[1]=='n'
           ? '1'
              : echoMode[1]=='f'
           ? '0'
              :  'x';
    } if(val[1]!='x') {
        setPersistEntry("CLINE_ECHO",&val[0]);
        printf("echo cLine command before executing will be: %s\n",
                val[1]=='1' ? "ENABLED" : "DISABLED" );
        return true;
    } else
        setErrorText("setEcho must given argument 'on' or 'off'!");
    return false;
}

int setAsyncMode( char* asyncMode )
{
    char val[4] = {"(x)\0"};
    if(asyncMode[0]=='o') {
        val[1] = asyncMode[1]=='n'
            ? '1'
               : asyncMode[1]=='f'
            ? '0'
               : 'x';
    } if(val[1]!='x') {
        setPersistEntry("CLINE_ASYNCHRON",&val[0]);
        printf("cLine: asynchronous execution will be %s\n",
                val[1]=='1' ? "ENABLED" : "DISABLED" );
        return true;
    } else {
        setErrorText("just \"on\" or \"off\" parameter please!");
        return false;
    }
}

int setNumberType( char* numberType )
{
    switch( numberType[0] ) {
    case 'F':
    case 'f':
        pool_set("\"%%g\\\",(double)%s\"");
        pool_set( "FLOAT" );
        break;
    case 'I':
    case 'i':
        pool_set( "\"%%s\\\",ulltoa((ulong)%s,&value[0],10)\"" );
        pool_set( "INTEGER" );
        break;
    default:
        setErrorText( "supported number-formats: [float|int]" );
        return false;
    }
    setPersistEntry( "NUMBER", pool_last(2) );
    printf( "cLine number format will be set to: %s\n",
             pool_last(1) );
    return true;
}

int persistSEToption( cmLn setModus, cmLn setValue )
{
    uint lstop = 0x00ffffff & FourCC(setModus);
    const char* modswtch = setModus+3;
    uint modeswitch = FourCC(modswtch);
    DEBUGFMT(" string: %s",setModus+3)
    DEBUGFMT("  value: %i",modeswitch)
    switch( modeswitch ) {
        case FOURCC('I','n','c','H'): return listedFeature(lstop,"INCLUDES",setValue);
        case FOURCC('I','n','c','P'): return listedFeature(lstop,"PATH",setValue);
        case FOURCC('D','e','f','i'): return listedFeature(lstop,"DEFINES",setValue);
        case FOURCC('N','u','m','T'): return setNumberType(setValue);
        case FOURCC('B','u','f','f'): return setBufferSize(setValue);
        case FOURCC('A','s','y','n'): return setAsyncMode(setValue);
        case FOURCC('E','c','h','o'): return setEchoMode(setValue);
        default: setErrorText( pool_setf("Unknown modus '%s'",setModus) );
                 return false;
    }
}


char* generateIncludes(const char*,uint,char*);
char* generateDeclares(const char*,uint,char*);
char* generateTypedefs(const char*,uint,char*);
char* generateResolver(const char*,uint,char*);

const char executeTokken[3] = { '\\', 'n', '\0' };
const ExecutableCmd proto_chunk = { NULL, NULL, EMPTY_(ulong) };


uint autoID = EMPTY;
ExecutableCmd cLine_compileExpresion( void* state, const char* /*IN*/ cCode, const char* /*IN*/ cIncl, const char* /*IN*/ fragmentName, uint* /*OUT*/ fragmentId )
{
    TCCState* compiler = (TCCState*)state;
    *fragmentId = ++autoID;
    int isAPIcall = false;
    if( cCode ) {
        pool_setf("%s ",fragmentName);
        if ( cCode[0] == '(')
             pool_setf("=='%s'",cCode);
        else
        if ( cCode[0]=='\"')
             pool_setf("='%s'",cCode);
        else pool_setf("=='(%s)'",cCode);
        PushCommandLine( pool_merge(2) );
        DEBUGLOG( "pushed new commandline" )
        isAPIcall = true;
    }
    if ( cIncl ) {
        char* Cincl = (char*)unQuoted( cIncl );
        int pfade = toSplitList( Cincl,",\0" );
        while ( *Cincl ) {
            DEBUGFMT( "add_include_path: %s", Cincl )
            tcc_add_include_path( compiler, Cincl );
            while( *Cincl++ );
        } 
    }
    tcc_add_include_path( compiler, LOCUS_POCUS );
    tcc_add_include_path( compiler, getPathOfTheCommander() );
    tcc_set_output_type( compiler, TCC_OUTPUT_MEMORY );
    tcc_set_options( compiler, "-w" );

    // declare an 'outer' buffer which will be filled
    // with string data containing c-code to be passed
    // to the compiler for let an binary immage being
    // compiled where actually executing image (cLine.c)
    // then later can link to, to make possible these
    // functions which before where passed as c code
    // to the compiler then can be called from within
    // this scope here in cLine.c actually executing.
    char autoCode[CLINE_OUTER_BUFFER] = {'\0'};

    DEBUGLOG( "generating code..." )
    // code generation is done within 3 separate steps:
    // 1. precproc (#include <bla.h>'s, #define BLUB(s) )
    // 2. typedefs (declare types in resolver scope) <- still TODO
    // 3. vardecls (variables in the resolver scope) 
    // 4. function per each parsed commandline token
    generateResolver( fragmentName, autoID,
    generateDeclares( fragmentName, autoID,
    generateTypedefs( fragmentName, autoID,
    generateIncludes( fragmentName, autoID,
                      &autoCode[0] ) ) ) );

    if (hasOption('v')) {
        printf("\n%s\nnow compiling...\n", &autoCode[0]);
    }
    DEBUGFMT( "\n%s\nnow compiling...\n", &autoCode[0] )
    if( tcc_compile_string( compiler, &autoCode[0] ) == -1 ) {
        setErrorText( "some fishy string was generated. compilation failed!" );
        return proto_chunk;
    }
    if( tcc_relocate( compiler, TCC_RELOCATE_AUTO ) < 0 ) {
        setErrorText( "relocating code faild!" );
        return proto_chunk;
    }

    pool_setfi( "autocode_id%i", autoID );
    pool_setf( "_%s", fragmentName );
    ExecutableCmd resolverCall = proto_chunk;
    if( !( resolverCall.exec = tcc_get_symbol( compiler, pool_merge(2) ) ) ) {
        setErrorText( "can't resolve entrypoint-symbol in relocated code!" );
        return proto_chunk;
    }
    if( isAPIcall ) {
        PopCommandLine( true );
    } return resolverCall;
}

static bool isChunkValid( ExecutableCmd* chunk )
{
    if(!chunk->exec) {
        if(!wasError())
            setErrorText("invalid");
        return false;
    } return true;
}

const char* cLine_echoCompiledCall( ExecutableCmd* chunk )
{
    DEBUGLOG("echo")
    if( !isChunkValid( chunk ) ) {
        DEBUGLOG("invalid")
        printf( "%s ", getError() );
        setErrorText( "command" );
        return NoString;
    }
    DEBUGLOG("will call the compiled expression resolver:")
    if(!chunk->done)
        chunk->size = strlen( chunk->done = chunk->exec() ) - 1;
    if(!chunk->size)
        chunk->size = strlen( chunk->done ) - 1;
    chunk->done[chunk->size]='\0';
    return chunk->done;
}

const char* cLine_execCompiledCall( ExecutableCmd* chunk )
{
    DEBUGLOG("exec")
    if(!isChunkValid(chunk)) {
        DEBUGLOG("invalid")
        printf( "%s ", getError() );
        setErrorText( "command" );
        return NoString;
    }
    DEBUGLOG("will call the compiled expression resolver:")
    if(!chunk->done)
        return chunk->done = chunk->exec();
    if(!chunk->size)
        return chunk->done;
    chunk->done[chunk->size] = '\n';
    return chunk->done;
}

#ifndef _P_NOWAIT
#define _P_NOWAIT 1
#endif
#ifdef cLine
int main(int argc,char**argv)
{
    if( !InitCommandLiner(argc,argv)
     || hasOption('h') )
        exit(USAGE(cLine));

    int ASYNC_MODE = ( isSwitch('a') && (!isSwitch('s')) );
    DEBUGFMT( "ASYNC_MODE is: %s", ASYNC_MODE ? "true" : "false" )

    if( hasOption('v') ) {
        setModus( "verbose" );
        printf("\n");
        showOptions();
    }
	
    if( isAnyOtherModusThen( "verbose" ) ) {
        int success = false;
        if( isModus("test") ) {
            switchIdx( 'T', indexOfName( rawNext('t') ) );
            if(!isValidArg(getName('T')))
                setErrorText("test needs phrase given");
        } else {
        beginPersistChange( LOCAL );
            if( isAnyModus("set*") )
                success = persistSEToption( getModus('s'), rawNext('s') );
       else if( isAnyModus("add*") && (!success) )
                success = persistSEToption( getModus('a'), rawNext('a') );
       else if( isAnyModus("rem*") && (!success) )
                success = persistSEToption( getModus('r'), rawNext('r') );
            if( nonCommittedChanges() ) {
		commitPersistChange(); exit( CheckForError() ); }
		} ExitOnError( "Modus" );
    }

    uint callId;

    TCCState* compiler;
    if( !( compiler = tcc_new() ) ) {
        setErrorText( "Could not create compiler state\n" );
        tcc_delete( compiler );
    } ExitOnError( "Interpreter" );
	
	if (hasOption('v')) noOption('v');

    ExecutableCmd command = cLine_compileExpresion( compiler, NULL, getVariable("CLINE_PATH"), "resolver", &callId );

    int returnValue = EXIT_SUCCESS;
    if( !CheckForError() ) {
        int echomode = CLINE_ECHO;
        FeatureGet getresult = getFeatured("CLINE_ECHO");
        if( getresult.type.flags & Type_NUMBER ) {
            echomode = (int)getresult.value.number;
        } else if ( catchError( "CLINE_ECHO" ) ) {
            printf("Catched error: FeatureGet 'CLINE_ECHO'\n");
        }
        if( echomode ) {
            if( hasOption('v') )
                printf("calling the compiled code fragment created following output:\n");
            fprintf( stdout, "%s\n", CLineEcho(&command) );
        }

        int amode = ASYNC_MODE;
        if( !isSwitch('s') ) {
            amode = CLINE_ASYNCHRON;
            getresult = getFeatured("CLINE_ASYNCHRON");
            if( getresult.type.flags & Type_NUMBER ) {
                amode = getresult.value.number >= 1.0;
            } if ( catchError( "CLINE_ASYNCHRON" ) ) {
                printf( "Catched error: FeatureGet 'CLINE_ASYNCHRON'\n" );
            } 
        }
        DEBUGFMT("amode is: %i",amode)
        if( ASYNC_MODE || amode == 1 ) {
            #if SYSTEM==LNX
                returnValue = system( pool_setf("exec -c %s", CLineExec( &command ) ) );
            #else
                returnValue = system( pool_setf("start \"\" %s", CLineExec( &command ) ) );
            #endif
        } else {
            returnValue = system( CLineExec( &command ) );
        }
    } tcc_delete( compiler );
    ExitOnError( "Interpreter" );
    exit( returnValue );
    return 0;
}
#endif

static char* addDefine( const char* macronym, const char* macroval, char* outerBuffer )
{
    outerBuffer = &strcpy(outerBuffer,"#define ")[8];
    outerBuffer = &strcpy(outerBuffer,macronym)[strlen(macronym)];
    *outerBuffer++ = ' ';
    outerBuffer = &strcpy(outerBuffer,macroval)[strlen(macroval)];
    *outerBuffer++ = '\n';
    *outerBuffer = '\0';
    return outerBuffer;
}

static char* addDefineUnsplit( char* macroslic, char* outerBuffer )
{
    strcat(outerBuffer, "#define " );
    outerBuffer = outerBuffer+=8;
    char* def = macroslic;
    char* val = def;
    int gotValue = 0;
    while( *++val ) {
        if( *val == '=' ) {
            gotValue = 1;
            break; }
    } *val='\0';
    strcat(outerBuffer,def);
    outerBuffer += strlen(def);
    strcat(outerBuffer,gotValue?" ":"\n");
    if(!gotValue) {
        outerBuffer += strlen("\n");
        return outerBuffer;
    } else ++val;
    outerBuffer += 1;
    def = val-1;
    char* un = val;
    while(*++def) {
        if(*def=='\\')
            if( *(def+1) == '\"' )
                ++def;
        *un++ = *def;
    } *un = '\0';
    strcat(outerBuffer,val);
    outerBuffer += strlen(val);
    strcat(outerBuffer,"\n");
    outerBuffer += strlen("\n");
    return outerBuffer;
}

char* addInclude( const char* file, ulong style, char* outerBuffer )
{
    tempf((char*)&style,file);
    char* incStr = pool_setf("#include %s\n",getTemp());
    return strcat( outerBuffer, incStr ) + strlen(incStr);
}

// generate a virtual C code file in memory and place
// any #include's and #define's it needs for solving
// each of the expressions which are contained in the
// currently executed 'cLine' command .
char* generateIncludes( const char* resolverName, uint acId, char* outerBuffer )
{
    char withoutHeader = 1;
    char* position = addDefine("CLINE_INTERNAL","(1)",outerBuffer);
    position = addDefine("WITH_AUTOCODE_ID(var,nam,num)","var##_id##num##_##nam",position);
    position = addDefine("NO_COMMAND_DEPENDENCIES","(1)",position);
    position = addDefine("autocode","\"autocode\"",position);
    pool_set("_OnTheFly_=\"autocode\"");
    position = addDefineUnsplit( pool_get(), position );
    FeatureGet feature = getFeatured( "CLINE_DEFINES" );
    if( feature.type.flags & (Type_STRING | Type_LISTER) ) {
        *position++ = '\n';
        if( feature.type.flags & Type_LISTER ) {
            for(int i=0; i<feature.type.size[1]; ++i )
                position = addDefineUnsplit( feature.value.lister[i], position );
        } else  position = addDefineUnsplit( feature.value.string, position );
    } else { catchError( "CLINE_DEFINES" );
        DEBUGLOG("No 'defines' defined!")
    }
    DEBUGFMT("%s will include sources 'OnTheFly'...",_OnTheFly_)

    if( search('f') ) {
        *position++ = '\n';
        char* namespaces = getName('f');
        int spacecount = 0;
        int nPos = strlen(namespaces);
        while(--nPos>=0) {
            if( (namespaces[nPos]==',') || (nPos == 0) ) {
                withoutHeader = 1;
                if(nPos==0) nPos--;
                else namespaces[nPos]='\0';
                char* namespace = &namespaces[nPos+1];
                for(int i = strlen(namespace);i>0;i--) {
                    if(namespace[i]=='.') {
                        withoutHeader = 0;
                        namespace[i]='\0';
                        break;
                    }
                }
                if( withoutHeader ) {
                    position = addInclude( pool_setf("persistente/%s_features.h",namespace),
                                           LIB, position );
                } else {
                    position = addDefine( "_OnTheFly_", namespace, position );
                    position = addInclude( pool_setf(".%s.h",namespace), LIB, position );
                }
            }
        } getName('f')[0] = '\0';
    }
    position = addInclude( ".environMentor.h", LIB, position );
    #if DEBUG
    printf( "GETTING INCLUDES\n" );
    #endif
    feature = getFeatured( "CLINE_INCLUDES" );
    if ( feature.type.flags & Type_LISTER ) {
        *position++ = '\n';
        for ( int i=0; i<feature.type.size[1]; ++i )
            position = addInclude( feature.value.lister[i], LIB, position );
    } else if ( feature.type.flags & Type_STRING ) {
        position = addInclude( feature.value.string, LIB, position );
    } else { catchError( "CLINE_INCLUDES" );
        DEBUGLOG( "No cLine INCLUDES defined" )
    }
    DEBUGLOG( "searching for 'i' parameter" )
    if( search('i') ) {
        int fSeek = -1;
        int fPos = 0;
        *position++ = '\n';
        char* includes = getName('i');
        while(includes[++fSeek]!='\0') {
            if(includes[fSeek]==',') {
                includes[fSeek]='\0';
                position = addInclude( &includes[fPos], CWD, position );
                fPos=fSeek+1;
            }
        }
        position = addInclude( &includes[fPos], CWD, position );
        getName('i')[0]='\0';
    }
    char* firstTokken = getNameByIndex(0);
    if( strlen(firstTokken) ) {
    if( firstTokken[0] != '~' ) {
        pool_setf( "~%s", firstTokken );
        strcpy( firstTokken, pool_get() );
    } }
    DEBUGLOG( "including 'includes' Done!" )
    return &strcpy(position,"\n")[strlen("\n")];
}

char* generateTypedefs( const char* fragmentName, uint fragmentID, char* position )
{
    if ( hasOption('e') ) {
        cmLn defs = rawNext('e');
        //TODO later...
    } return position;
}

char* declFopen( char* position, char* decl, const char* fopenner, const char* fragment )
{
    decl = toSplitList( decl, " \0" );
    for( int i=0; i<3 && *decl; ++i) {
        char* unqu = unQuoted(decl);
        position = &strcpy(position,unqu)[strlen(unqu)];
        if ( i==2 ) {
            position = &strcpy( position, "\"," )[2];
            position = &strcpy( position, getName('m') )[ strlen(getName('m')) ];
        *position++ = ')'; }
        while(*decl++);
        *position++ = i>1?';':i?*decl?'=':';':' ';
        if (i==1) position = &strcpy( position, fopenner )[ strlen(fopenner) ];
    } *position++ = "\n";
    return position;
}

char* generateDeclares( const char* fragmentName, uint fragmentID, char* position )
{
    if ( search('d') ) {
        char* list = (char*)getName('d');
        int declsCount = toSplitList( &list[0], ",\0" );
        char fragment[MAX_NAM_LEN];
        sprintf( &fragment[0], "_id%i_%s", fragmentID, fragmentName );
        while( declsCount-- ) {
            char* decl = list;
            while(*list++);
            char decldata[MAX_NAM_LEN];
            switch( *(uint*)decl ) {
                case FOURCC('F','I','L','E'): {
                    if (!search('m') ) setOption('m',"\"r\"");
                    else setOption('m', toQuoted(getName('m')));
                    position = declFopen( position, decl,
                            " fopen(\"", &fragment[0]
                                          );
                } break;
                case FOURCC('B','6','4','F'): {
                    if (!search('m') ) setOption('m',"\"er\"");
                    else setOption('m', toQuoted(getName('m')));
                    position = declFopen( position, decl,
                           " base64_createFileStream(\"",
                             &fragment[0] );
                } break;
                default: {
					if (decl[0]=='~') ++decl;
                    const uint parts = toSplitList( decl, " \0" );
                    char* declcode = &decldata[0];
                    for( int i=0; i<3 &&  i<parts; ++i) {
                        declcode = &strcpy( declcode, decl )[ strlen(decl) ];
                        if (i==2) { break;
                        } else if (i==0) { *declcode++ = ' ';
                        } else if (i==1) { if (parts==3) {
                            *declcode++ = '='; } else { break; }
                        } while(*decl++);
                    } strcpy( declcode, ";\n" );
                    position = strcat( position, &decldata[0] )
                             + strlen( &decldata[0] );
                } break;
            }
        } getName('d')[0] = '\0';
    } return &strcpy(position,"\n")[strlen("\n")];
}

#define CMDARG "%%s\",\"%s\""
#define STRING "%%s\",%s"
#ifndef NUMBER
#define NUMBER "%%ull\",(ulong)%s"
#endif
#define NORMAL(tokken)  "\" " ## tokken
#define MERGED(tokken)  "\"" ## tokken
#define SINGLECASE(a)   case (a)
#define ZWEIERCASE(a,b) case ((b<<8)+a)


// begin creating code for writing a string containing the 'solver'
// function into the 'outer' string buffer.
// - This function later will solve these expressions which are
//   given by the input command line. When this function is compiled
//   by the virtual compiler instance and has been linked to the
//   actually executing cLine command, this function should be able
//   writing a string into the function's 'inner' buffer which so
//   will contain the calculated expression results then.
#define BEGIN_FUNCTION(auto,type,name,outerbuffer)  char* \
outerbuffer ##_StartAddress = outerbuffer; \
char* currentBufferPosition = outerbuffer; \
sprintf(currentBufferPosition,"char value[%i]=",CLINE_INNER_BUFFER); \
        currentBufferPosition += strlen(currentBufferPosition); \
sprintf(currentBufferPosition,"\"autocode_id%i_%s\";\n",auto,name); \
        currentBufferPosition += strlen(currentBufferPosition); \
sprintf(currentBufferPosition,"%s* WITH_AUTOCODE_ID(autocode,%s,%i)",type,name,auto); \
        currentBufferPosition += strlen(currentBufferPosition); \
sprintf(currentBufferPosition,"(void)\n{\n  pool_InitializeCycle();\n"); \
        currentBufferPosition += strlen(currentBufferPosition); \
sprintf(currentBufferPosition,"  char* innerBuffer = &value[32];\n"); \
        currentBufferPosition += strlen(currentBufferPosition)

// defines an expression (in most cases gatherd
// from a single commandline tokken) to be solved
#define RESOLVE_TOKKEN(typ,val) sprintf(currentBufferPosition, \
"  sprintf(innerBuffer," ## typ ## ");\n  innerBuffer+=strlen(innerBuffer);\n", \
val); currentBufferPosition+=strlen(currentBufferPosition)

// closes the function by returning that string
// buffer which should be filled with the results now
#define RETURN_FUNCTION(type,outerbuffer) \
sprintf(currentBufferPosition,"  pool_freeAllCycles();\n"); \
        currentBufferPosition+=strlen(currentBufferPosition); \
sprintf(currentBufferPosition,"  return (%s*)&value[32];\n}\n",type); \
        currentBufferPosition+=strlen(currentBufferPosition); \
return outerbuffer ## _StartAddress


// function which generates (by above construction macros)
// another function which is able resolving given expressions
// when executed by the virtual tcc instance
char* generateResolver( const char* resolverName, uint autoid, char* outerBuffer)
{
    BEGIN_FUNCTION( autoid, "char", resolverName, outerBuffer );
  #ifdef FILTER_RAWNAMES
    char* tokken; char* lastTokken = 0;
    while((tokken=nextRaw())>lastTokken) {
        lastTokken = tokken;
  #else
    for(int i=0;i<numGum();i++) {
        char* tokken = getNameByIndex(i);
  #endif
        if(tokken[0]=='~'||tokken[0]=='=') {
            unsigned offset = 1;
            switch( tokken[0] + (tokken[offset]=='~' || tokken[offset]=='='
                              ? (tokken[offset++]<<8)
                              : 0 )) {
                SINGLECASE('='):     RESOLVE_TOKKEN( NORMAL(STRING), &tokken[offset] ); break;
                SINGLECASE('~'):     RESOLVE_TOKKEN( MERGED(CMDARG), &tokken[offset] ); break;
                ZWEIERCASE('=','='): RESOLVE_TOKKEN( NORMAL(NUMBER), &tokken[offset] ); break;
                ZWEIERCASE('=','~'): RESOLVE_TOKKEN( MERGED(NUMBER), &tokken[offset] ); break;
                ZWEIERCASE('~','='): RESOLVE_TOKKEN( MERGED(STRING), &tokken[offset] ); break;
                ZWEIERCASE('~','~'): RESOLVE_TOKKEN( MERGED(NUMBER), &tokken[offset] ); break;
            }
        } else RESOLVE_TOKKEN( NORMAL(CMDARG), tokken );
    }
    RESOLVE_TOKKEN( MERGED(CMDARG), &executeTokken[0] );
    RETURN_FUNCTION( "char", outerBuffer );
}




