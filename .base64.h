#ifndef IncludesFor_base64
#define IncludesFor_base64 (1)

#include <stdlib.h>
#include <stdio.h>



#if (!defined(NOT_USE_ENVIRONMENTOR)) && defined(__TINYC__) 
#include ".environMentor.h"
#if WINDOWS
#include <io.h>
#endif
#else
#ifndef WITH_JUNKYARD
#define WITH_JUNKYARD
#endif
#ifndef WITH_STRGPOOL
#define WITH_STRGPOOL
#endif
#include ".commandliner.h"
#if WINDOWS
#include <io.h>
#endif
#if defined(__cplusplus) && (!defined(_OnTheFly_))
extern "C" {
#endif
#endif

#ifndef BASE64_WITH_LINEBREAKS
#define BASE64_WITH_LINEBREAKS (1)
#endif

#if     BASE64_WITH_LINEBREAKS > 0
#define BASE64_COMPRESSIONRATE  (4.0/3.0)*(65.0/64.0)
#else
#define BASE64_COMPRESSIONRATE  (4.0/3.0)
#endif
#define BASE64_ENCODED_SIZE(size) (uint)((size*BASE64_COMPRESSIONRATE)+2.5)
#define BASE64_DECODED_SIZE(size) (uint)(0.5+(size-2.0)/BASE64_COMPRESSIONRATE) 
#ifndef BASE64_VERFAHREN
#define BASE64_VERFAHREN 1
#endif

#define EXPERIMENTAL 0

#ifndef BASE64_API
#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_BASE64_API )
#define BASE64_API __declspec(dllexport)
#else
#define BASE64_API
#endif
#endif

#ifndef BASE64_ENABLE_STREAMING_API
#define BASE64_ENABLE_STREAMING_API (1)
#endif 


#define uInt(fromChars)    (*(unsigned*)fromChars)
#define asFrame(fromChars) (*(b64Frame*)fromChars)


#if     BASE64_ENABLE_STREAMING_API
#include ".b64streams.h"
#else
typedef union BASE64_API b64Frame {
    uint  u32;
    byte  u8[4];
    char  i8[4];
} b64Frame;
typedef b64Frame BASE64_API const B64Nuller;
#endif

#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <base64.h>
#else

BASE64_API void        base64_Initialize(void);
BASE64_API b64Frame    base64_encodeFrame( b64Frame threeChars );
BASE64_API b64Frame    base64_decodeFrame( b64Frame fourChars ); //returns 3 bytes bin data + 0 or +!=0 on decoding errors (4th byte then points actual bad input byte)
BASE64_API int         base64_encodeData(char* dst, const byte* src, unsigned cbSrc); //encode binary data of cbSrc length
BASE64_API int         base64_decodeData(byte* dst, const char* src); //decode base64 data (at best terminated by equal sign)

BASE64_API const char* base64_encode(const byte* data, uint size);
BASE64_API const byte* base64_decode(const char* encd, uint* size);

// encode content of file dst to file src
BASE64_API int         base64_encodeFile(const char* dst, const char* src);

// decode base64 content from file src to file dst
BASE64_API int         base64_decodeFile(const char* dst, const char* src);

// encode content of file dst to file src
BASE64_API int         base64_encodeFileToFile(FILE* dst, FILE* src);

// decode base64 content from file src to file dst
BASE64_API int         base64_decodeFileToFile(FILE* dst, FILE* src);

BASE64_API int         base64_encodeFromFile(const char* fileName, int* out_len);
BASE64_API int         base64_decodeFromFile(const char* fileName, int* out_len);


// get the regular, base64 standard coding table 
BASE64_API const char* base64_b64Table(void);

// get actually in use coding table loaded 
BASE64_API const char* base64_getTable(void);

// set a base64 table to be used for following operation (table won't be stored persistent)
BASE64_API const char* base64_setTable(const char* tableOrFile);

// does a check if a passed table matches the actual configured 'Valid Table' conditions.
// If the table indeed IS valid, it then sets up that table as regularly used 'CodingTable'
// for any further de- or en- coding operations.
// If the passed table is NOT valid, it doesn't set up that table and generates an error:
// 'invalid table' by returning a filtered (but so too short) version of the table which
// has any invalid characters being removed from it.
BASE64_API const char* base64_setTable_Checked( const char* newTable );

// Set up and reconfigure for using a different CodingTable.
// parameter can be:
// - String, consisting from 64 unique characters, terminated by a '=' sign as 65th carracter
// - Filename, pointing a file which contains 64 unique caracters + terminating '=' sign
// - NULL/stdin, will make the programm waiting for available input on the stdin stream
BASE64_API const char* base64_newTable(const char* TableData_Or_FileName_Or_stdin);

#endif

// below functions only may be supported for c 'script' thats executed by an 'interpreter' or jit compiler
// these functions provide API for reconfiguration of script code sections via changing preprocessor code (macros)
// and so won't apply when using compiling bas64.cc source by using a regular c 'compiler' to a binary image.
#if (!defined(NOT_USE_ENVIRONMENTOR)) && defined(__TINYC__)
const char* base64_pushTable(const char* newTable);
const char* base64_popTable(void);
void        base64_setBufferSize(int);
// enables or disables including 'byteOrder' and compiling it's source on the fly
void        base64_toggleReverseByteorder(const char* on_or_off_or_NULL);
void        base64_toggleWriteLinebrakes(const char* on_or_off_or_NULL);
#endif

#ifdef _OnTheFly_
 #include ".base64.cc"
#elif defined(__cplusplus)
}
#endif

#endif