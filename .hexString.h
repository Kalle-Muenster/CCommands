/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      hexString.h                                ||
||     Author:    Kalle Muenster                             ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_hexString
#define IncludesFor_hexString (1)

#ifndef COMMANDLINER_ESCENTIALS_DEFINED
#include "./eszentielle/.CommandLinerTypes.h"
#endif

#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_HEXHEX_API )
#define HEXHEX_API __declspec(dllexport) 
#endif

#if defined(__cplusplus) && !defined(_OnTheFly_)
extern "C" {
#endif

#ifndef HEXHEX_API
#define HEXHEX_API
#endif

#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <hexString.h>
#else

HEXHEX_API void  hexString_Initialize( void );

// convert decimal numbers to hexadecimal number
HEXHEX_API char* hexString_fromNum(const char* num);
// convert hexadecimal numbers to decimal number
HEXHEX_API char* hexString_toNum(const char* hex);


// data conversion:  binary <-> base16

HEXHEX_API uint  hexString_toBin(const char* srcHex, uint* srcLen, byte* dstBin, uint dstLen);
HEXHEX_API uint  hexString_toHex(const byte* srcBin, uint srcLen, char* dstHex, uint dstLen);

HEXHEX_API byte* hexString_fromHex(const char*,uint*);
HEXHEX_API char* hexString_fromBin(const byte*,uint);

HEXHEX_API char* hexString_int64ToHex(ulong dat);
HEXHEX_API ulong hexString_hexToInt64(const char*);

HEXHEX_API char* hexString_int32ToHex(uint dat);
HEXHEX_API uint  hexString_hexToInt32(const char*);

#endif


#ifdef _OnTheFly_
 #include ".hexString.cc"
#elif defined(__cplusplus)
 }
#endif
#endif
