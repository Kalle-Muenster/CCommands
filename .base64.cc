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
#define BASE64_CODING_TABLE base64defaultTable
#endif
#ifndef BASE64_BUFFER_SIZE
#define BASE64_BUFFER_SIZE (512)
#endif

#ifndef WINDOWS
#define WINDOWS (0)
#endif
#if WINDOWS == true
    #include <conio.h>
    #define getch _getch
#else //if LINUX == true
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
            if( CodeTable[c] == fourChars.i8[i] ) {\
                bin3.u32 += ( c << SixBitIehShift(i) );\
                c = 65;\
            } if( c == 64 )
#elif BASE64_VERFAHREN == 2
#define BASE64_SELECTED_ALGORYTHM \
        else { int c = DerDickeBatzen[fourChars.i8[i]];\
            if ( !(c<0) ) {\
                bin3.u32 += ( c << SixBitIehShift(i) );\
            } else
#endif


#ifndef     BASE64_SPECIAL_CHARACTERS
const char* BASE64_SPECIAL_CHARACTERS = {
    "-_"
};
#ifndef BASE64_ALLOW_SPECIAL_CHARACTERS
#define BASE64_ALLOW_SPECIAL_CHARACTERS (0)
#endif
#else
#ifndef BASE64_ALLOW_SPECIAL_CHARACTERS
#define BASE64_ALLOW_SPECIAL_CHARACTERS (1)
#endif
#endif


#ifdef _OnTheFly_
 #define PREPARE_POOLBOTTOM Base64Pool
 #include ".poolBottom.h"
 StringPool* Base64Pool;
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
     -r               : resets the current coding table back to standard.\n\
    --f-[on|off]      : toggles 'reverse byteorder during decode' setting.\n\
                        %s\n\
    --l-[on|off]      : toggles 'handle linebreaks during decode' setting.\n\
                        %s\n\
    --b-<buffersize>  : sets new size for internal used buffer\n\
                        %i byte\n\n";
 #endif

int USAGE(base64)
{
    Assert('h', "no arguments where given!");
   #ifdef using_environMentor
    environMentor_CheckAndEnable();
    Synopsis("[options] <[--e/--d]-SourceFile> [--o-OutputFile]\n  base64 [persitent options]\n");
    printf( DOKU(base64) );
    printf( DOKU(base64_persist),
        BASE64_ALLOW_SPECIAL_CHARACTERS?toQuoted(BASE64_SPECIAL_CHARACTERS):"DISABLED",
        BASE64_REVERSED_ORDER ?"ENABLED":"DISABLED",
        BASE64_WITH_LINEBREAKS?"ENABLED":"DISABLED",
        BASE64_BUFFER_SIZE
    );
   #else
    Synopsis("[options] <[--e/--d]-SourceFile> [--o-OutputFile]\n");
    printf( DOKU(base64) );
   #endif
    if(isSwitch('v'))
        showOptions();
    CheckForError();
    return getErrorCode();
}
#endif


char _codeTableBuffer[65];
bool _isTableInitialized = false;
bool _isExternCall = true;
const char* CodeTable;

const char* base64defaultTable = {
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="
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
        sprintf(&buffer[0],"\"%s\"",base64defaultTable);
        setPersistEntry("BASE64_PUSHED_TABLE",&buffer[0]);
    commitPersistChange();
    printf("Table Popped!\n");
    return BASE64_PUSHED_TABLE;
}

const char* base64_pushTable(const char* table)
{
    if(table[0]!='\0') {
        char buffer[MAX_NAM_LEN];
        sprintf(&buffer[0],"\"%s\"",getFeatured("BASE64_CODING_TABLE").value.string);
        beginPersistChange(LOCAL);
            setPersistEntry("BASE64_PUSHED_TABLE",&buffer[0]);
            sprintf(&buffer[0],"\"%s\"",table);
            setPersistEntry("BASE64_CODING_TABLE",&buffer[0]);
        commitPersistChange();
        printf("Table Pushed!\n");
    } return BASE64_CODING_TABLE;
}

void base64_setBufferSize(int newSize)
{
    if(newSize>640000) {
        setErrorText("please don't allocate buffers greater then 640k please");
    } else {
        //setMacroNum("BASE64_BUFFER_SIZE",newSize);
        beginPersistChange(LOCAL);
            tempf("(%i)",(const char*)(ptval)newSize);
            setPersistEntry("BASE64_BUFFER_SIZE",getTemp());
            setPersistEntry("CYCLE_SIZE",getTemp());
            int cycle_size = newSize / 64;
            tempf("(%i)",(const char*)(ptval)cycle_size);
            setPersistEntry("CYCLE_COUNT",getTemp());
        commitPersistChange();
        printf("set buffer size to %i byte...\n",newSize);
    }
}

void base64_toggleReverseByteorder(const char* onOrOff)
{
    int ReverseDuringDecode = BASE64_REVERSED_ORDER;
    if((!strcmp(onOrOff,"on"))||(!strcmp(onOrOff,"off")))
        ReverseDuringDecode = onOrOff[1]=='f';
    // toggle current setting if NULL was passed
    setMacroNum("BASE64_REVERSED_ORDER",(ReverseDuringDecode-1)*(-1));
    printf( "reverse byte order during decode: %s\n",
            ReverseDuringDecode?"DISABLED":"ENABLED" );
}

void base64_toggleWriteLinebrakes(const char* onOrOff)
{
    int EncoderWeitesLinebreaks = BASE64_WITH_LINEBREAKS;
    if((!strcmp(onOrOff,"on"))||(!strcmp(onOrOff,"off")))
        EncoderWeitesLinebreaks = onOrOff[1]=='f';
    // toggle current setting if NULL was passed
    setMacroNum("BASE64_WITH_LINEBREAKS",(EncoderWeitesLinebreaks-1)*(-1));
    printf( "encoded output contains line break every 64 chars: %s\n",
            EncoderWeitesLinebreaks?"DISABLED":"ENABLED" );
}

// allow characters to be contained within coding tables:
// passing char argument: add given character to the white list.
// passing true/false: enable or disable to allow all characters
//                     contained in the white list.
void base64_allowCharacter(byte allowed)
{
    if(allowed>1) {
        pool_setCheckpoint();
        #if !defined(BASE64_SPECIAL_CHARACTERS)
            pool_setc('\"',3)[1]=allowed;
        #else
            pool_setf("\"%s",BASE64_SPECIAL_CHARACTERS);
            if (isSpecialCharacter(allowed) && (!isAllowedSpecialCharacter(allowed))) {
                pool_setc(allowed,1);
            } pool_setc('\"',1);
        #endif
        char* whitelist = pool_collectCheckpoint();
        beginPersistChange(LOCAL);
            setPersistEntry("BASE64_SPECIAL_CHARACTERS",whitelist);
        commitPersistChange();
        printf("white listed characters: %s\n",whitelist);
    } else {
        if(allowed){
            setMacroNum("BASE64_ALLOW_SPECIAL_CHARACTERS",1);
            setTemp("white listed characters are ALLOWED\n");
        }else{
            setMacroNum("BASE64_ALLOW_SPECIAL_CHARACTERS",0);
            setTemp("white list disabled!\n");
        } fwrite(getTemp(),1,strlen(getTemp()),stdout);
    }
}
#else // if not using_environMentor
#define NUMBER_OF_FEATURES (0)
#endif


////////////////////////////////////////////////////////////////////////////
// Internal used functions:

// saves the used coding table beside the output file
// as *_table.dat when not encoding by standard table
const char* saveTableFile( char* b64table )
{
    if( hasOption('e')
    &&  hasOption('c') ) {
        char buffer[128] = {'\0'};
        sprintf( &buffer[0], "%s_table.dat", getName('e') );
        FILE* b64TableFile = fopen(&buffer[0],"w");
        fwrite( b64table, 1, 65, b64TableFile );
        fflush( b64TableFile );
        fclose( b64TableFile );
        if( hasOption('v') )
            printf( "Saved given base64 table to file: %s\n",
                    &buffer[0] );
    } return (const char*)b64table;
}


#if  BASE64_VERFAHREN == 2
char DerDickeBatzen[256]; /* = {
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
}; */
#endif


// decides which base64 table will be used for any following de/en code opperations.
// It gegebenenfalls loads it from 'file' or from 'string' if this maybe was defined
// (by option "c") previously. - when 'string', it will also output a secondary file
// called "*_table.dat", which will contain that table. If no option "c" is defined,
// it just uses the last macro which was pushed by the "base64_pushTable()" function,
// or the default one, when there never was pushed a table before.
const char* loadeCode(void)
{
    if( isSwitch('c') )
        base64_newTable( NULL );

    char* coderLoader =(char*) ( hasOption('c') ?
                                   getName('c') : NUMBER_OF_FEATURES > 1
                                                ? BASE64_CODING_TABLE
                                                : base64defaultTable );
    int i=-1;
    const char* loade = NULL;
    while( coderLoader[++i] != '\0' ) {
        if( (coderLoader[i] == '=') && ( i >= 64 ) ) {
            if(_isExternCall ) {
                DEBUGLOG("call from other command!")
                loade = &coderLoader[i - 64];
            } else {
                loade = saveTableFile( &coderLoader[i - 64] );
            }
        }
    } if( !loade ) {
        FILE* codeFile = fopen( coderLoader, "r" );
        if( codeFile ) {
            fread( &_codeTableBuffer[0], 1, 65, codeFile );
            fflush(codeFile); fclose(codeFile);
            loade = &_codeTableBuffer[0];
        } else
            loade = base64defaultTable;
    }
   #if BASE64_VERFAHREN == 2
    for( int i=0; i<256; ++i )
        DerDickeBatzen[i] = -1;
    for( int i=0; i<65; ++i )
        DerDickeBatzen[loade[i]] = i;
   #endif
    _isTableInitialized = true;
    DEBUGFMT("using coding table: %s",loade)
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

// check if char is 'special' AND if that spacial is actually whitelisted at least
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

// encodes one frame (first 3 byte of 'frame' to 4 chars base64-data)
b64Frame base64_encodeFrame( b64Frame frame )
{
    uint threeByte = frame.u32;
    frame.u32 = 0;
   #if BASE64_REVERSED_ORDER
    threeByte = byteOrder_reverse32( threeByte << 8 );
    for( uint i=3; i<EMPTY; --i ) {
   #else
    for( uint i=0; i<4; ++i ) {
   #endif
        frame.u8[i] = CodeTable[threeByte%64];
        threeByte >>= 6;
    } return frame;
}

// decode one frame (4 chars base64-data to 3 byte binary data)
b64Frame base64_decodeFrame( b64Frame fourChars )
{
    b64Frame bin3 = {0};
    for( int i=0; i<4; ++i ) {
        if( fourChars.i8[i] == '=' ) {
            bin3.u32 += ( 64 << SixBitIehShift(i) );
            bin3.i8[3]=(4-i); break;
        } BASE64_SELECTED_ALGORYTHM {
            bin3.i8[3] = (char)setError(
                "invalid base64 data", 4-i );
            i = 4; }
        }
    }
   #if BASE64_REVERSED_ORDER
    byte ctrl = bin3.u8[3];
    bin3.u32 = byteOrder_reverse32( bin3.u32 ) >> 8;
    bin3.u8[3] = ctrl;
   #endif
    return bin3;
}

// encode data batzen src to base64 string dst... (returns encoded size)
int base64_encodeData( char* dst, const byte* src, unsigned cbSrc )
{
    uint lB= 0;
    uint iD= 0;
    uint iS=((cbSrc+3)%3); // is adding 3 neccessary?
    if( iS ) cbSrc+=(3-iS);
    iS = 0;

    do { asFrame( &dst[lB+iD] ) =
            base64_encodeFrame( asFrame( &src[iS] ) );
        iD += 4;
   #if BASE64_WITH_LINEBREAKS > 0
         if( iD%64 == 0 )
            dst[iD+lB++] = '\n';
   #endif
    } while ( cbSrc > (iS+=3) );

    dst[(iD += lB)]='=';
    dst[iD+1]='\0';
    DEBUGLOG(dst)
    return iD;
}

// decode base64 string 'src' to byte data 'dst'... (returns decoded size)
int base64_decodeData( byte* dst, const char* src )
{
    int iS = -4;
    int iD = -3;
    int iT =  0;

    while (! (iT = (
        asFrame( &dst[iD+=3] ) = base64_decodeFrame( asFrame( &src[iS+=4] ) )
            ).i8[3]) );

    dst[iD+=(4-iT)]='\0';
    return iD;
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
        frame = base64_encodeFrame( frame );
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
    } frame = base64_encodeFrame( frame );
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
    while(pos>3) {
        int linebreak=0;
        pos-=fscanf(f,"%s\n",&readbuffer[0]);
        char* r = &readbuffer[0];
        int p = 0;
        while(*r){ ++p;
            if(*r==10||*r==13) {
                *r = '\0';
            } else {
                ++r;
            }
        } b64Frame* rf = (b64Frame*)r;
        while(p) {
            uint binR = base64_decodeFrame( *rf ).u32;
            pool_setb( (void*)&binR, 3 );
            outPosition += 3;
            p-=4; ++rf;
        }
    } frame.i8[0] = frame.i8[1] = frame.i8[2] = frame.i8[3] = '=';
    if( pos > 0 ) {
        fread( &frame.i8[0], 1, pos, f );
    } frame = base64_decodeFrame( frame );
    pool_setb( (void*)&frame.u32, 3 );
    outPosition += 3;
    fflush(f); fclose(f);
    DEBUGFMT("outputSize: %i",*out_len=outPosition)
    return *out_len;
}

// encode content of file dst to file src
int base64_encodeFileToFile( FILE* dst, FILE* src )
{
    int cpos = ftell(src);
    fseek(src,0,SEEK_END);
    int size = ftell(src)-cpos;
    fseek(src,cpos,SEEK_SET);
    b64Frame dat;
    dat.u32 = 0;
    cpos = 0;
    int lb = 0;
    while( size>2 ) {
        size -= (int)fread( &dat, 1, 3, src );
        dat = base64_encodeFrame( dat );
        cpos += (int)fwrite( &dat, 1, 4, dst );
   #if BASE64_WITH_LINEBREAKS > 0
        if( cpos%64 == 0 ) {
            lb += (int)fwrite( "\n", 1, 1, dst );
        }
   #endif
    } if( size )
        lb += (int)fwrite("=",1,size,dst);
    else
        lb += (int)fwrite("=",1,4,dst);
    return cpos+lb;
}

// decode base64 content from file src to file dst
int base64_decodeFileToFile( FILE* dst, FILE* src )
{
    int size = 0;
    int last = 0;
    char rbuf[5] = {0,0,0,0,0};
    char* buf = &rbuf[0];
    int in = 4;
    while( in == 4 ) { in = 0;
        while( in < 4 ) {
            if( fread( &rbuf[in], 1, 1, src ) == EOF ) break;
            char r = rbuf[in];
            in += ( r != 10 && r != 13 );
        } if( in < 4 ) {
            for( int i=4; i>in; )
                rbuf[--i]='=';
        } b64Frame f = base64_decodeFrame( asFrame(buf) );
        if( f.i8[3] ) {
            #if DEBUG
            printf( "EOS-frame: control byte is: %i\n", f.i8[3] );
            #endif
            if( f.i8[3] < 4 )
                size += (int)fwrite( &f.i8[0], 1, 3-f.u8[3], dst );
            return size;
        } else size += (int)fwrite( &f.i8[0], 1, 3, dst );
    } return size;
}

// encode binary content of file 'src' to base64 data into file 'dst'
int base64_encodeFile( const char* dst_nam, const char* src_nam )
{
    FILE* dst = fopen( dst_nam, "w" );
    FILE* src = fopen( src_nam, "rb" );
    int outlen = base64_encodeFileToFile( dst, src );
    fflush(dst);
    fclose(dst);
    fclose(src);
    return wasError() ? 0 : outlen;
}

// decodes content of file 'src' to file 'dst'
int
base64_decodeFile( const char* dst_nam, const char* src_nam )
{
    FILE* dst = fopen( dst_nam, "wb" );
    FILE* src = fopen( src_nam, "r" );
    int outlen = base64_decodeFileToFile( dst, src );
    fflush(dst);
    fclose(dst);
    fclose(src);
    return wasError() ? 0 : outlen;
}

const char* base64_encode( const byte* data, uint size )
{
    if(!size)
        size = (uint)strlen((const char*)data);
    uint theBiggerSize = BASE64_ENCODED_SIZE(size);
    if( pool_sizePlan( theBiggerSize ) > 0 ) {
        base64_encodeData( pool_setc('\0',theBiggerSize), data, size );
        return pool_get();
    } else {
        char* chunk = (char*)junk_allocateJunkChunk( theBiggerSize );
        base64_encodeData( chunk, data, size );
        return chunk;
    }
}

const byte* base64_decode( const char* data, uint* size )
{
    uint decoded = 0;
    if( !size ) { size = &decoded; }
        *size = *size > 0 ? BASE64_DECODED_SIZE(*size)
              : BASE64_DECODED_SIZE(strlen(data));
    if( pool_sizePlan( *size ) ) {
        *size = base64_decodeData( (byte*)pool_setc('\0', *size ), data );
        return (byte*)pool_get();
    } else {
        byte* chunk = (byte*)junk_allocateJunkChunk( *size );
        *size = base64_decodeData( chunk, data );
        return chunk;
    }
}



const char* base64_setTable( const char* newTable )
{
    setOption( 'c', newTable );
    _isTableInitialized = false;
    return CodeTable = loadeCode();
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
         return base64_setTable( checked );
    else return pool_get();
}

// either loads given coding table (can be either
// filename or string containing table data self)
// or, if NULL is passed, waits for table input
// via stdin.
const char* base64_newTable( const char* newTable )
{
    if (newTable == NULL) {
        setOption('c', "\0");
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
        return CodeTable = loadeCode();
    }
}

void base64_dtor(void)
{
#ifdef _OnTheFly_
    pool_attach( Base64Pool );
    pool_pop();
#endif
    pool_freeAllCycles();
}

// need to be called once before using base64_ functions
// for initializing internal (commandLiner) buffers and
// loading right coding table for followng opperations.
void base64_Initialize(void)
{
 #ifdef _OnTheFly_
    if (!_isTableInitialized) {
        Base64Pool = pool_InitializeCycle();
        pool_push();
        Base64Pool = pool_detach();
        junk_installCleansener( &base64_dtor );
    }
    if(!_isExternCall) {
   #if DEBUG
        printf("base64_Initialize(): call from other command!");
   #endif
   #if using_environMentor
        const char* tbl = getVariable( "BASE64_CODING_TABLE" );
   #else
        const char* tbl = base64defaultTable;
   #endif
        if( isValidArg( tbl ) ) {
            CodeTable = tbl;
            _isTableInitialized = true;
        }
    }
 #else
    QuickCommandInit();
    Pool = pool_getBottom();
 #endif
    if ( !_isTableInitialized ) CodeTable = loadeCode();
}

// return the currently loaded coding table...
// (calls 'base64_Initialize()' implicitely)
const char* base64_getTable(void)
{
    if( !_isTableInitialized ) base64_Initialize();
    return CodeTable;
}

// return the regular base64 coding table...
const char* base64_b64Table(void)
{
    return base64defaultTable;
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
            base64_allowCharacter( (allow[1]=='n') ? 1 : 0 );
        else
            base64_allowCharacter( allow[0] );
    }
    if ( search('l') ) {
        base64_toggleWriteLinebrakes( getName('l') );
    }
#endif

    if( isPureConfigCall() ) {
        if( hasOption('c') ) {
            if( isSwitch('c') )
                setOption('c',base64_newTable(NULL));
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
        printf("\nusing base64 code table:\n%s\n\n", CodeTable);

    ExitOnError( "Initialization" );

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

    int bytesWritten = 0;
    int bytesCoded = 0;
    byte outbuffer[BASE64_BUFFER_SIZE];

    if( hasOption('t') ) {
        bytesCoded = (Mode=='e') ? base64_encodeData((char*)&outbuffer[0],getName(Mode), strlen(getName(Mode))):
                     (Mode=='d') ? base64_decodeData((byte*)&outbuffer[0],getName(Mode)):
    0;} else if( isDefault('o') ) {
        bytesCoded = (Mode=='e') ? base64_encodeFromFile( getName(Mode), &bytesCoded ):
                     (Mode=='d') ? base64_decodeFromFile( getName(Mode), &bytesCoded ):
    0;} else {
        bytesCoded = (Mode=='e') ? base64_encodeFile( getName('o'), getName(Mode) ):
                     (Mode=='d') ? base64_decodeFile( getName('o'), getName(Mode) ):
    0;}
    ExitOnError( Mode=='e' ? "Encoding" : "Decoding" );

    if ( !isDefault('o') ) {
        if ( isModus("verbose") ) {
           #if BASE64_REVERSED_ORDER > 0
            printf( "reversed byteorder while de-/encoding!\n\n" );
           #endif
            printf( "%i bytes written to file: %s\n\n",bytesCoded,getName('o'));
        } exit(bytesCoded);
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
    int written = 0;
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
    } return written;
}
#endif
