/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      hexString.cc                               ||
||     Author:    Kalle Muenster                             ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifdef _OnTheFly_
 #include ".environMentor.h"
 StringPool* Base16Pool = NULL;
 #define PREPARE_POOLBOTTOM Base16Pool
 #include ".poolBottom.h"
#else
 #include ".CheckForProjectSettings.h"
 #include ".hexString.h"
#ifndef  CYCLE_SIZE
 #define CYCLE_SIZE (2048)
 #include ".stringPool.h"
 StringPool* Pool = NULL;
#endif
#endif


#ifdef _OnTheFly_
#include ".commandLiner.h"
#define CYCLE_SIZE_ERROR \
setErrorText(pool_setfi("CYCLE_SIZE (%i)",CYCLE_SIZE));
int USAGE(hexString)
{
    Synopsis("<-d|-e> [mode] <data>\n  hexString <-x|-n> <number>");
    printf("   -x    : convert decimal number to hexadecimal number\n\
   -n    : convert hexadecimal number to decimal number\n\
   -e    : encode binary data to base16 string.\n\
   -d    : decode base16 string to binary data.\n\
   -s    : de/en code from stdin\n\
           if -f is also present, into file <filename>\n\
   -f    : if -s is given also, de/en code into file\n\
           if -s is ommitted, de/en code from file\n\n");
    printf("  Persistent Options:\n\n\
   -b <size> : set buffersize. currently assigned:\n\
                   %i byte\n\n",CYCLE_SIZE);
    if(hasOption('v'))
        showOptions();
    return (CheckForError()||(!hasOption('h')));
}
#else
 #ifndef  DEBUG
  #define DEBUG 0
  #define CYCLE_SIZE_ERROR
 #else
  #define CYCLE_SIZE_ERROR \
  printf( "%s(): Error: CYCLE_SIZE (%i)\n",\
          __FUNCTION__, CYCLE_SIZE );
 #endif
#endif

#include ".byteOrder.h"

#define SLICE_SIZE (CYCLE_SIZE/CYCLE_COUNT)

const char xehxeh[16] = {
    '0','1','2','3','4','5','6','7',
#if (defined(HEXSTRING_UPPERCASE) && (!defined(HEXSTRING_LOWERCASE)))
    '8','9','A','B','C','D','E','F'
#else
    '8','9','a','b','c','d','e','f'
#endif
};

const char hexhex[128] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1, 0, 1,
     2, 3, 4, 5, 6, 7, 8, 9,-1,-1,
    -1,-1,-1,-1,-1,10,11,12,13,14,
    15,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,10,11,12,
    13,14,15,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1
};

//------------ convert numbers from/to decimal/hexadecimal ----------------//

char* hexString_toNum( const char* hex )
{
    uint len = strlen(hex);
    ulong numval = 0;
    while( *hex == '0' ) {
        if( *++hex == '\0' ) {
            return pool_set( len > 8 ? "00000000000000000000" : "0000000000" );
        }
    } len = strlen(hex);

    if( len > 16 ) {
        return setError( "wordsize", FourCC("bits") );
    } else if ( len == 16 ) {
        int i = 0;
        while( i < 16 ) {
            char upper = (char)toupper( hex[i] );
            if( upper == 'F' ) ++i;
            else if( upper < 'F' ) break;
            else return setError( "wordsize", FourCC("bits") );
        }
    }
    uint size = len / 2;
    size += size;
    hexString_toBin( hex, &size, (byte*)&numval, size/2 );
    const char* fmtstr = ( numval > UINT32_MAX ) ? "%llu" : "%u";
    return pool_setfi( fmtstr, numval );
}

char* hexString_fromNum( const char* num )
{
    uint len = strlen( num );
    ulong numval = 0;
    while( *num == '0' ) {
        if( *++num == '\0' ) {
            return pool_set( len > 10 ? "0000000000000000" : "00000000" );
        }
    } len = strlen( num );

    if( len > 20 ) {
        return setError( "wordsize", FourCC("bits") );
    } else if ( len == 20 ) {
        const char* maxstr = "18446744073709551615";
        int i = 0;
        while( i < 20 ) {
            if( num[i] == maxstr[i] ) ++i;
            else if( num[i] < maxstr[i] ) break;
            else return setError( "wordsize", FourCC("bits") );
        }
    }
    sscanf( num, "%llu", &numval );
    uint size = (numval <= UINT32_MAX) ? 4u : 8u;
    char buff[32];
    buff[ hexString_toHex( (byte*)&numval, size, &buff[0], size*2 ) ] = '\0';
    return pool_set( &buff[0] );
}

//------------- convert data from binary to base16 and vice verse ------------//

uint hexString_toHex( const byte* srcBin, uint srcLen, char* dstHex, uint dstLen )
{
    int end = (dstLen/2) + (dstLen%2);
    end = srcLen < end ? srcLen : end;
    for( int i=0, I=(end*2)-2; i < end; ++i, I-=2 ) {
        dstHex[I]   = xehxeh[srcBin[i]/16];
        dstHex[I+1] = xehxeh[srcBin[i]%16];
    } dstHex[ end+=end ] = '\0';
    return end;
}

uint hexString_toBin( const char* srcHex, uint* srcLen, byte* dstBin, uint dstLen )
{
    int srcLength = srcLen? *srcLen : strlen(srcHex);
    int i = srcLength; uint c = 0;
    int n = 0; char oneByte[2];
    while( (--i >= 0 ) && (c < dstLen) ) {
        if( (oneByte[n]=hexhex[(byte)srcHex[i]]) >= 0 )
            if(++n==2)
                dstBin[c++] = oneByte[n=0]
                            + oneByte[1]*0x10;
    } if(srcLen)
        *srcLen = (srcLength - i)-1;
    return c;
}


char* hexString_int64ToHex( ulong dat ) {
#ifdef using_commandLiner
    hexString_toHex( (byte*)&dat, 8u, getTemp(), 16u );
    return getTemp();
#else
    hexString_toHex( (byte*)&dat, 8u, pool_setc(0,16), 16u );
    return pool_get();
#endif
}

ulong hexString_hexToInt64( const char* sixteen ) {
    char data[9]; uint len=16;
    hexString_toBin( sixteen, &len, &data[0], 8u );
    return *(ulong*)&data[0];
}

char* hexString_int32ToHex( uint dat ) {
#ifdef using_commandLiner
    hexString_toHex( (byte*)&dat, 4u, getTemp(), 8u );
    return getTemp();
#else
    hexString_toHex( (byte*)&dat, 4u, pool_setc(0,8), 8u );
    return pool_get();
#endif
}

uint hexString_hexToInt32( const char* aight ) {
    ulong data; uint len=8;
    hexString_toBin( aight, &len, (byte*)&data, 4u );
    return (uint)data;
}

byte* hexString_fromHex( const char* hxst, uint* outLen )
{
    uint outSiz = 0;
    if( !outLen ) outLen = &outSiz;
    int srcLength=strlen(hxst);
    uint* outwrite;
    if( pool_setCheckpoint() ) {
        byte dstBuf[SLICE_SIZE+1];
        dstBuf[SLICE_SIZE]=0;
        int inRead=0; *outLen = 0;
        while( inRead < srcLength ) {
            uint bytecount = srcLength-inRead;
            uint dstWrite = hexString_toBin(
                 &hxst[inRead], &bytecount, &dstBuf[0], SLICE_SIZE
                                             );
            pool_setb( &dstBuf[0], dstWrite );
            inRead += bytecount;
           *outLen += dstWrite;
        } return (byte*)pool_collectCheckpoint();
    } else {
        CYCLE_SIZE_ERROR
        return (byte*)0;
    }
}

char* hexString_fromBin( const byte* srcBin, uint binLen )
{
    int pos;
    if((pos = pool_sizePlan( binLen*2 ))!=0) {
        uint* size;
        if( pos < 0 )
            pool_push();
        pos = 0;
        size = pool_setCheckpoint();
        char dstHex[SLICE_SIZE];
        *(word*)&dstHex[SLICE_SIZE-2] = (word)0;
        const uint MAX_CHUNK_LEN = (SLICE_SIZE/2);
        do{ uint leftb = binLen - pos;
            uint chunk = leftb < MAX_CHUNK_LEN
                       ? leftb : MAX_CHUNK_LEN;
            pos += ( hexString_toHex(
                 &srcBin[pos], chunk, &dstHex[0], SLICE_SIZE-2
                                        ) / 2 );
            pool_set( &dstHex[0] );
        } while( (*size) && (*size < (binLen*2)) );
        return pool_collectCheckpoint();
    } else {
        CYCLE_SIZE_ERROR
        return (char*)0;
    }
}

void hexStringDtor( void )
{
    StringPool* hexbottom = (StringPool*)getDingens("hexo");
    StringPool* cmdbottom = (StringPool*)getDingens("pool");
    if( hexbottom != Base16Pool->running ) {
      #if DEBUG
        printf("%s(): hexpool not at bottom!\n",__FUNCTION__);
      #endif
        pool_freeAllCycles_ex( hexbottom );
    }
    if( cmdbottom ) {
        pool_attach_ex( cmdbottom, hexbottom );
        pool_pop_ex( cmdbottom );
    } else {
        free( hexbottom );
      #if DEBUG
        printf("%s(): freed hexpool bottom %p\n",__FUNCTION__,hexbottom);
      #endif
    }
}

void hexString_Initialize( void )
{
    if( Base16Pool == NULL ) {
        Base16Pool = pool_InitializeCycle();
        pool_push();
        Base16Pool = pool_detach();
        addDingens( "hexo", Base16Pool, &hexStringDtor );
    }
}

#ifdef hexString

#define mod(cnt) ((cnt-8) % MAX_NAM_LEN)
#define pos(cnt) (cnt % MAX_NAM_LEN)
#define end(hex) (*(ulong*)&incoming[hex]==t)

int hexString_setBufferSize( cmLn size )
{
    int newSize = atoi(size);
    if( newSize>0 && newSize < 640000 ) {
        int newCount = newSize/MAX_NAM_LEN;
        pool_setfi("(%i)",newCount);
        pool_setfi("(%i)",newSize);
        beginPersistChange(LOCAL);
            setPersistEntry("CYCLE_SIZE",pool_get());
            setPersistEntry("CYCLE_COUNT",pool_last(2));
        commitPersistChange();
        return newSize;
    } else setErrorText("invalid size parameter");
    return NULL;
}

int main( int argc, char**argv )
{
    if(! InitCommandLiner( argc, argv )
      || hasOption('h') )
        exit( USAGE(hexString) );

    if( hasOption('b') ) {
        if ( isSwitch('b') )
            switchIdx('b', indexOfName(rawNext('b')) );
        if ( !hexString_setBufferSize(getName('b')) )
            ExitOnError("ARGUMENT");
        else
            printf("\n set hexString buffersize to: %s\n\n",getName('b'));
        exit(EXIT_SUCCESS);
    }

    hexString_Initialize();

    if(hasOption('x')) {
        if(!exists('x')) ExitOnError("Parameter");
        printf( "%s\n", hexString_fromNum( getName('x') ) );
        exit(CheckForError());
    }
    if(hasOption('n')) {
        if(!exists('n')) ExitOnError("Parameter");
        printf( "%s\n", hexString_toNum( getName('n') ) );
        exit(CheckForError());
    }

    short mode = hasOption('e') ? 'e'
               : hasOption('d') ? 'd'
               : false;
    if( !mode ) { setErrorText("either -e or -d must be given");
        exit(USAGE(hexString)); }

    if(hasOption('v')) showOptions();

    if(hasOption('e')) if (isSwitch('e')) switchIdx('e',indexOfName(rawNext('e')));
    if(hasOption('d')) if (isSwitch('d')) switchIdx('d',indexOfName(rawNext('d')));
    if(hasOption('f')) if (isSwitch('f')) switchIdx('f',indexOfName(rawNext('f')));
    if(hasOption('s')) if (isSwitch('s')) switchIdx('s',indexOfName(rawNext('s')));

    if(hasOption('v')) showOptions();

    FILE* o = stdout;
    if( !hasOption('s') ) {
        if( !hasOption('f') ) {
            if( mode=='e' )
                printf("\n%s\n", hexString_fromBin( getName('e'), strlen(getName('e')) ) );
            else
                printf("\n%s\n", hexString_fromHex( getName('d'), NULL ) );
        } else {
            FILE* f = fopen(getName(mode),mode=='e'?"rb":"r");
            if( !isSwitch('f') ) {
                o = fopen(getName('f'),mode=='e'?"w":"wb");
            }
            fseek(f,0,SEEK_END);
            int srcLength = ftell(f);
            fseek(f,0,SEEK_SET);
            const int chunksize = mode == 'd' ? (CYCLE_SIZE) : (CYCLE_SIZE/2);
            chunksize = chunksize < srcLength
                      ? chunksize : srcLength;
            char buffer[chunksize+1];
            int progress = 0;
            while( progress < srcLength ) {
                int byteLeft = srcLength - progress;
                int currentchunk = byteLeft < chunksize
                                 ? byteLeft : chunksize;
                if( mode == 'd' && currentchunk < chunksize )
                    currentchunk += (currentchunk % 2);
                progress += fread( &buffer[0], 1, currentchunk, f );
                DEBUGFMT("readbytes: %i",progress)
                buffer[currentchunk]='\0';
                if( mode == 'e' ) {
                    DEBUGFMT("currentchunk: %i",currentchunk)
                    DEBUGFMT("maxchunksize: %i",chunksize)
                    fwrite( hexString_fromBin( &buffer[0], currentchunk ), 1, (currentchunk*2), o );
                } else {
                    byte* bindat = hexString_fromHex( &buffer[0], &currentchunk );
                    fwrite( bindat, 1, currentchunk, o );
                }
            }
            printf("\n\n");
                fflush(f);
                fclose(f);
            if( o != stdout ) {
                fflush(o);
                fclose(o);
            }
        } ExitOnError( "Buffer" );
        printf( "\n\n" );
        exit( EXIT_SUCCESS );
    } else {
        int modus = mode=='d' ? 1 : 2;
        if(!isSwitch('s'))
            o = fopen(getName('s'),mode=='d'?"wb":"w");
        else
            pool_setCheckpoint();
        byte incoming[MAX_NAM_LEN+9];
        incoming[MAX_NAM_LEN+8] = 0;
        byte outgoing[modus==2?MAX_NAM_LEN*2:MAX_NAM_LEN/2];
        int streaming = true;
        const char Terminstor[8] = {'H','e','x','H','e','x','!','\n'};
        const ulong t = *(ulong*)&Terminstor[0];
            int count = 0;
            int c = 8;
            int p = pos(c);
            int m = mod(c);
            int P = m+8;
            int C = false;
            while( streaming && (p<MAX_NAM_LEN) ) {
                incoming[P=(m=mod(c))+8]=(byte)getch();
                if( (p=pos(c)) < m ) {
                    incoming[p]=incoming[P];
                    C = true;
                } else if (C) {
                    if(o!=stdout) fwrite( modus==2
                                        ? hexString_fromBin(&incoming[8],MAX_NAM_LEN)
                                        : hexString_fromHex(&incoming[8],NULL),
                                          modus, MAX_NAM_LEN, o );
                    else {
                        count = ( modus == 2 ? hexString_toHex(&incoming[8],MAX_NAM_LEN,&outgoing[0],MAX_NAM_LEN*2)
                                             : hexString_toBin(&incoming[8], NULL, &outgoing[0], MAX_NAM_LEN / 2 ));
                        pool_setb(&outgoing[0],count);
                    } C = false;
                } streaming = !end(m);
            } if(o!=stdout) {
                fflush(o); fclose(o);
                printf("output saved to file: %s\n",getName('s'));
            }
        printf("\n\n");
        exit( CheckForError() );
    }
}//end
#endif
