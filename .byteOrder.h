/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      byteOrder.h                                ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_byteOrder
#define IncludesFor_byteOrder (1)

#include <string.h>
#include ".CheckForProjectSettings.h"

#ifdef _OnTheFly_
#undef  ARGUM_SLASHING
#undef  QUOTE_ENDOUBLING
#define ARGUM_SLASHING    (0)
#define QUOTE_ENDOUBLING  (0)
#if defined( NO_SPACE_CHECKING )
#if NO_SPACE_CHECKING != 1
#undef  NO_SPACE_CHECKING
#define NO_SPACE_CHECKING (1)
#endif
#endif
#elif defined(__cplusplus) && !defined(_OnTheFly_)
extern "C" {
#endif

#ifndef COMMANDLINER_ESCENTIALS_DEFINED

typedef unsigned char      byte;
typedef unsigned int       uint;
typedef unsigned long long ulong;
typedef float              float32;
typedef double             float64;

#define COMMANDLINER_ESCENTIALS_DEFINED (1)
#endif

 #ifndef QWORD
 #define QWORD unsigned long long int
 #endif
 #ifndef DWORD
 #define DWORD unsigned int
 #endif
 #ifndef WORD
 #define WORD unsigned short int
 #endif
 #ifndef FLOAT32
 #define FLOAT32 float
 #endif
 #ifndef FLOAT64
 #define FLOAT64 double
 #endif

#if defined( __x86_64__) || defined(_WIN64)
 #define WORD_BYTESIZE 8
 #define WORD_TYPENAME "QWORD"
 #define WORD_SIZETYPE QWORD
 #define PRECISON_TYPE FLOAT64
#elif defined(_M_IX86) || defined(__x86__)
 #define WORD_BYTESIZE 4
 #define WORD_TYPENAME "DWORD"
 #define WORD_SIZETYPE DWORD
 #define PRECISON_TYPE FLOAT32
#endif


#define ENDIAN_IS_BIG___ (1414743380u == ('T'|'E'<<8|'S'<<16|'T'<<24))
#define ENDIAN_IS_LITTLE (1413829460u == ('T'|'E'<<8|'S'<<16|'T'<<24))

#if ENDIAN_IS_BIG___
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN (1)
#define __LITTLE_ENDIAN (0)
#define   BYTE_ORDER_NAME "BIG ENDIAN"
#endif
#else
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN (0)
#define __LITTLE_ENDIAN (1)
#define   BYTE_ORDER_NAME "LITTLE ENDIAN"
#endif
#endif

#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_FOURCC_API )
#include <byteOrder.h>
#else
#define FOURCC_API


typedef DWORD FOURCC_API fourCC;
typedef QWORD FOURCC_API longCC;


FOURCC_API int           IS_BIG_ENDIAN(void);
FOURCC_API const char*   byteOrder_fourCCtoString(fourCC fourCCval);
#define                  byteOrder_stringTOfourCC(strchrPt) (*(fourCC*)strchrPt)
FOURCC_API const char*   byteOrder_longCCtoString(longCC longCCval);
#define                  byteOrder_stringTOlongCC(strchrPt) (*(longCC*)strchrPt)
FOURCC_API WORD_SIZETYPE byteOrder_reverse(WORD_SIZETYPE input);
FOURCC_API fourCC        byteOrder_reverse32(fourCC input);
FOURCC_API longCC        byteOrder_reverse64(longCC input);
FOURCC_API byte*         byteOrder_reverseData(void* data,unsigned cbData);
FOURCC_API byte*         byteOrder_reverseData32(void* data,unsigned cbData);
FOURCC_API byte*         byteOrder_reverseData64(void* data,unsigned cbData);
FOURCC_API char*         byteOrder_resverseString(char* inputString);
FOURCC_API char*         byteOrder_resverseString32(char* inputString);
FOURCC_API char*         byteOrder_resverseString64(char* inputString);

#if WORD_BYTESIZE >= 8
#define SIZECC longCC
#define byteOrder_sizeCCtoString(arg) byteOrder_longCCtoString(arg)
#define byteOrder_stringTOsizeCC(arg) byteOrder_stringTOlongCC(arg)
#else
#define SIZECC fourCC
#define byteOrder_sizeCCtoString(arg) byteOrder_fourCCtoString(arg)
#define byteOrder_stringTOsizeCC(arg) byteOrder_stringTOfourCC(arg)
#endif

#endif

#ifdef _OnTheFly_
 #define WITH_STRGPOOL
 #include ".byteOrder.cc"
#elif defined(__cplusplus)
 }
#endif
#endif
