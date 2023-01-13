#ifndef _OnTheFly_
 #include ".CheckForProjectSettings.h"
 #if (!defined(NOT_USE_ENVIRONMENTOR)) && defined(__TINYC__)
  #include ".environMentor.h"
 #else
  #include ".commandliner.h"
 #endif
 #include ".base64.h"
#endif

#ifndef BASE64_PUSHED_TABLE
#define BASE64_PUSHED_TABLE "noTableWasPushed"
#endif
#ifndef BASE64_CODING_TABLE
#define BASE64_CODING_TABLE BASE64_DEFAULTTABLE
#endif
#ifndef BASE64_BUFFER_SIZE
#define BASE64_BUFFER_SIZE (1024)
#endif

#ifndef WINDOWS
#define WINDOWS (0)
#endif
#if WINDOWS == true
    #include <conio.h>
    #define getch _getch
#else //if LINUX ?
    #define getch getchar
#endif

#include ".byteOrder.h"
#ifndef BASE64_REVERSED_ORDER
#define BASE64_REVERSED_ORDER (0)
#endif

#if     BASE64_REVERSED_ORDER > 0
#define SixBitIehShift(gitigit) (6*(3-gitigit))
#else
#define SixBitIehShift(ieh) (6*ieh)
#endif

#if BASE64_VERFAHREN == 1
#define BASE64_SELECTED_ALGORYTHM \
        else for( uint c = 0; c < 65; ++c ) {\
            if( state->CodeTable[c] == fourChars.i8[i] ) {\
                bin3.u32 += ( c << SixBitIehShift(i) );\
                c = 65;\
            } if( c == 64 )
#else
 #undef BASE64_VERFAHREN
#define BASE64_VERFAHREN (2)
#define BASE64_SELECTED_ALGORYTHM \
        else { int c = state->derDickeBatzen[fourChars.i8[i]];\
            if ( !(c<0) ) {\
                bin3.u32 += ( c << SixBitIehShift(i) );\
            } else
#endif


#ifndef     BASE64_SPECIAL_CHARACTERS
const char* BASE64_SPECIAL_CHARACTERS = "-_";
#ifndef BASE64_ALLOW_SPECIAL_CHARACTERS
#define BASE64_ALLOW_SPECIAL_CHARACTERS (0)
#endif
#else
#ifndef BASE64_ALLOW_SPECIAL_CHARACTERS
#define BASE64_ALLOW_SPECIAL_CHARACTERS (1)
#endif
#endif

b64State EncoderState = {
    false, true, 0, 0ull, NULL, NULL, { 0 }
#if BASE64_VERFAHREN == 2
    ,{ 0 }
#endif
};
b64State* encoderState = &EncoderState;

#ifdef _OnTheFly_
 #define PREPARE_POOLBOTTOM state->BasePool
 #include ".poolBottom.h"
#else
 StringPool* Pool;
#endif

#ifdef _OnTheFly_
const char* DOKU(base64) = "\
    --d-<input>       : decode: base64 file <input> back to binary data\n\
    --e-<input>       : encode: file <input> to base64.\n\
    --o-<output>      : writes the ouput to file <output> instead to stdout.\n\
     -t               : interpret -d/-e/-c as string-input (rather then filenames).\n\
     -v               : print verbose details to stdout.\n\
     -h               : print this help-message.\n\n";
 #ifdef using_environMentor
  const char* DOKU(base64_persist) = "\
Changing Persists:\n\n\
    --c-<codetable>   : defines own coding table (64 chars + terminating '=' sign!)\n\
    --allow           : allows special character being used within code tables.  \n\
      [on|off|'char']   %s\n\
     -r               : resets the current coding table back to default.\n\
    --w-[std|web]     : select base64 table variant is used as 'default'\n\
                        %s\n\
    --f-[on|off]      : toggles 'use reversed byteorder' setting.\n\
                        %s\n\
    --l-[on|off]      : toggles 'add linebreaks to data' setting.\n\
                        %s\n\
    --b-<buffersize>  : sets new size for internal used buffer\n\
                        %i byte\n\n";
 #endif

int USAGE(base64)
{
    Assert('h', "no arguments where given!");
   #ifdef using_environMentor
    environMentor_CheckAndEnable();
    Synopsis("[options] <[--e/--d]-InputFile> [--o-OutputFile]\n  base64 [persitent options]\n");
    printf( DOKU(base64) );
    printf( DOKU(base64_persist),
        BASE64_ALLOW_SPECIAL_CHARACTERS ? toQuoted(BASE64_SPECIAL_CHARACTERS) : "DISABLED",
        base64_b64Table()[63] == '_' ? "WebSafe" : "Standard",
        BASE64_REVERSED_ORDER ?"ENABLED":"DISABLED",
        BASE64_WITH_LINEBREAKS?"ENABLED":"DISABLED",
        BASE64_BUFFER_SIZE
    );
   #else
    Synopsis("[options] <[--e/--d]-InputFile> [--o-OutputFile]\n");
    printf( DOKU(base64) );
   #endif
    if(isSwitch('v'))
        showOptions();
    CheckForError();
    return getErrorCode();
}
#endif


////////////////////////////////////////////////


const char* base64defaultTable = {
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="
};

const char* base64websafeTable = {
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_="
};

#ifdef using_environMentor ////////////////////////////////////////////
// functions for changing persistent settings using .environMentor.h //
///////////////////////////////////////////////////////////////////////

char isSpecialCharacter(char);
char isAllowedCharacter(char);
char isAllowedSpecialCharacter(char);

const char* base64_popTable(void)
{
    char buffer[MAX_NAM_LEN];
    beginPersistChange(LOCAL);
        sprintf(&buffer[0],"\"%s\"",BASE64_PUSHED_TABLE);
        setPersistEntry("BASE64_CODING_TABLE",&buffer[0]);
        sprintf(&buffer[0],"\"%s\"",BASE64_DEFAULTTABLE);
        setPersistEntry("BASE64_PUSHED_TABLE",&buffer[0]);
    commitPersistChange();
    printf("Table Popped!\n");
    return BASE64_PUSHED_TABLE;
}

const char* base64_pushTable( const char* table )
{
    const char* loaded = getFeatured("BASE64_CODING_TABLE").value.string;
    if( table[0] != '\0' ) {
        char buffer[68];
        sprintf( &buffer[0], "\"%s\"", loaded );
        beginPersistChange(LOCAL);
            setPersistEntry( "BASE64_PUSHED_TABLE", &buffer[0] );
            sprintf( &buffer[0], "\"%s\"", table );
            setPersistEntry( "BASE64_CODING_TABLE", &buffer[0] );
        commitPersistChange();
        printf( "Table Pushed!\n" );
    } return loaded;
}

void base64_setBufferSize( int newSize )
{
    if( newSize > 640000 ) {
        setErrorText( "please don't allocate buffers greater then 640k please" );
    } else {
        beginPersistChange(LOCAL);
            tempf( "(%i)", (const char*)(ptval)newSize );
            setPersistEntry("BASE64_BUFFER_SIZE",getTemp());
            setPersistEntry("CYCLE_SIZE",getTemp());
            int cycle_size = newSize / 64;
            tempf("(%i)",(const char*)(ptval)cycle_size);
            setPersistEntry("CYCLE_COUNT",getTemp());
        commitPersistChange();
        printf("set buffer size to %i byte...\n",newSize);
    }
}

void base64_toggleDefaultTable( const char* stdOrWeb )
{
    if (!stdOrWeb) stdOrWeb = NoString;
    int useWebTable = BASE64_DEFAULTTABLE[63] == '_';
    if((!strcmp(stdOrWeb,"std"))||(!strcmp(stdOrWeb,"web")))
        useWebTable = stdOrWeb[1] == 't';
    // toggle the current setting if NULL was passed
    useWebTable = (useWebTable-1)*(-1);
    if( useWebTable ) base64_allowCharacter( (byte)1 );

    setMacroRaw( "BASE64_DEFAULTTABLE", useWebTable
               ? "base64websafeTable" : "base64defaultTable" );
    printf( "The default base64 table now is: %s\n",
             useWebTable ? "WebSafe" : "Standard" );
}

void base64_toggleReverseByteorder( const char* onOrOff )
{
    FeatureGet revorder = getFeatured( "BASE64_REVERSED_ORDER" );
    if ( !revorder.type.flags ) {
        revorder.type.flags = Type_NUMBER|Type_MACROL;
        revorder.value.number = (long double)BASE64_REVERSED_ORDER;
    }
    int ReverseByteOrderSetting = (int)revorder.value.number;
    if (!onOrOff) onOrOff = NoString;
    if( (!stringCompare(onOrOff,"on")) || (!stringCompare(onOrOff,"off")) )
        ReverseByteOrderSetting = onOrOff[1]=='f';
    // toggle the current setting if NULL was passed
    revorder.value.number = (long double)((ReverseByteOrderSetting-1)*(-1));
    setFeatured( &revorder );
    printf( "Reversing byte order during en/de coding operations: %s\n",
            ReverseByteOrderSetting ? "DISABLED" : "ENABLED" );
}

void base64_toggleWriteLinebrakes( const char* onOrOff )
{
    if (!onOrOff) onOrOff = NoString;
    int EncoderWritesLinebreaks = BASE64_WITH_LINEBREAKS;
    if((!strcmp(onOrOff,"on"))||(!strcmp(onOrOff,"off")))
        EncoderWritesLinebreaks = onOrOff[1]=='f';
    // toggle the current setting if NULL was passed
    setMacroNum("BASE64_WITH_LINEBREAKS",(EncoderWritesLinebreaks-1)*(-1));
    printf( "encoded output contains line break every 64 chars: %s\n",
            EncoderWritesLinebreaks ? "DISABLED" : "ENABLED" );
}

// allow characters to be contained within coding tables:
// passing char argument: add given character to the white list.
// passing true/false: enable or disable to allow all characters
//                     contained in the white list.
void base64_allowCharacter( byte allowed )
{
    if( allowed > 0x01 ) {
        int strings = 3;
        pool_set("\"");
        pool_set( BASE64_SPECIAL_CHARACTERS );
        if (isSpecialCharacter((char)allowed)
        &&(!isAllowedSpecialCharacter((char)allowed)) ) {
            pool_setc( (char)allowed, 1 );
            ++strings;
        } pool_set("\"");
        char* whitelist = pool_merge( strings );
        setMacroStr( "BASE64_SPECIAL_CHARACTERS", whitelist );
        printf( "white listed characters: %s\n", whitelist );
    } else {
        if( allowed ) {
            setMacroNum("BASE64_ALLOW_SPECIAL_CHARACTERS",1);
            setTemp( "white listed characters are ALLOWED\n" );
        } else {
            setMacroNum("BASE64_ALLOW_SPECIAL_CHARACTERS",0);
            setTemp( "white list disabled!\n" );
        } fwrite( getTemp(), 1, strlen(getTemp()), stdout );
    }
}
#else // if not using_environMentor
#define NUMBER_OF_FEATURES (0)
#endif


////////////////////////////////////////////////////////////////////////////
// Internal used functions:

// saves the used coding table beside the output file
// as *_table.dat when not encoding by standard table
const char* saveTableFile( char* b64tableSrc )
{
    if( hasOption('e')
    &&  hasOption('c') ) {
        char buffer[128] = {'\0'};
        sprintf( &buffer[0], "%s_table.dat", getName('e') );
        FILE* b64TableFile = fopen( &buffer[0], "w" );
        fwrite( b64tableSrc, 1, 65, b64TableFile );
        fflush( b64TableFile );
        fclose( b64TableFile );
        if( hasOption('v') )
            printf( "Saved given base64 table to file: %s\n",
                    &buffer[0] );
    } return (const char*)b64tableSrc;
}

const char* loadTableFile( b64State* state, const char* path )
{
    FILE* codeFile = fopen( path, "r" );
    if( codeFile ) {
        fread( &state->codeTableBuffer[0], 1, 65, codeFile );
        fflush( codeFile ); fclose( codeFile );
    } else {
        for( int i = 0; i < 66; ++i )
            state->codeTableBuffer[i] = BASE64_DEFAULTTABLE[i];
    } &state->codeTableBuffer[0];
}

/*
#if  BASE64_VERFAHREN == 2
char DerDickeBatzen[256];  = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,62,-1,-1,-1,63,52,53,
    54,55,56,57,58,59,60,61,-1,-1,
    -1,64,-1,-1,-1, 0, 1, 2, 3, 4,
     5, 6, 7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,23,24,
    25,-1,-1,-1,-1,-1,-1,26,27,28,
    29,30,31,32,33,34,35,36,37,38,
    39,40,41,42,43,44,45,46,47,48,
    49,50,51,-1,-1,-1,-1,-1,

    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1
};
#endif
*/

// decides which base64 table will be used for any following de/en code opperations.
// It gegebenenfalls loads it from 'file' or from 'string' if this maybe was defined
// (by option "c") previously. - when 'string', it will also output a secondary file
// called "*_table.dat", which will contain that table. If no option "c" is defined,
// it just uses the last macro which was pushed by the "base64_pushTable()" function,
// or the default one, when there never was pushed a table before.
const char* loadeCode( b64State* state, const char* from )
{
    if( from == NULL ) {
        if ( state->isExternCall ) {
            from = BASE64_DEFAULTTABLE;
        } else {
            if ( isSwitch('c') )
                base64_NewTable( state, from );
            from = ( hasOption('c') ?
                       getName('c') : NUMBER_OF_FEATURES > 1
                                    ? BASE64_CODING_TABLE
                                    : BASE64_DEFAULTTABLE );
        }
    }

    const char* loade = NULL;
    if( from[64] == '=' ) {
        if( state->isExternCall ) {
            DEBUGLOG( "call from other command!" )
            loade = &from[0];
        } else {
            loade = saveTableFile( &from[0] );
        } memcpy( &state->codeTableBuffer[0], loade, 65 );
        state->codeTableBuffer[65] = '\0';
        loade = &state->codeTableBuffer[0];
    }
    if( !loade ) {
        loade = loadTableFile( state, from );
    }

   #if BASE64_VERFAHREN == 2
    for( int i=0; i<256; ++i )
        state->derDickeBatzen[i] = -1;
    for( int i=0; i<65; ++i )
        state->derDickeBatzen[loade[i]] = i;
   #endif

    state->isTableInitialized = true;
    DEBUGFMT( "using coding table: %s", loade )
    return loade;
}

//checks for valid base64 characters...

// check if char is a defined 'special' character (e.g. means char is NOT a regular base64 digit)
char isSpecialCharacter( char check )
{
    return ( check == '<'  || check == '>' ||
             check == '\\' || check < 33   || check > 126 )
           ? check
           : false;
}

// check if char is 'special' AND if that 'spacial' also is whitelisted
char isAllowedSpecialCharacter( char special )
{
    char* chars = ((char*)&BASE64_SPECIAL_CHARACTERS[0]-1);
    while(*++chars) if( *chars == special ) return special;
    return false;
}

// returns true if char is a base64 digit or a whitelisted special character
// returns false if char is a not whitelisted and not base64 digit character
char isAllowedCharacter( char check )
{
    char special = isSpecialCharacter( check );
    if ( special )
        return isAllowedSpecialCharacter( special );
    else
        return check;
}



/////////////////////////////////////////////////////////////////////////
// Prefixed functions to be used by other sources which include .base64.h

b64Frame base64_EncodeFrame( b64State* state, b64Frame frame )
{
   #if BASE64_REVERSED_ORDER == 0
    frame.u8[3] = 0u;
   #endif
    uint threeByte = frame.u32;
    frame.u32 = 0u;
   #if BASE64_REVERSED_ORDER == 1
    threeByte = byteOrder_reverse32( threeByte << 8 );
    for( uint i=3; i<EMPTY; --i ) {
   #else
    for( uint i=0; i<4; ++i ) {
   #endif
        frame.u8[i] = state->CodeTable[threeByte%64];
        threeByte >>= 6;
    } return frame;
}

b64Frame base64_DecodeFrame( b64State* state, b64Frame fourChars )
{
    b64Frame bin3;
    bin3.u32 = 0u;
    for( int i = 0; i < 4; ++i ) {
        if( fourChars.i8[i] == '=' ) {
            bin3.u32 += ( 64 << SixBitIehShift(i) );
            bin3.i8[3] = (char)(4-i); break;
        } BASE64_SELECTED_ALGORYTHM {
            bin3.i8[3] = (char)setError(
                "invalid base64 data", 4-i );
            i = 4; }
        }
    }
   #if BASE64_REVERSED_ORDER == 1
    byte ctrl = bin3.u8[3];
    bin3.u32 = byteOrder_reverse32( bin3.u32 ) >> 8;
    bin3.u8[3] = ctrl;
   #endif
    return bin3;
}



// encodes one frame (first 3 byte of 'frame' to 4 chars base64-data)
b64Frame base64_encodeFrame( b64Frame frame )
{
    return base64_EncodeFrame( encoderState, frame );
}

// encodes last frame (enodes first 3 byte of 'frame' to 4 chars base64-data)
// and exchanges trailing 'A's in the output against '=' termination signs.
b64Frame base64_encEndFrame( b64State* ste, b64Frame frm )
{
    byte mod = frm.u8[3];
    frm = base64_EncodeFrame( ste, frm );
    switch ( mod ) {
    case 0: frm.u32 = 1027423549u; break;
    case 1: frm.i8[1] = frm.i8[1] == 'A' ? '=' : frm.i8[1];
    case 2: frm.i8[2] = frm.i8[2] == 'A' ? '=' : frm.i8[2];
            frm.i8[3] = frm.i8[3] == 'A' ? '=' : frm.i8[3];
    }return frm;
}

// decode one frame (4 chars base64-data to 3 byte binary data)
b64Frame base64_decodeFrame( b64Frame fourChars )
{
    return base64_DecodeFrame( encoderState, fourChars );
}



#if BASE64_WITH_LINEBREAKS == 1
#define IncrementAndLineBreak if( ((lO+(iD+=4)) % 64) == 0 ) dst[ iD + lB++ ] = '\n'
#else
#define IncrementAndLineBreak iD += 4
#endif

// encode data batzen src to base64 string dst... (returns encoded size)
int base64_EncodeData( b64State* state, char* dst, const byte* src, uint cbSrc, uint lO )
{
    uint lB = 0;
    uint iD = 0;
    uint iM = cbSrc % 3;
    iM = iM ? iM : 3;
    uint iS = cbSrc - iM;

    // make a 3byte padded copy of the maybe less then 3 byte sized last
    // frame of input data. used as input for encoding the endframe after
    // encoding input data without it's last, truncated frame. (which in
    // case passed buffer not ends on a 3byte boundary would cause error)
    b64Frame end = {0};
    while ( iS < cbSrc ) {
        end.u8[lB++] = src[iS++];
    } cbSrc -= iM;
    end.u8[3] = (byte)iM;
    iS = lB = 0;

    do { asFrame( &dst[lB+iD] ) = base64_EncodeFrame( state, asFrame( &src[iS] ) );
       IncrementAndLineBreak;
    } while ( (iS+=3) < cbSrc );

    asFrame( &dst[iD+lB] ) = base64_encEndFrame( state, end );
    IncrementAndLineBreak;
    dst[iD+=lB] = '\0';
    return (int)iD;
}
#undef IncrementAndLineBreak

int base64_encodeData( char* dst, const byte* src, uint cbSrc, uint lO )
{
    return base64_EncodeData( encoderState, dst, src, cbSrc, lO );
}


// decode base64 string 'src' to byte data 'dst'... (returns decoded size)
int base64_DecodeData( b64State* state, byte* dst, const char* src, uint cbSrc )
{
    uint iS = 0u;
    uint iD = 0u;
    cbSrc = cbSrc ? cbSrc - 4 : EMPTY;

#if BASE64_WITH_LINEBREAKS == 1
    b64Frame copy;
    do { if ( src[iS] == '\n' ) ++iS;
        copy = base64_DecodeFrame( state, asFrame( &src[iS] ) );
        asFrame( &dst[iD] ) = copy;
        iD += 3;
    } while ( (copy.i8[3] == 0 ) && ( ( iS += 4 ) <= cbSrc ) );
#else
    while ( ( asFrame( &dst[iD] ) = base64_DecodeFrame( state, asFrame( &src[iS] ) )
        ).i8[3] == 0 ) {
        iD += 3;
        if( (iS += 4) > cbSrc )
            break;
    }
#endif
    return (int)iD;
}

int base64_decodeData( byte* dst, const char* src, uint cbSrc )
{
    return base64_DecodeData( encoderState, dst, src, cbSrc );
}


// encodes contents of file 'fileName' into the pool (returns encoded size)
int
base64_encodeFromFile( const char* fileName, int* out_len )
{
    setOption('e', fileName);
    FILE* f = fopen(fileName,"rb");
    fseek(f,0,SEEK_END);
    const uint fileSize = ftell(f);
    fseek(f,0,SEEK_SET);
    if ( isModus("verbose") ) {
        printf("file %s opened... filesize is %i\n",fileName,fileSize);
    }
    pool_setCheckpoint();
    b64Frame frame = {0};
    uint pos = fileSize;
    DEBUGFMT("fileSize: %i",fileSize)

    int outPosition = 0;
    while( pos > 2 ) {
        fread( &frame.u32, 1, 3, f );
        frame = base64_EncodeFrame( encoderState, frame );
        pool_setb( (void*)&frame.u32, 4 );
        pos -= 3;
        outPosition += 4;
   #if BASE64_WITH_LINEBREAKS > 0
        if( !(outPosition%64) ) {
            pool_setc('\n',1);
            outPosition+=1; }
   #endif
    } frame.u32 = 0;
    if( pos ) {
        fread( &frame.u8[0], 1, pos, f );
    } frame = base64_EncodeFrame( encoderState, frame );
    pool_setb( (void*)&frame.u8[0], 4 );
    pool_setc( '=', 1 );
    fflush(f); fclose(f);
    *out_len = outPosition+6;
    return *out_len;
}

// decode content of file 'fileName' into the pool. (returns decoded size)
int
base64_decodeFromFile( const char* fileName, int* out_len )
{
    setOption('d', fileName);
    FILE* f = fopen(fileName,"rb");
    fseek(f,0,SEEK_END);
    const uint fileSize = ftell(f);
    fseek(f,0,SEEK_SET);
    DEBUGFMT("fileSize: %i",fileSize)

    pool_setCheckpoint();
    b64Frame frame = {0};
    uint pos = fileSize;
    char readbuffer[MAX_NAM_LEN+1] = {0};
    int outPosition = 0;
    while( pos > 3 ) {
        int linebreak=0;
        pos -= fscanf( f, "%s\n", &readbuffer[0] );
        char* r = &readbuffer[0];
        int p = 0;
        while( *r ) { ++p;
            if(*r==10||*r==13) {
                *r = '\0';
            } else {
                ++r;
            }
        } b64Frame* rf = (b64Frame*)r;
        while(p) {
            uint binR = base64_DecodeFrame( encoderState, *rf ).u32;
            pool_setb( (void*)&binR, 3 );
            outPosition += 3;
            p-=4; ++rf;
        }
    } frame.i8[0] = frame.i8[1] = frame.i8[2] = frame.i8[3] = '=';
    if( pos > 0 ) {
        fread( &frame.i8[0], 1, pos, f );
    } frame = base64_DecodeFrame( encoderState, frame );
    pool_setb( (void*)&frame.u32, 3 );
    outPosition += 3;
    fflush(f); fclose(f);
    DEBUGFMT( "outputSize: %i", *out_len = outPosition )
    return *out_len;
}

// encode content of file dst to file src
ptdif base64_encodeFileToFile( FILE* dst, FILE* src, byte* buf, ptval siz )
{
    long cpos = ftell(src);
    fseek(src,0,SEEK_END);
    ptval size = ftell(src)-cpos;
    fseek(src,cpos,SEEK_SET);

    if( siz && buf ) {
        cpos = 0;
        const ptval chunks = (uint)( ((double)siz/7.0) * (64.0/65.0) - 1.0 );
        const ptval dstSiz = chunks * 4;
        const ptval srcSiz = chunks * 3;
        byte* srcBuf = buf;
        byte* dstBuf = buf + (chunks+1)*3;
        do{ siz = srcSiz < size ? srcSiz : size;
            siz = fread( srcBuf, 1, siz, src );
            size -= siz;
            int dec = base64_encodeData( (char*)dstBuf, srcBuf, (uint)siz, cpos % 65 );
            cpos += (long)fwrite( dstBuf, 1, dec, dst );
        } while ( siz == srcSiz );
        siz = cpos;
    } else {
        b64Frame dat;
        dat.u32 = 0;
        cpos = 0;
        int lb = 0;
        while (size > 2) {
            size -= (int)fread( &dat, 1, 3, src );
            dat = base64_EncodeFrame( encoderState, dat );
            cpos += (int)fwrite( &dat, 1, 4, dst );
#if BASE64_WITH_LINEBREAKS == 1
            if (cpos % 64 == 0) lb += (int)fwrite( "\n", 1, 1, dst );
#endif
        } if (size) {
            dat.u32 = 0;
            fread( &dat, 1, size, src );
            dat = base64_EncodeFrame( encoderState, dat );
            if( size == 1 ) {
                fwrite( &dat.u8[0], 1, 3, dst );
                fwrite( "=", 1, 1, dst );
            } else {
                fwrite( &dat.u8[0], 1, 4, dst );
                dat.u32 = *(uint*)"====";
#if BASE64_REVERSED_ORDER == 0
                dat.i8[0] = encoderState->CodeTable[0];
#endif
                fwrite( &dat.u8[0], 1, 4, dst );
                lb += 4;
            }
        } else {
            dat.u32 = *(uint*)"====";
#if BASE64_REVERSED_ORDER == 0
            dat.i8[0] = encoderState->CodeTable[0];
#endif
            fwrite( &dat.u8[0], 1, 4, dst );
        } siz = cpos + lb + 4;
    } return siz;
}

// decode base64 content from file src to file dst
ptdif base64_decodeFileToFile( FILE* dst, FILE* src, byte* buf, ptval siz )
{
    long cpos = 0;
    ptval size = 0;
    if ( buf && siz )
    {
        cpos = ftell(src);
        fseek(src, 0, SEEK_END);
        size = ftell(src) - cpos;
        fseek(src, cpos, SEEK_SET);
        cpos = 0;

        const ptval chunks = (ptval)(((double)siz / 7.0)*(64.0 / 65.0) - 1.0);
        const ptval sizSrc = chunks * 4;
        const ptval srcLos = (chunks + 1) * 3;
        byte* bufDst = buf;
        byte* bufSrc = buf + srcLos;
        do{ siz = sizSrc < size ? sizSrc : size;
            ptval enc = 0;
#if BASE64_WITH_LINEBREAKS == 1
            int lb = 0;
            do{ if (fread(bufSrc, 1, 1, src) == 0) {
                    siz = enc; size = enc + lb;
                } else {
                    if (*bufSrc != '\n') {
                        ++bufSrc; ++enc;
                    } else ++lb;
                }
            } while (enc < siz);
            size -= (enc + lb);
            bufSrc = buf + srcLos;
#else
            siz = fread( bufSrc, 1, siz, src );
            size -= siz;
#endif
            bufSrc[siz] = 0;
            enc = base64_decodeData( bufDst, (const char*)bufSrc, (uint)siz );
            cpos += (long)fwrite( bufDst, 1, enc, dst );
        } while( siz == sizSrc );
    } else {
        char rbuf[5] = { 0,0,0,0,0 };
        int in = 4;
        char* bu = &rbuf[0];
        while (in == 4) {
            in = 0;
            int end = false;
            while (in < 4) {
                if (fread(&rbuf[in], 1, 1, src) == 0) {
                    end = true; break;
                } char r = rbuf[in];
                in += (r != 10 && r != 13);
            } if (in == 0) { return size; }
            else if (in < 4) {
                for (int i = 4; i > in; )
                    rbuf[--i] = '=';
            } b64Frame f = base64_DecodeFrame( encoderState, asFrame(bu) );
            if (f.i8[3] || end ) {
#if DEBUG
                printf("EOS-frame: control byte is: %i\n", f.i8[3]);
#endif
                if (f.i8[3] < 4)
                    size += (int)fwrite(&f.i8[0], 1, 3 - f.u8[3], dst);
                return size;
            } else size += (int)fwrite(&f.i8[0], 1, 3, dst);
        } cpos = (long)size;
    }
    return cpos;
}

// encode binary content of file 'src' to base64 data into file 'dst'
int base64_encodeFile( const char* dst_nam, const char* src_nam, byte* buf, uint siz )
{
    FILE* src = fopen( src_nam, "rb" );
    FILE* dst = fopen( dst_nam, "w" );
    int outlen = (int)base64_encodeFileToFile( dst, src, buf, siz );
    fflush(dst);
    fclose(dst);
    fclose(src);
    return wasError() ? 0 : outlen;
}

// decodes content of file 'src' to file 'dst'
int base64_decodeFile( const char* dst_nam, const char* src_nam, byte* buf, uint siz )
{
    FILE* src = fopen( src_nam, "r" );
    FILE* dst = fopen( dst_nam, "wb" );
    int outlen = (int)base64_decodeFileToFile( dst, src, buf, siz );
    fflush(dst);
    fclose(dst);
    fclose(src);
    return wasError() ? 0 : outlen;
}

const char* base64_encode( const byte* data, uint* size )
{
    if( !size ) return NoString;
    if( *size == 0 || *size == EMPTY ) {
        *size = 0; return NoString;
    }
    uint outsiz = BASE64_ENCODED_SIZE( *size );
    char* outdat = pool_sizePlan( outsiz ) > 0
                 ? (char*)pool_setc( '\0', outsiz )
                 : (char*)junk_allocateJunkChunk( outsiz );
    *size = base64_encodeData( outdat, data, *size, 0 );
    return outdat;
}

const byte* base64_decode( const char* data, uint* ptSize )
{
    uint size = ptSize ? *ptSize : (uint)strlen(data);
    if( !ptSize ) { ptSize = &size; }

    uint outsiz = (size > 0 && size < EMPTY)
                ? BASE64_DECODED_SIZE( size )
                : BASE64_DECODED_SIZE( strlen(data) );

    byte* outdat = pool_sizePlan( size ) > 0
                 ? (byte*)pool_setc( '\0', outsiz )
                 : (byte*)junk_allocateJunkChunk( outsiz );

    *ptSize = base64_decodeData( outdat, data, size );
    return outdat;
}

const char* base64_UseTable( b64State* state, const char* changeTo )
{
    const char* oldtable = state->CodeTable;
    state->CodeTable = changeTo;
    return oldtable;
}

const char* base64_useTable( const char* changeTo )
{
    return base64_UseTable( encoderState, changeTo );
}

const char* base64_SetTable( b64State* state, const char* newTable )
{
    if ( !state->isExternCall ) {
        setOption( 'c', newTable );
        newTable = NULL;
    } state->isTableInitialized = false;
    return state->CodeTable = loadeCode( state, newTable );
}

const char* base64_setTable( const char* newTable )
{
    return base64_SetTable( encoderState, newTable );
}

const char* base64_checkTable( const char* newTable )
{
    char* checked = pool_setc( '\0', 65 );
    int i = 0;
    while( i < 64 ) {
        char input = newTable[i];
        int pos = -1;
        while (checked[++pos] != '\0')
            if (input == checked[pos]
            || input == '='
            || ( isSpecialCharacter(input)
        #if BASE64_ALLOW_SPECIAL_CHARACTERS > 0
            && !isAllowedSpecialCharacter(input)
        #endif
            )) break;
        if (pos == i) {
            checked[i] = input;
            ++i;
        }
    }
    if( strlen(checked) < 64 ) {
        setErrorText( "invalid table" );
        return NULL;
    } else {
        checked[64] = '=';
    } return checked;
}

const char* base64_setTable_Checked( const char* newTable )
{
    const char* checked = base64_checkTable( newTable );
    if ( checked )
         return base64_SetTable( encoderState, checked );
    else return pool_get();
}

// either loads given coding table (can be either
// filename or string containing table data self)
// or, if NULL is passed, waits for table input
// via stdin.
const char* base64_newTable( const char* newTable )
{
    if( newTable == NULL ) {
        setOption( 'c', "\0" );
        char* table = (char*)getName('c');
        char tableNotComplete = 64;
        printf("input 64 characters for new table:\n");

        while( tableNotComplete )
        { char input = getch();

            int pos = -1;
            while ( table[++pos] != '\0' )
                if (input == table[pos]
                    || input == '='
                    || ( isSpecialCharacter( input )
                    #if BASE64_ALLOW_SPECIAL_CHARACTERS>0
                       && !isAllowedSpecialCharacter( input )
                    #endif
                    )) break;

            if( pos == (64 - tableNotComplete) ) {
                table[pos] = input;
                tableNotComplete--;
                fwrite( &table[pos], 1, 1, stdout );
            } table[64 - tableNotComplete] = '\0';
        }
        table[64] = '=';
        table[65] = '\0';
        printf( "\n\n" );
        return table;
    } else {
    #ifdef using_environMentor
        base64_pushTable( newTable );
    #endif
        char* tbl = (char*)setOption( 'c', newTable );
        tbl[64]='=';
        tbl[65]='\0';
        return newTable;
    }
}

const char* base64_NewTable( b64State* state, const char* newTable )
{
    const char* same = base64_newTable( newTable );
    return ( newTable == same
           ? state->CodeTable = loadeCode( state, same )
           : same );
}


void base64_dtor(void)
{
    b64State* state = encoderState;
    pool_freeAllCycles();
#ifdef _OnTheFly_
    StringPool* cmdpool = (StringPool*)getDingens("pool");
    if( cmdpool ) {
        pool_attach_ex( cmdpool, encoderState->BasePool );
    } else {
        free( state->BasePool );
      #if DEBUG
        printf( "%s(): freed Base64 pool instance %p\n",
              __FUNCTION__, state->BasePool );
      #endif
    }
#endif
}

void base64_objectDtr( void* objPtr )
{
    b64State* objec = (b64State*)objPtr;
    b64State* state = encoderState;
    pool_attach( objec->BasePool );
}

b64State* base64_InitializeState( b64State* state )
{
    if(!state) {
        state = junk_allocateNewObject( &base64_objectDtr, sizeof(b64State) );
        state->isTableInitialized = false;
#ifdef _OnTheFly_
#ifdef base64
        state->isExternCall = false;
#else
        state->isExternCall = true;
#endif
#else
        state->isExternCall = true;
#endif
    }
 #ifdef _OnTheFly_
    if( !state->isTableInitialized ) {
        if( state == &EncoderState ) {
            encoderState = state;
            state->BasePool = pool_InitializeCycle();
            junk_installCleansener( &base64_dtor );
            pool_push();
            state->BasePool = pool_detach();
        } else {
            b64State* initThis = state;
            state = encoderState;
            initThis->BasePool = pool_push();
            initThis->BasePool = pool_detach();
            state = initThis;
        }
    }
    if (!state->isExternCall) {
   #if DEBUG
        printf("base64_Initialize(): call from other command!");
   #endif
   #if using_environMentor
        const char* tbl = getVariable( "BASE64_CODING_TABLE" );
   #else
        const char* tbl = BASE64_DEFAULTTABLE;
   #endif
        if( isValidArg( tbl ) ) {
            state->CodeTable = tbl;
            state->isTableInitialized = true;
        }
    }
 #else
    QuickCommandInit();
    Pool = pool_getBottom();
 #endif
    if ( !state->isTableInitialized ) state->CodeTable = loadeCode( state, NULL );
    return state;
}

b64State* base64_State( void )
{
    return encoderState;
}

// need to be called once before using base64_ functions
// for initializing internal (commandLiner) buffers and
// loading right coding table for followng opperations.
void base64_Initialize( void )
{
    encoderState = base64_InitializeState( &EncoderState );
}

// return the currently loaded coding table...
// (calls 'base64_Initialize()' implicitely)
const char* base64_GetTable( b64State* state )
{
    return ( !state->isTableInitialized )
         ? base64_InitializeState( state )->CodeTable
         : &state->codeTableBuffer[0];
}

const char* base64_getTable(void)
{
    return base64_GetTable( encoderState );
}

// return the regular base64 coding table...
const char* base64_b64Table(void)
{
    return BASE64_DEFAULTTABLE;
}

#ifdef _OnTheFly_
#if BASE64_ENABLE_STREAMING_API ///////////////////////////
#include ".b64Streams.cc"                                //
#endif ////////////////////////////////////////////////////
#endif

ulong ReadUserInput(const char* mode, char* defaultvalue)
{
#define BeginUserinputRequest Base64StreamPortion
#include "eszentielle/.CommandlinerInputs.h"

    InitDatasetRequest(10);

    if( *(fourCC*)mode == FourCC("search") ) {
        CreateStringInputRequest("\nWhat are you searching (8chars)? [0 for exit]: ", defaultvalue, 'i');
        return byteOrder_stringTOlongCC(getName('i'));
    } else {
        CreateStringInputRequest("\nHowmany bytes you'd like to read? [0 for exit]: ", defaultvalue, 'i');
        return atoll(getName('i'));
    }

#define EndOfUserinputRequest Base64StreamPortion
#include "eszentielle/.CommandlinerInputs.h"
}

uint readFromStream( B64F* file, uint bytecount )
{
    b64Stream* b64f = (b64Stream*)file;
    uint readbytes = 0;
    uint framesize = base64_frameSize( OUTPUT, b64f );
    b64Frame frame;
    do { frame = base64_getFrame( b64f );
        if ( base64_isEndFrame( frame, b64f ) ) {
            if( framesize == 3 && frame.u8[3] < 4 ) {
                fwrite( &frame, 1, framesize -= frame.u8[3], stdout );
                readbytes += framesize;
            } break;
        } else fwrite(&frame, 1, framesize, stdout);
    } while((readbytes += framesize) < bytecount);
    return readbytes;
}

#if defined(base64) ///////////////////////////////////////
// main kram...                                          //
///////////////////////////////////////////////////////////
int outputToConsole(FILE*,byte*,unsigned);
int isPureConfigCall(void) {
    return ( (!hasOption('e')) && (!hasOption('d')) );
}

int main(int argc,char** argv)
{
    if( !(InitCommandLiner(argc,argv)>0) )
        exit(USAGE(base64));

    if(!hasOption('o'))
        setOption('o',"stdout");
    else if (!search('o')) ExitOnError("o-Error");

    if ( hasOption('v') ) {
        setModus("verbose");
    }
    if (!isSwitch('H')) {
        setSwitch('n');
    }

    if ( isModus("verbose") && (!hasOption('h')) ) {
        showOptions();
    }

#ifdef using_environMentor
    if( search('b') ) {
        int newBufferSize = BASE64_BUFFER_SIZE;
        sscanf( getName('b'), "%i", &newBufferSize );
        base64_setBufferSize( newBufferSize );
    }
    if( search('f') ) {
        base64_toggleReverseByteorder( getName('f') );
    }
    if( search('w') ) {
        base64_toggleDefaultTable( getName('w') );
    }
    if( hasOption('r') ) {
        printf( "Resetting table and white list...\n" );
        beginPersistChange( LOCAL );
            removePersistEntry( "BASE64_SPECIAL_CHARACTERS" );
            removePersistEntry( "BASE64_ALLOW_SPECIAL_CHARACTERS" );
        printf( "table now is: %s\n",
                base64_popTable() );
    }
    if( isModus("allow") ) {
        cmLn allow = rawNext('a');
        if((!stringCompare(allow,"on"))||(!stringCompare(allow,"off")))
            base64_allowCharacter( (allow[1]=='n') ? 0x01 : 0x00 );
        else
            base64_allowCharacter( (byte)allow[0] );
    }
    if ( search('l') ) {
        base64_toggleWriteLinebrakes( getName('l') );
    }
#endif

    if( isPureConfigCall() ) {
        if( hasOption('c') ) {
            if( isSwitch('c') )
                setOption('c', base64_NewTable( encoderState, NULL ) );
#ifdef using_environMentor
            else base64_pushTable( getName('c') );
#endif
        } exit( hasOption('h')
              ? USAGE(base64)
              : CheckForError() );
    }

    char Mode = 0;
         if( search('e') )
        Mode = 'e';
    else if( search('d') )
        Mode = 'd';
    else {
        setErrorText("need either 'd' or 'e' parameter given");
        USAGE( base64 );
    }

    ExitOnError( "Parameter" );
    base64_Initialize();

    if (isModus("verbose"))
        printf("\nusing base64 code table:\n%s\n\n", encoderState->CodeTable);

    ExitOnError( "Initialization" );

#if BASE64_ENABLE_STREAMING_API
    if ( hasOption('s') ) {
        printf("option 's' set!\n");
        char modestr[3] = {'r',Mode,'\0'};
        B64F* b64f = base64_createFileStream( getName(Mode), &modestr[0] );
        printf( "\nOpened a base64 %s stream for reading from %s\n\n",
        Mode == 'e'?"encoding":"decoding", getName(Mode) );
        if ( isModus("stream") ) {
            uint bytecount = 128;
            setOption('i',"128");
            while ( bytecount = (uint)ReadUserInput( getName('s'), (char*)getName('i') ) ) {
                bytecount = readFromStream(b64f, bytecount);
                printf("\n\n ...%i bytes read\n",bytecount);
            } exit(CheckForError());
        }
    }
#endif

    int bytesWritten = 0;
    int bytesCoded = 0;
    byte* outbuffer = (byte*)pool_setc( 0, BASE64_BUFFER_SIZE );

    if( hasOption('t') ) {
        bytesCoded = (Mode=='e') ? base64_encodeData( (char*)outbuffer, getName(Mode), (uint)strlen(getName(Mode)), 0 ):
                     (Mode=='d') ? base64_decodeData( (byte*)outbuffer, getName(Mode), (uint)strlen(getName(Mode)) ):
    0;} else if( isDefault('o') ) {
        bytesCoded = (Mode=='e') ? base64_encodeFromFile( getName(Mode), &bytesCoded ):
                     (Mode=='d') ? base64_decodeFromFile( getName(Mode), &bytesCoded ):
    0;} else {
        bytesCoded = (Mode=='e') ? base64_encodeFile( getName('o'), getName(Mode), outbuffer, BASE64_BUFFER_SIZE ):
                     (Mode=='d') ? base64_decodeFile( getName('o'), getName(Mode), outbuffer, BASE64_BUFFER_SIZE ):
    0;}
    ExitOnError( Mode=='e' ? "Encoding" : "Decoding" );

    if ( !isDefault('o') ) {
        if ( isModus("verbose") ) {
           #if BASE64_REVERSED_ORDER > 0
            printf( "reversed byteorder while de-/encoding!\n\n" );
           #endif
            printf( "%i bytes written to file: %s\n\n",bytesCoded,getName('o'));
        } exit( bytesCoded );
    }

    FILE* outputStream = stdout;
    if( stringCompare( getName('o'), "stdout") ) {
        outputStream = fopen( getName('o'), "wb" );
        DEBUGLOG("file opened for writing!")
    }

   #if BASE64_REVERSED_ORDER > 0
    printf( "reversed byteorder while de-/encoding!\n\n" );
   #endif

    char headers = (Mode=='d') * 2;

    bytesWritten = outputToConsole(
        outputStream, &outbuffer[0], bytesCoded
                                    );

    if( isModus("verbose") ) {
        printf( "\n\n%i bytes written to %s (with line breaks and headers)\n"
                "%i byte of data de/en coded (wihout headers or line breaks)\n",
                bytesWritten, getName('o'), bytesCoded );
    }
    exit( getErrorCode() );
}

// writes given buffer "pData" into given stream "outstream"
int outputToConsole( FILE* outstream, byte* pDdata, unsigned cbData )
{
    ptdif written = 0;
    if( hasOption('t') )
        written += fwrite( &pDdata[0], 1, cbData, outstream );
    else
    while( pDdata = (byte*)pool_collectCheckpoint() ) {
        int write = cbData;
        if(cbData > CYCLE_SIZE) {
            write = CYCLE_SIZE;
            cbData-=CYCLE_SIZE;
        } fwrite( pDdata, 1, write, outstream );
        written += write;
    }
    if( !stringCompare( getName('o'), "stdout" ) ) {
        fflush(outstream);
        fclose(outstream);
    } return (int)written;
}
#endif

