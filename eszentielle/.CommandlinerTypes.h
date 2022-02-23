#ifndef COMMANDLINER_ESCENTIALS_DEFINED

#include <stdint.h>

#ifdef ulong
#undef ulong
#endif

#if defined(__TINYC__)
   typedef uint8_t              byte;
   typedef uint16_t             word;
   typedef uint32_t             uint;
   typedef int64_t              slong;
   typedef uint64_t             ulong;
   typedef __SIZE_TYPE__        ptval;
   typedef __PTRDIFF_TYPE__     ptdif;
#elif defined(_MSC_BUILD)
   typedef unsigned __int8      byte;
   typedef unsigned __int16     word;
   typedef unsigned __int32     uint;
   typedef signed   __int64     slong;
#ifndef QT_VERSION
   typedef unsigned long long   ulong;
#endif

#ifdef _WIN64
   typedef unsigned __int64     ptval;
   typedef signed   __int64     ptdif;
#elif _WIN32
   typedef unsigned __int32     ptval;
   typedef signed   __int32     ptdif;
#else
#define                         ptval size_t
#define                         ptdif intptr_t
#endif
#else //  GCC or MINGW
   typedef unsigned char        byte;
   typedef unsigned short       word;
   typedef unsigned int         uint;
   typedef signed long long int slong;
   typedef unsigned long long   ulong;
   typedef uintptr_t            ptval;
   typedef intptr_t             ptdif;
#endif

   typedef const char*          cmLn;
   typedef short                cmOp;
   typedef int                  cmIx;
   typedef int                  cmBl;

#define COMMANDLINER_ESCENTIALS_DEFINED (1)
#endif
