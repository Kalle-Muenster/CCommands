/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      crypt64.h                                  ||
||     Author:    Kalle                                      ||
||     Generated: 17.10.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_crypt64
#define IncludesFor_crypt64 (1)

#include <stdlib.h>

#include "./eszentielle/.CommandLinerTypes.h"


#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#define CRYPS64_API __declspec(dllexport) 
#else
#define CRYPS64_API
#endif

#if defined(NOT_USE_ENVIRONMENTOR) || (!defined(__TINYC__))
#define WITH_STRGPOOL
#include ".base64.h"
#elif defined(__cplusplus) && (!defined(_OnTheFly_))
#include ".base64.h"
extern "C" {
#else
#include ".environMentor.h"
#ifndef BASE64_ENABLE_STREAMING_API
#define BASE64_ENABLE_STREAMING_API (1)
#endif
#include ".base64.h"
#endif


#define CRYPT64_ENCRYPTED_SIZE(size) BASE64_ENCODED_SIZE(size)+16u
#define CRYPT64_DECRYPTED_SIZE(size) BASE64_DECODED_SIZE(size-16u)

	typedef b64Frame k64Chunk;

    typedef struct CRYPS64_API K64 {
        const char* table;
        union {
            ulong value;
            char  ccode[8];
        } pass;
        b64Frame b64cc[21];
    } K64;

    typedef struct CRYPS64_API k64Stream {
        b64Stream   b64;
        K64*        key;
        const byte* val;
		const char* enc;
		const char* dec;
    } k64Stream, K64F;

	typedef enum CRYPS64_API CRYPS64 {
		CRYPST = '?',
		BASE64 = 0x40u, //FOURCC('b','6','4','\0'), // try to see if possible if changing value to '0x40' (B64_FLAG) is possible without causing any evil side effects
		BINARY = 0x10u, //FOURCC('b','i','n','\0'), // try to see if possible if changing value to '0x10' (BIN_FLAG) is possible without causing any evil side effects
		CONTXT_ERROR = FOURCC('c','t','x','\0'),
		FORMAT_ERROR = FOURCC('f','m','t','\0'),
		TABLES_ERROR = FOURCC('t','b','l','\0'),
		STREAM_ERROR = FOURCC('d','i','r','\0'),
		OUTPUT_ERROR = FOURCC('d','s','t','\0'),
		INPUTS_ERROR = FOURCC('s','r','c','\0'),
		PHRASE_ERROR = FOURCC('k','e','y','\0')
	} CRYPS64;

#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <crypt64.h> 
#else

   CRYPS64_API void     crypt64_Initialize( bool init );

   CRYPS64_API K64F*    crypt64_createFileStream( const char* pass, const char* path, const char* mode );
   CRYPS64_API uint     crypt64_sread( byte* dst, uint size, uint count, K64F* cryps );
   CRYPS64_API uint     crypt64_swrite( const byte* src, uint size, uint count, K64F* cryps );
   CRYPS64_API k64Chunk crypt64_getYps( K64F* vonDa );
   CRYPS64_API uint     crypt64_putYps( k64Chunk dieses, K64F* nachDa );
   CRYPS64_API int      crypt64_frameSize( K64F* stream );

   CRYPS64_API K64* crypt64_allocateNewKey(void);
   CRYPS64_API K64* crypt64_initializeKey(K64* key,ulong value);
   CRYPS64_API K64* crypt64_createKeyFromPass(const char* passphrase);
   CRYPS64_API K64* crypt64_createKeyFromHash(ulong hash);
   CRYPS64_API ulong crypt64_calculateHashValue( const byte* data, int size );
   CRYPS64_API void crypt64_invalidateKey(K64* key);
   CRYPS64_API int  crypt64_isValidKey(K64* key);

   CRYPS64_API int  crypt64_prepareContext( K64* key, byte mod );
   CRYPS64_API int  crypt64_releaseContext( K64* key );

   CRYPS64_API uint crypt64_encrypt(K64* key64, const byte* data, uint  size, char* dest);
   CRYPS64_API uint crypt64_decrypt(K64* key64, const char* data, byte* dest);
   CRYPS64_API uint crypt64_encryptFile(K64* key64, const char* src, const char* dst);
   CRYPS64_API uint crypt64_decryptFile(K64* key64, const char* src, const char* dst);
   CRYPS64_API uint crypt64_decryptStdIn(K64*, FILE* destination);
   CRYPS64_API uint crypt64_encryptStdIn(K64*, FILE* destination);
   CRYPS64_API k64Chunk crypt64_encryptFrame(K64* key64, k64Chunk threeByte);
   CRYPS64_API k64Chunk crypt64_decryptFrame(K64* key64, k64Chunk fourChars);

   CRYPS64_API uint crypt64_binary_encrypt(K64* key, const byte* data, uint size, byte* dest);
   CRYPS64_API uint crypt64_binary_decrypt(K64* key, const byte* data, uint size, byte* dest);
   CRYPS64_API uint crypt64_binary_encryptFile(K64* key64, const char* src, const char* dst);
   CRYPS64_API uint crypt64_binary_decryptFile(K64* key64, const char* src, const char* dst);
   CRYPS64_API uint crypt64_binary_decryptStdIn(K64*, FILE* destinatin);
   CRYPS64_API uint crypt64_binary_encryptStdIn(K64*, FILE* destinatin);
   CRYPS64_API k64Chunk crypt64_binary_encryptFrame(K64* key64, k64Chunk threeByte);
   CRYPS64_API k64Chunk crypt64_binary_decryptFrame(K64* key64, k64Chunk threeByte);

#endif
#ifdef _OnTheFly_
#include ".crypt64.cc"
#elif defined(__cplusplus)
}
#endif
#endif
