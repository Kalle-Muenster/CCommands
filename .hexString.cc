/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      hexString.cc                               ||
||     Author:    Kalle Muenster                             ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifdef _OnTheFly_
 #include ".environMentor.h"
#else
 #include ".CheckForProjectSettings.h"
 #include ".hexString.h"
#endif

#ifndef  CYCLE_SIZE
 #define CYCLE_SIZE (2048)
#endif

#include ".stringPool.h"

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
	uint lang = strlen(hex);
	uint kurz = (lang/16)+1;
#if defined(__TINYC__)
	ulong data[kurz];
#else
    ulong* data = (ulong*)malloc(kurz*sizeof(ulong));
#endif
    pool_setCheckpoint();
	hexString_toBin( hex, (uint*)&lang, (byte*)&data[0], kurz*16 );
	do { --kurz;
		pool_setfi( "%llu", data[kurz] );
	} while( kurz );
#if !defined(__TINYC__)
    free(data);
#endif
	return pool_collectCheckpoint();
}

char* hexString_fromNum( const char* num )
{
	char wumm[32]; ulong valum = 0;
	int gum=0; pool_setCheckpoint();
	const char* nam = num; int gam = gum;
	while( *++nam ) { ++gam;
	    while( *nam == ' ' ) ++gum; 
	    if( gum ) { 
			valum = atol( num );
			int out = hexString_toHex((byte*)&valum,8u,&wumm[0],16u);
			wumm[out]='\0';
			pool_set( &wumm[0] ); gam = 0;
			num = nam; 
			gum = gam;
		}
	} if( gam ) {
		valum = atol( num );
		char wumm[32];
		wumm[hexString_toHex((byte*)&valum,8u,&wumm[0],16u)]='\0';
		pool_set( &wumm[0] );
	} return pool_collectCheckpoint();
}

//------------- convert data from binary to base16 and vice verse ------------//

uint hexString_toHex(const byte* srcBin, uint srcLen, char* dstHex, uint dstLen)
{
    uint end = (dstLen/2)+(dstLen%2);
    end = srcLen < end ? srcLen : end;
    for (uint i=0, I=0; i < end; i++, I+=2 ) {
        dstHex[I]   = xehxeh[srcBin[i]/16];
        dstHex[I+1] = xehxeh[srcBin[i]%16];
    } dstHex[end+=end] = '\0';
    return end;
}

uint hexString_toBin(const char* srcHex, uint* srcLen, byte* dstBin, uint dstLen)
{
    int srcLength = srcLen? *srcLen : strlen(srcHex);
    int i = -1; uint c = 0;
    int n = 0; char oneByte[2];
    while( (++i<srcLength) && (c<dstLen) ) {
        if( (oneByte[n]=hexhex[(byte)srcHex[i]]) >= 0 )
            if(++n==2)
                dstBin[c++] = oneByte[n=0]*0x10
                            + oneByte[1];
    } if(srcLen)
        *srcLen = i;
    return c;
}


char* hexString_int64ToHex(ulong dat) {
#ifdef using_commandLiner
	hexString_toHex((byte*)&dat,8u,getTemp(),16u);
	return getTemp();
#else
	hexString_toHex((byte*)&dat,8u,pool_setc(0,16),16u);
	return pool_get();
#endif
}

ulong hexString_hexToInt64(const char* sixteen) {
	char data[9]; uint len=16;
	hexString_toBin(sixteen,&len,&data[0],8);
	return *(ulong*)&data[0];
}

char* hexString_int32ToHex(uint dat) {
#ifdef using_commandLiner
	hexString_toHex((byte*)&dat,4u,getTemp(),8u);
	return getTemp();
#else
	hexString_toHex((byte*)&dat,4u,pool_setc(0,8),8u);
	return pool_get();
#endif
}

uint hexString_hexToInt32(const char* aight) {
	ulong data; uint len=8;
	hexString_toBin(aight,&len,(byte*)&data,4u);
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

#ifdef hexString

#define mod(cnt) ((cnt-8) % MAX_NAM_LEN)
#define pos(cnt) (cnt % MAX_NAM_LEN)
#define end(hex) (*(ulong*)&incoming[hex]==t)

int hexString_setBufferSize(cmLn size)
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

int main(int argc,char**argv)
{
    if(! InitCommandLiner(argc,argv)
      || hasOption('h') )
        exit(USAGE(hexString));
		
	pool_InitializeCycle();
#if DEBUG
	pool_PrintStatistics();
#endif	

    if( hasOption('b') ) {
        if ( isSwitch('b') )
            switchIdx('b', indexOfName(rawNext('b')) );
        if ( !hexString_setBufferSize(getName('b')) )
            ExitOnError("ARGUMENT");
        else
            printf("\n set hexString buffersize to: %s\n\n",getName('b'));
        exit(EXIT_SUCCESS);
    }
	
	if(hasOption('x')) {
		if(!exists('x')) ExitOnError("Parameter");
		printf( hexString_fromNum( getName('x') ) );
		exit(CheckForError());
	}
	if(hasOption('n')) {
		if(!exists('n')) ExitOnError("Parameter");
		printf( hexString_toNum( getName('n') ) );
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
					fwrite( hexString_fromBin( &buffer[0], currentchunk ),1,(currentchunk*2),o);
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
        } ExitOnError("BUFFER");
        printf("\n\n");
        exit(CheckForError());
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
        exit(CheckForError());
    }
}//end
#endif
