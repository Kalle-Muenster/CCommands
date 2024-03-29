/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      tokken.cc                                  ||
||     Author:    Kalle                                      ||
||     Generated: 26.05.2020                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include <.stringPool.h>
  #include <.junkYard.h>
  #include "eszentielle/.CommandlinerTypes.h"
  #include ".tokken.h"
#endif

#include <stdlib.h>
#include <.byteOrder.h>
#include <.hexString.h>
#include <.base64.h>

#ifndef HEXSTRING_UPPERCASE
#define HEXSTRING_UPPERCASE (1)
#endif

  int USAGE(tokken)
  {
  Synopsis("--<h|b|3>-<size[,count]> [options]");
    printf("Options:\n\n\
  --h-<size[,count]> :  created tokken consisting from <size> count on hex digits\n\
  --3-<size[,count]> :  created tokken consisting from <size> count on base32 digits\n\
  --b-<size[,count]> :  created tokken consisting from <size> count on base64 digits\n\
                     :  ( adding [,count] creates a big bunch of <count> tokkens at once! )\n\
  --g-<groups>       :  make tokken consisting from several, separated groups.\n\
                         '--g-8-16-8' - xxxxxxxx-xxxxxxxxxxxxxxxx-xxxxxxxx\n\
                         '--g-6.10.16' - xxxxxx.xxxxxxxxxx.xxxxxxxxxxxxxxxx\n\
  --u-<count>        :  create <count> on tokkens which pattern matches uuid values (e.g. short for\n\
                         'tokken --h-32,<count> --g-8-4-4-4-12'\n\
  --o-<filename>     :  write output into a file instead of generating stdout\n\
  -v,--verbose       :  output more detailed creation info\n\
  -?,--help          :  display this help message screen\n\n");
#ifdef using_environMentor
   printf("Persists:\n\n\
  HEXSTRING_UPPERCASE (1/0) : Controls character caseing of generated hexdigits\n\n");
#endif
    return CheckForError()
        || !hasOption('h');
  }

#define BASE32_UPPER_DIGITS "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567="
#define BASE32_LOWER_DIGITS "abvdefghijjlmnopqrstuvwxyz234567="

const char* tokken_createHex( int size )
{ pool_scope

    MakeArray( char, outbuf, size+1 )
    int p = 0;
    outbuf[size]='\0';
    char* out = &outbuf[0];
    while( size > p ) { int left = size-p; int get = left<16 ? left/2 : 8;
        slong numb = rand() | ((slong)rand() << 16) | ((slong)rand() << 32) | ((slong)rand() << 48);
        p += hexString_toHex( (const byte*)&numb, get, out, left );
        out += 16;
    } char* created = pool_set( &outbuf[0] );
    CleanArray( outbuf )
    return created;
}

const char* tokken_createB64( int size )
{ pool_scope

    MakeArray( char, out, size+1 )
    int p = 0;
    out[size]='\0';
    const char* b64chars = base64_b64Table();
    while( size > p ) { int i = p; p += 12;
        ulong numb = rand() | ((ulong)rand() << 16) | ((ulong)rand() << 32) | ((ulong)rand() << 48);
        for(;i<p&&i<size;++i) {
            out[i] = b64chars[numb&63];
        numb >>= 6; };
    } const char* creation = pool_set( &out[0] );
    CleanArray( out )
    return creation;
}

const char* tokken_createB32( int size )
{ pool_scope

    MakeArray( char, out, size+1 )
    int p = 0;
    out[size]='\0';
    const char* b32chars = BASE32_UPPER_DIGITS;
    while( size > p ) { int i = p; p += 16;
        ulong numb = rand() | ((ulong)rand() << 16) | ((ulong)rand() << 32) | ((ulong)rand() << 48);
        for(;i<p&&i<size;++i) {
            out[i] = b32chars[numb&31];
        numb >>= 4; };
    } const char* creation = pool_set( &out[0] );
    CleanArray( out )
    return creation;
}

tokken_Generator* tokken_define( tokken_CharSet forMode, int size, const char* groupString )
{
    char groupSep = 0;
    int groupCount = 0;
    const int generatorsize = sizeof(tokken_Generator);
    tokken_Generator* arg;

    if ( isEmptyArg(groupString) ) {
        arg = (tokken_Generator*)junk_allocateJunkChunk( generatorsize );
        arg->grouping = NULL;
        arg->charset = forMode;
        arg->size = size;
        arg->create = &tokken_create;
        return arg;
    }
    MakeArray( char, groupArg, 2*strlen( groupString ) )
    char* grparg = strcpy( groupArg, groupString );
    while( *++grparg ) {
        if( (*grparg>='9') || (*grparg<='0') ) {
            groupSep = *grparg; break;
        }
    } char seppl[3] = { groupSep, 0, 0 };
    grparg = groupArg;
    groupCount = toSplitList( groupArg, &seppl );

    arg = (tokken_Generator*)junk_allocateJunkChunk( ++groupCount + generatorsize + 1 );
    byte* groups = (byte*)arg + generatorsize;
    groups[0] = (byte)groupSep;
    arg->size = groups[groupCount] = 0;
    for( int i = 1; i < groupCount; ++i ) {
        arg->size += (groups[i] = (byte)atoi( grparg ));
        while( *grparg++ );
    } arg->grouping = groups;
    arg->create = &tokken_create;
    arg->charset = forMode;
    CleanArray( groupArg )
    return arg;
}

const char* tokken_groupedHex( int size, const char* grouping )
{ pool_scope

    int p = 0;
    while( grouping[p++] );
    int groups = p;
    MakeArray( byte, grpbuf, p )
    byte* group = grpbuf; p = 0;
    while( p < groups ) {
        group[p] = grouping[p];
        ++p;
    } p = 0;
    --groups;
    char d[3];
    d[0] = group ? *(group++) : 0;
    d[1] = size;
    d[2] = 0;
    if(!group) group = (byte*)&d[1];
    MakeArray( char, outbuf, size + groups )
    outbuf[(size+groups)-1]='\0';
    char* out = &outbuf[0];
    while( size > p ) { int left = size-p; int get = left<16 ? left/2 : 8;
        slong numb = rand() | ((slong)rand() << 16) | ((slong)rand() << 32) | ((slong)rand() << 48);
        p += hexString_toHex( (const byte*)&numb, get, out, left );
        out += 16;
    } char* created = pool_setc( '\0', size + groups );
    int i = 0, o = 0; *group+=1;
    while ( outbuf[i] ) {
        if( !(--*group) ) { ++group;
            created[i + o++] = d[0];
        } created[o + i] = outbuf[i];
    ++i; }
    CleanArray( outbuf )
    CleanArray( grpbuf )
    return created;
}

const char* tokken_groupedB64( int size, const char* grouping )
{ pool_scope

    int p = 0;
    while ( grouping[p++] );
    int groups = p;
    MakeArray( byte, grpbuf, p )
    byte* group = grpbuf; p = 0;
    while (p < groups) {
        group[p] = grouping[p];
        ++p;
    } p = 0;
    --groups;
    char d[3];
    d[0] =  group ? *(group++) : 0;
    d[1] = size;
    d[2] = 0;
    int o = 0;
    if(!group) group = (byte*)&d[1];
    MakeArray( char, outbuf, size + groups )
    outbuf[(size+groups)-1]='\0';
    const char* b64chars = base64_b64Table();
    while( size > p ) { int i = p; p += 12;
        ulong numb = rand() | ((ulong)rand() << 16) | ((ulong)rand() << 32) | ((ulong)rand() << 48);
        for( ; i<p && i<size; ++i ) {
            outbuf[i+o] = b64chars[numb&63];
            if( !(--*group) ) { ++group;
                outbuf[i+(++o)] = d[0];
            } numb >>= 6; };
    } outbuf[(size+groups)-2] = '\0';
    const char* zuruck = pool_set( &outbuf[0] );
    CleanArray( outbuf )
    CleanArray( grpbuf )
    return zuruck;
}

const char* tokken_groupedB32( int size, const char* grouping )
{ pool_scope

    int p = 0;
    while ( grouping[p++] );
    int groups = p;
    MakeArray( byte, grpbuf, p )
    byte* group = grpbuf; p = 0;
    while (p < groups) {
        group[p] = grouping[p];
        ++p;
    } p = 0;
    --groups;
    char d[3];
    d[0] =  group ? *(group++) : 0;
    d[1] = size;
    d[2] = 0;
    int o = 0;
    if(!group) group = (byte*)&d[1];
    MakeArray( char, outbuf, size + groups )
    outbuf[(size+groups)-1]='\0';
    const char* b32chars = BASE32_UPPER_DIGITS;
    while( size > p ) { int i = p; p += 16;
        ulong numb = rand() | ((ulong)rand() << 16) | ((ulong)rand() << 32) | ((ulong)rand() << 48);
        for( ; i<p && i<size; ++i ) {
            outbuf[i+o] = b32chars[numb&31];
            if( !(--*group) ) { ++group;
                outbuf[i+(++o)] = d[0];
            } numb >>= 4; };
    } outbuf[(size+groups)-2] = '\0';
    const char* zuruck = pool_set( &outbuf[0] );
    CleanArray( outbuf )
    CleanArray( grpbuf )
    return zuruck;
}

const char* tokken_create( const tokken_Generator* mode )
{
    if( mode->grouping ) {
        switch ( mode->charset ) {
            case tokken_HEX: return tokken_groupedHex( mode->size, mode->grouping );
            case tokken_B32: return tokken_groupedB32( mode->size, mode->grouping );
            case tokken_B64: return tokken_groupedB64( mode->size, mode->grouping );
        }
    } else {
        switch ( mode->charset ) {
            case tokken_HEX: return tokken_createHex( mode->size );
            case tokken_B32: return tokken_createB32( mode->size );
            case tokken_B64: return tokken_createB64( mode->size );
        }
    } setErrorText( "unknown token charset" );
    return NULL;
}

#ifdef tokken
int main(int argc,char**argv)
{
    if( !InitCommandLiner( argc, argv ) ) {
        exit( USAGE(tokken) );
    }
    if (hasOption('?') || isModus("help")) {
        exit( USAGE(tokken) );
    }

    ulong seed = (ptval)(void*)&main;
    pool_InitializeCycle();
    if( isModus("verbose") )
        printf("\nfrom random seed: %llu\n",seed);
    srand( seed );

    if ( search('u') ) {
        setOption('h',pool_setf("32,%s",getName('u')));
        setOption('g',"8-4-4-4-12");
        noOption('u');
    }

    uint count = 1;
    int  size = 0;
    char* sizeArg;
    char* countArg;
    char* groupArg;
    tokken_CharSet charset;

    if ( search('g') ) {
        groupArg = getName('g');
    } else groupArg = NoString;


    if( search('h') ) {
        charset = tokken_HEX;
        countArg = sizeArg = getName('h');
    } else if ( search('b') ) {
        charset = tokken_B64;
        base64_Initialize();
        countArg = sizeArg = getName('b');
    } else if ( search('3') ) {
        charset = tokken_B32;
        countArg = sizeArg = getName('3');
    } else exit( USAGE(tokken) );

    while(*countArg) if( *++countArg == ',' ) {
        *countArg = '\0';
        count = atoi( ++countArg );
        break;
    } size = atoi( sizeArg );

    tokken_Generator* grp = tokken_define( charset, size, groupArg );

    if( isModus( "verbose" ) ) {
        printf( "generating %i token%s",
                count, count > 1 ? "s\n" : "\n" );
        printf( "using '%s' charset\n", hasOption('b')
              ? "B64Table" : hasOption('h')
              ? "HexValue" : "B32Table" );
    } printf("\n");

    FILE* output = stdout;
    if ( search('o') ) {
        output = fopen( getName('o'), "w" );
        if( !output ) setErrorText(
            pool_setf( "cannot open output file %s for writing token", getName('o') )
        );
    } else setOption( 'o', "stdout" );

    ExitOnError( "Parameter" );

    for ( int i = 0; i < count; ++i ) {
        fprintf( output, "%s\n", tokken_create( grp ) );
    } printf("\n");

    if( isModus("verbose") ) {
        printf( "written %i (%ibyte) tokkens to: %s\n\n",
                count, grp->size, getName('o') );
    } exit( CheckForError() );
}

#endif
