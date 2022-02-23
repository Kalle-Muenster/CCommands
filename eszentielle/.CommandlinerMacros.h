/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      .CommandlinerMacros.h                      ||
||     Author:    Kalle                                      ||
||     Generated: 05.02.2018                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _CommandlinerMacros_h_
#define _CommandlinerMacros_h_

#ifndef EMPTY
#define EMPTY (0xFFFFFFFF)
#define EMPTY_(T) ((T)-1)
#endif

#define fixate(n) n
#define prefix(prfx,name) fixate( prfx ## _ ## name )
#define suffix(name,sffx) fixate(name)_ ## sffx
#define AsInt(pt4chars) (*(int*)pt4chars)
#define FourCC(p4chars) (*(uint*)p4chars)
#define LongCC(p8chars) (*(ulong*)p8chars)
#define FOURCC(a,b,c,d) (d<<24|c<<16|b<<8|a)
#define CASE64(fccA,fccB) (ulong)((ulong)fccB<<32|(ulong)fccA)
#define LONGCC(a1,b1,c1,d1,a2,b2,c2,d2) CASE64(FOURCC(a1,b1,c1,d1),FOURCC(a2,b2,c2,d2))
#define Assert(o,msg) hasOption(o) ? true : (!setErrorText(msg))
#define SEPPEL WINDOWS ? ';' : ':'


#define search(o) ( hasOption(o) \
    ? ( isSwitch(o) \
        ? isValidArg( switchIdx( o,indexOfName(rawNext(o)) ) ) \
        : isValidArg( getName(o) ) ) \
    : false )

#define exists(o) ( (!search(o)) \
    ? ( isValidArg( switchIdx( o,indexOfName(nextRaw()) ) ) \
        ? true : ( !setErrorText( setTempf( "missing '%c' parameter!", (char*)(ptval)o ) ) ) ) \
    : true )

#define ensure( o, msg ) ( (!search(o)) \
    ? ( isValidArg( switchIdx( o, indexOfName(nextRaw()) ) ) \
        ? true : ( !setErrorText( msg ) ) ) \
    : true )

#define replace(s,a,b) for( int find=0; s[find]!='\0'; s[find++]=s[find]==a?b:s[find] )

#ifdef __TINYC__
#define MakeArray(typ,nam,len) typ nam ## _data[len]; typ* nam = &nam ## _data[0];
#define CleanArray(nam) nam = NULL;
#else
#define MakeArray(typ,nam,len) typ* nam = (typ*)malloc( sizeof(typ) * (len) );
#define CleanArray(nam) free( nam );
#endif

#ifdef  DEBUG
 #if    DEBUG
#define ENABLE
 #endif
#endif
#ifdef  ENABLE
#define DEBUGLOG(msg) printf("%s(): %s\n", __FUNCTION__, msg);
#define DEBUGFMT(fmt,arg) printf("%s(): " fmt "\n", __FUNCTION__, arg);
#undef  ENABLE
#else
#define DEBUGLOG(msg)
#define DEBUGFMT(fmt,arg)
#endif


#define USAGE(commandName) show_ ## commandName ## _usage()
#define DOKU(commandName) DocumentationFor_ ## commandName

#define CreateDoku(command,dokumentation) const char* \
    DocumentationFor_ ## command = { dokumentation }

#define Synopsis(parameterUsage) printf( \
    "\nUsage:\n\n  %s %s\n\n", \
    getNameOfTheCommander(), \
    parameterUsage \
)
#define UsageDefinition(command,synopsis) int \
show_ ## command ## _usage() { \
    if(hasOption('v')) showOptions(); \
    Synopsis(synopsis); \
    printf( DocumentationFor_ ## command ); \
    return (CheckForError()||(!hasOption('h'))); \
}

#define ERROR_CODE(comState) (*(int*)&comState->CommanderError[0])
#define ERROR_NAME(comState) (&comState->CommanderError[4])
#define COMMANDER_BUFFER ((MAX_NUM_GUM+1)*MAX_NAM_LEN+1)


#endif
