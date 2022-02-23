/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      byteOrder.cc                               ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef using_commandLiner
#ifdef byteOrder
#include ".environMentor.h"
#else
#include ".commandLiner.h"
#endif
#endif
#ifndef _OnTheFly_
 #include ".CheckForProjectSettings.h"
 #include ".byteOrder.h"
#else
#ifdef byteOrder
 int USAGE(byteOrder) {
    Assert('h',"At least one argument must be given!\n");
    printf("\nHelper-tool for byte order related things like fourCCs, flags and so on.\n");
    Synopsis("[ <-[-i|-v|-c]-value> [-r|-x] | <--reverseData \"string\"> ]");
    printf("  --reverse <data> - will reverse string <data> and output it. \n\
  --i-<" WORD_TYPENAME ">      - will reverse the given integer...\n\
  --v-<" WORD_TYPENAME ">      - will reinterprete a given integer value being a string.\n\
  --c-<char*>      - will display the " WORD_TYPENAME " value of a string (but not reversed!)\n\
    -r             - will reverse during conversion when using --c or --v.\n" );
    printf("    -x,-X          - assume input numbers (and output) as hexadecimal notatation\n");
    printf("  --32             - allways proceed 32bit words (even on 64bit machines)\n\
  --64             - allways preceed 64bit words (even on 32bit machines)\n\
  *when bitness parameter (--64/--32) is ommitted all reorder operations will\n\
   be performed assuming data blocks at local machines actual wordsize\n\n");
    return (CheckForError());
 }
#endif
#endif


const static int SIZE_OFFSET = sizeof(WORD_SIZETYPE) - 1;
int IS_BIG_ENDIAN(void) { return ENDIAN_IS_BIG___; }


typedef struct Stringo {
    union {
    fourCC   value;
    byte     bingo[5];
};} Stringo;
Stringo stringo = {
     (fourCC)0
};
const char* byteOrder_fourCCtoString(fourCC intVal)
{
    ((Stringo*)&stringo)->value = intVal;
    return (const char*)&stringo.bingo[0];
}

typedef struct Strongo {
    union {
    longCC   value;
    fourCC   bingo[2];
    byte     bongo[9];
};} Strongo;
Strongo strongo = {
    { '\0' }
};
const char* byteOrder_longCCtoString( longCC longVal )
{
    ((Strongo*)&strongo)->value = longVal;
    return (const char*)&strongo.bongo[0];
}

WORD_SIZETYPE byteOrder_reverse( WORD_SIZETYPE input )
{
    byte* a = (byte*)&input;
    byte* b = a+SIZE_OFFSET;
    do{ byte t=*a;*a=*b;*b=t;
    } while( ++a < --b );
    return input;
}

fourCC byteOrder_reverse32( fourCC input )
{
    byte* aaa = (byte*)&input;
    byte* bbb = aaa+3;
    do{ byte ttt = *aaa;
       *aaa = *bbb;
       *bbb = ttt;
    } while( ++aaa < --bbb );
    return input;
}

longCC byteOrder_reverse64( longCC input )
{
    byte* a = (byte*)&input;
    byte* b = a+7;
    do{ byte t=*a;*a=*b;*b=t;
    } while( ++a < --b );
    return input;
}

byte* byteOrder_reverseData32( void* data, unsigned cbData )
{
    const fourCC* end = (fourCC*)data + cbData/4;
    for( fourCC* dat = (fourCC*)data; dat != end; ++dat )
        *dat=byteOrder_reverse32(*dat);
    return (byte*)data;
}
byte* byteOrder_reverseData64( void* data, unsigned cbData )
{
    const longCC* end = (longCC*)data + cbData/8;
    for( longCC* dat = (longCC*)data; dat != end; ++dat )
        *dat=byteOrder_reverse64(*dat);
    return (byte*)data;
}
byte* byteOrder_reverseData( void* data, unsigned cbData )
{
    const WORD_SIZETYPE* end = (WORD_SIZETYPE*)data + cbData/WORD_BYTESIZE;
    for( WORD_SIZETYPE* dat = (WORD_SIZETYPE*)data; dat < end; ++dat )
        *dat=byteOrder_reverse(*dat);
    return (byte*)data;
}

char* byteOrder_resverseString( char* inputString )
{
    return (char*)byteOrder_reverseData( (void*)inputString, (uint)strlen(inputString) );
}
char* byteOrder_resverseString32( char* inputString )
{
    return (char*)byteOrder_reverseData32( (void*)inputString, (uint)strlen(inputString) );
}
char* byteOrder_resverseString64( char* inputString )
{
    return (char*)byteOrder_reverseData64( (void*)inputString, (uint)strlen(inputString) );
}

ulong reverseBy( int wordsize, void* value ) {
    switch(wordsize) {
        case 32: return (ulong)byteOrder_reverse32(*(fourCC*)value);
        case 64: return (ulong)byteOrder_reverse64(*(longCC*)value);
        default: return (ulong)byteOrder_reverse(*(WORD_SIZETYPE*)value);
    }
}


#ifdef byteOrder
#include ".hexString.h"
#include <stdio.h>
#include <string.h>
int main(int argc,char**argv)
{
    char noArgum = !InitCommandLiner(argc,argv);
    bool REVERSE = false;
    char HEXOGEN = false;

    if ( isSwitch('x') | isSwitch('X') ) {
        HEXOGEN = 'X';
    }
    if ( hasOption('c') ) {
        REVERSE = hasOption('r');
    } else {
        REVERSE = ( hasOption('r') && (!HEXOGEN) );
    }

    if( hasOption('r') ) {
        if( !isSwitch('r') ) {
            if( isModus("reverse") ) {
                if( !isValidArg( setOption('d',rawNext('r')) ) )
                    setErrorText( "missing '--reverse <string>'" );
            } else {
                setErrorText( setTempf( "unknown modus '--%s'", getName('r') ) );
            }
        } else {
            if ( !hasOption('c') ) {
                REVERSE = !HEXOGEN;
            }
        }
    } setOption( 'r', REVERSE ? " reversed" : NoString );

    if( wasError() || hasOption('h') || noArgum ) {
        printf( "\nwords size: %i byte\n", sizeof(SIZECC) );
        printf( "byte order: %s\n", BYTE_ORDER_NAME );
        exit( noArgum ? EXIT_SUCCESS : USAGE( byteOrder ) );
    }

    int mode;
    byte* (*reverseDataFunction)( void*, unsigned );
    if (isModus("32")) { mode = 32;
        reverseDataFunction = &byteOrder_reverseData32;
    } else
    if (isModus("64")) { mode = 64;
        reverseDataFunction = &byteOrder_reverseData64;
    } else { mode = 0;
        reverseDataFunction = &byteOrder_reverseData;
    }

    if( search('d') ) {
        printf("%s\n", reverseDataFunction(
                                getName('d'),
                         strlen(getName('d'))) );
                         exit( CheckForError() ); }

    word hex = 0;
    if( search('i') ) {
        if( hex = (*(word*)&getName('i')[0] == *(word*)"0x") ? 'X' : HEXOGEN )
            setOption( (HEXOGEN='X'), getName('i')+2 );
        else if ( hex )
            setOption( 'X', getName('i') );
        else hex = 0;
        if ( hex ) setOption( 'i', hexString_toNum( getName('X') ) );
        else hex = 'i';
        ulong inp = atoll( getName('i') );
        if ( REVERSE ) inp = reverseBy( mode, &inp );
        printf( "\nwill reverse input %s:\n %i\n", getName(hex), inp );
    } else
    if( search('c') ) { hex = 10;
        char* ccstr = (char*)getName('c');
        int p = 0, o = 0, P;
        while ( ccstr[P=p+o] ) {
            if( ccstr[P] == '\\' && ccstr[P+1] == '0' ) {
                ccstr[p] = '\0'; ++o;
            } else {
                ccstr[p] = ccstr[P];
            } ++p;
        } while ( p < WORD_BYTESIZE ) ccstr[p++] = '\0';
        ulong value;
        switch( mode ) {
            case 32: value = byteOrder_stringTOfourCC( ccstr ); break;
            case 64: value = byteOrder_stringTOlongCC( ccstr ); break;
            default: value = byteOrder_stringTOsizeCC( ccstr ); break;
        } value = REVERSE ? reverseBy( mode, &value) : value;
        if ( HEXOGEN ) hex = 16;
        ulltoa( value, getTemp(), hex );
        const int l = mode ? mode/8 : WORD_BYTESIZE;
        for( int i=0; i<l; ++i ) ccstr[i] = ( ccstr[i] ? ccstr[i] : ' ');
        ccstr[l] = '\0';
        printf( "\nthe%s %s value of string: %s\n %s\n", getName('r'),
                mode==64? "QWORD" : mode==32? "DWORD" : WORD_TYPENAME,
                ccstr, getTemp() );
    } else
    if( search('v') ) {
        int len = hex = 0;
        const int l = mode ? mode/8 : WORD_BYTESIZE;
        if( hex = stringCompare( getName('v'), "0x" ) > 2 ? 'x' : hex ) {
            len = strlen( setOption( 'x', getName('v')+2 ) );
        } else
        if( hex = (HEXOGEN) ? 'x' : hex ) {
            len = strlen( setOption( 'x', getName('v') ) );
        } else hex = 'v';
        if ( hex == 'x' ) {
            if ( len < l  ) { pool_setc('0',l-len);
                 len = 2; } else len = 1;
            pool_set( getName('x') );
            setOption('v', hexString_toNum( setOption('x', pool_merge(len)) ) );
        hex = 'v'; }
        ulong value = atoll( getName('v') );
        if ( mode == 32 ) value = (uint)value;
        if( REVERSE ) value = reverseBy( mode, &value );
        char* outputstring;
        switch ( mode ) {
            case 32: outputstring = (char*)byteOrder_fourCCtoString( (fourCC)value );
                     setTempf( "%i", value ); break;
            case 64: outputstring = (char*)byteOrder_longCCtoString( (longCC)value );
                     setTemp( getName(hex) ); break;
            default: outputstring = (char*)byteOrder_sizeCCtoString( (SIZECC)value );
                     setTemp( getName(hex) ); break;
        } for ( int i = 0; i < l; ++i )
            outputstring[i] = ( outputstring[i] ? outputstring[i] : ' ' );
        outputstring[l] = '\0';
        printf( "\n%s %s %s reinterpretet as string:\n %s\n", getName('r'),
                mode==64 ? "QWORD" : mode==32 ? "DWORD" : WORD_TYPENAME,
                getTemp(), outputstring );
    }
    exit( CheckForError() );
}//eof
#endif