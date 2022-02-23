/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      CommandlinerTests.cc                       ||
||     Author:    Kalle                                      ||
||     Generated: 18.08.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".CommandlinerTests.h"
#else
  #include <.commandLiner.h>

  int USAGE(CommandlinerTests)
  {
    Synopsis("<category>");
    printf("testable categories:\n\n\
   --Size : Struct sizes and alignments\n\
   --Arch : Architecture and Wordsizes\n\
      -i  :  - integer types\n\
      -f  :  - float types\n\
   --Char : Chars and Strigs...\n\
      64  :  - Base64\n\
      64s :  - Base64 Stream api\n\
   --All  : All categories\n\n");
    return CheckForError()
        || !hasOption('h');
  }
#endif

// regular includes here:
#include <stdlib.h>
#include <float.h>
#include <stdint.h>

void tests_printStructSizes(void)
{
    printf("Size of the CommanderState struct:\n\
    evaluated by macro 'ComStateStructSize': %i\n\
    evaluated by keyword 'sizeof()': %i\n\
    size of actual 'c' instance: %i\n\n",
        ComStateStructSize,
        sizeof(CommanderState),
        sizeof(c)
    );

    PushCommandLine( RebuildCommandLine().cmln );
    printf("Pushed a new CommanderState instance...\n\
    evaluated size by 'sizeof(pushedinst)': %i\n\n",
        sizeof(*c.running->running)
    );

    PopCommandLine(true);

    ExitOnError("Push/Pop");
}

void tests_architectureTest(int which)
{
    if(which&0x01) {
    unsigned short int iSrt = UINT16_MAX;
    printf("unsigned short:\n  size: %i\n   max: %u\n\n",
            sizeof(iSrt), iSrt );
    unsigned int iReg = UINT_MAX;
    printf("unsigned int:\n  size: %i\n   max: %u\n\n",
            sizeof(iReg), iReg );
    unsigned long int iLong = ULONG_MAX;
    printf("unsigned long int:\n  size: %i\n   max: %lu\n\n",
            sizeof(iLong), iLong );
    unsigned long long int i64 = UINT64_MAX;
    printf("unsigned long long int:\n  size: %i\n   max: %llu\n\n",
            sizeof(i64), i64 );
    }
    if(which&0x02) {

    float fFlt = FLT_EPSILON;
    printf("float = FLT_EPSILON:\n  size: %i\n epsilon: %0.24f\n epsilon-e: %.18e\n",
            sizeof(fFlt), fFlt, fFlt );
	PrintValues(FLT,"f");
	
    double fDbl = DBL_EPSILON;
    printf("double = DBL_EPSILON:\n  size: %i\n epsilon: %0.53lf\n epsilon-e: %.18e\n",
            sizeof(fDbl), fDbl, fDbl );
	PrintValues(DBL,"lf");
	
    long double fLdbl = LDBL_EPSILON;
    printf("long double = LDBL_EPSILON:\n  size: %i\n epsilon: %0.53Lf\n epsilon-e: %.18le\n\n",
            sizeof(fLdbl), fLdbl, fLdbl );
	PrintValues(LDBL,"Lf");
    }
}

int tests_stringCompareTest()
{
    int failcount = 0;
    int result = stringCompare("Dies","Das");
    printf("\nTested comparision of 'Dies' == 'Das'\n   expected: 2\n   result: %i\n",result);
    printf((failcount+=(result != 2))?"FAIL\n":"PASS\n");
    result = stringCompare("Dies","Dies");
    printf("\nTested comparision of 'Dies' == 'Dies'\n   expected: 0\n   result: %i\n",result);
    printf((failcount+=(result != 0 ))?"FAIL\n":"PASS\n");
    result = stringCompare("Dies","Diese");
    printf("\nTested comparision of 'Dies' == 'Diese'\n   expected: 5\n   result: %i\n",result);
    printf((failcount+=(result != 5))?"FAIL\n":"PASS\n");
    return failcount;
}

void tests_base64Conversion(int what)
{
    base64_Initialize();
    base64_newTable( getFeatured("BASE64_CODING_TABLE").value.string );
    const char* b64Table = base64_getTable();
    if( what<'s' ) {
         // base64_api
        printf("\nbase64_API: no tests yet...\n");
        if( what=='S' ) {
            what='s';
        }// base64_stream_api
    } if( what=='s' ) {
        printf("\nbase64_stream_API tests:\n");

        // b64 encode during file read
        uint* size = NULL;
        char encodedLine[65];
        encodedLine[64] = '\0';
        printf("'stream encode' file stream by reading plain text file: '.hallo.cc'\n");
        printf("active_table: %s\n\n",b64Table);
        pool_setf("%s/.hallo.cc",getPathOfTheCommander());
        b64File* b64f = base64_createFileStream( pool_get(), "re" );
        int fram = 0;
        pool_push();
        size = pool_setCheckpoint();
        while( fram = base64_sread( &encodedLine[0], 1, 64, b64f ) ) {
            encodedLine[fram]='\0';
            printf( "%s", pool_setf("%s\n",&encodedLine[0]) );
        } //base64_destream( &b64f );
        printf( "...done! encoded base64 data to memory\n\n" );
        printf("now will read decoded plain text from that base64 data via 'base64 data decoding stream':\n");
        printf("active_table: %s\n\n",b64Table);
        b64Stream* b64s = base64_createDataStream( pool_collectCheckpoint(), EMPTY, "rd" );
        fram = 0;
        while( fram = base64_sread( &encodedLine[0], 3, 20, b64s ) ) {
            fwrite( &encodedLine[0], 3, fram, stdout );
        } //base64_destream( &b64s );
        printf( "...done!\n\n" );
        pool_pop();
    }
    /*
b64Stream   base64_createDataStream( void* src_dat, uint src_len, const char* mode );
b64Stream   base64_createFileStream( const char* fnam, const char* mode ); // e.g. fopen like syntax
b64Stream   base64_createPoolStream( const char* mode ); // create a stream strait into/outoff the pool
void        base64_destream( b64Stream* ); // frees a b64Stream
uint        base64_streamBytesRemaining( b64Stream* ); // bytes left till eos
int         base64_canStream( b64Stream* ); // is everything valid?, and.. are stream bytes available?
uint        base64_sread( void* dst, uint size, uint count, b64Stream* ); // read data from b64Stream into buffer 'dst' while either de or en-coding the stream
uint        base64_swrite( void* src, uint size, uint count, b64Stream* ); // write data 'src' into the de- or en-coding stream
b64Frame    base64_getFrame( b64Stream* ); // e.g. getch like function but gets an b64Frame instead of a char
int         base64_setFrame( b64Stream*, b64Frame ); /

b64Frame    base64_encodeFrame(b64Frame threeByte); //returns 4 chars b64 data
b64Frame    base64_decodeFrame(b64Frame fourChars); //returns 3 bytes bin data + 0 or +!=0 on decoding errors (4th byte then points actual bad input byte)
int         base64_encodeText(char* dst,char* src);
int         base64_encodeData(char* dst,const byte* src,unsigned cbSrc);
int         base64_decodeBase64(byte* dst,char* src);
int         base64_encodeFile(const char* dst,const char* src); // encode content of file dst to file src
int         base64_decodeFile(const char* dst,const char* src); // decode base64 content from file src to file dst
int         base64_encodeFromFile(const char* fileName, int* out_len);
int         base64_decodeFromFile(const char* fileName, int* out_len);
    */
}

int tests_crypsingKeyTests( ulong val, ulong end, int print )
{
    if ( val < end ) {
        int matches = 0;
        K64* testling = crypt64_allocateNewKey();
        char comparse[65] = {0};
        crypt64_initializeKey( testling, val );
        memcpy( &comparse[0], K64_table( testling ), 64 );
        if ( print ) printf( "generationg %i keys... first key (comparse) is:\n    %s", end - val, &comparse[0] );
        while ( ++val != end ) {
            if ( !crypt64_initializeKey( testling, val ) )
                ExitOnError( "Keying" );
            const char* nextkey = K64_table( testling );
            if ( print ) printf( "\n%s", nextkey );
            if ( !stringCompare( &comparse[0], nextkey ) ) {
                if ( print ) {
                    printf( "  <-  %i identical tables generated", ++matches );
                } else ++matches;
            }
        } return matches;
    }
}

#ifdef CommandlinerTests
int main(int argc,char**argv)
{
    if( !InitCommandLiner(argc,argv) ) {
        exit(USAGE(CommandlinerTests));
    }
    int ALL = isModus("All");
    if( isModus("Arch") || ALL ) {
        setSwitch('i');
        setSwitch('f');
    } int NUM = ( isSwitch('i') ? 1 : 0 )
              + ( isSwitch('f') ? 2 : 0 );
    int B64 = false;
    if( isModus("Char") || ALL ) {
        B64 = stringCompare(rawNext('C'),"64");
        B64 = B64==3 ? 'S' : B64==1 ? 0 : 's';
    }
    printf("\n");

    if( isModus("Size") || ALL ) {
        tests_printStructSizes();
    }
    if( NUM ) {
        tests_architectureTest(NUM);
    } int fails = 0;
    if( isModus("Char") || ALL ) {
        fails += tests_stringCompareTest();
        if( B64 )
            tests_base64Conversion(B64);
    }
    if ( isModus("Keys") || ALL ) {
        srand( (int)&main );
        const ulong start = rand() | ((ulong)rand() << 16) | ((ulong)rand() << 32) | ((ulong)rand() << 48);
        const ulong end = start + 1000;
        int matches = tests_crypsingKeyTests(start,end,true);
        if ( matches ) printf( "\n There have been %i identical keys found in range %llu to %llu\n", matches, start, end );
        else printf( "\n There no identical key was found in range %llu to %llu\n", start, end );
    }

	PrintCompiler();
    exit(fails);
}

#endif
