#ifndef using_b64Streams
#define using_b64Streams


#ifndef   COMMANDLINER_ESCENTIALS_DEFINED
 #include "./eszentielle/.CommandLinerTypes.h"
#endif

#ifndef   BASE64_API
 #if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_BASE64_API )
  #define BASE64_API __declspec(dllexport)
 #else
  #define BASE64_API
 #endif
#endif

typedef union BASE64_API b64Frame {
    uint  u32;
    byte  u8[4];
    char  i8[4];
} b64Frame;

typedef b64Frame BASE64_API const B64Nuller;

 // flg[0]: byte per frame in streamdirection
 // flg[1]: type of stream (DATA, FILE, POOL)
 // flg[2]: direction of stream ('w' or 'r')
 // flg[3]: byte per frame against direction

typedef enum BASE64_API b64StreamMode {
    ENCODE = 3,   // flg[0]
    DECODE = 4,   // flg[0]
    DATASTREAM = 'D', // flg[1]
    POOLSTREAM = 'P', // flg[1]
    FILESTREAM = 'F', // flg[1]
    STDSTREAM  = 'G', // flg[1]
    INPUT  = 'r', // flg[2]
    OUTPUT = 'w', // flg[2]
    READ_ENCODED = 7496960,
    READ_DECODED = 7496704,
    WRITE_ENCODE = 7824640,
    WRITE_DECODE = 7824384,
} b64StreamMode;


typedef struct BASE64_API b64Stream b64Stream;
typedef struct BASE64_API b64Stream {
    ptval      pos;
    b64Frame*  buf;
    void*      dat;
    ptval      len;
    b64Frame (*nxt)(void*);
    int      (*set)(b64Stream*, b64Frame);
    b64Frame*(*get)(b64Stream*);
    byte       flg[4];
} b64Stream, B64S;

typedef struct BASE64_API b64File {
    ptval      pos;
    ptval      buf;
    void*      dat;
    ptval      len;
    b64Frame (*nxt)(void*);
    int      (*set)(b64Stream*, b64Frame);
    b64Frame*(*get)(b64Stream*);
    byte       flg[4];
} b64File, B64F;

typedef struct BASE64_API StringPool StringPool;
typedef StringPool BASE64_API POOL;
typedef struct BASE64_API b64Pool {
    uint*      pos;
    b64Frame*  buf;
    POOL*      dat;
    uint*      len;
    b64Frame (*nxt)(void*);
    int      (*set)(b64Stream*, b64Frame);
    b64Frame*(*get)(b64Stream*);
    byte       flg[4];
} b64Pool;

typedef b64File BASE64_API B64F;

#if     BASE64_WITH_LINEBREAKS > 0
#define BASE64_STREAM_RATE(b64strm) \
    ((float)b64strm->flg[3]/(float)b64strm->flg[0]) \
  * ((float)(61+b64strm->flg[3])/(float)(61+b64strm->flg[0]))
#else
#define BASE64_STREAM_RATE(b64strm) ((float)b64strm->flg[3]/(float)b64strm->flg[0])
#endif

// just for internal usage:
BASE64_API uint        base64_initNewB64StreamStruct( const char* mode, b64Stream* stream );
BASE64_API int         base64_initB64FileStreamStruct( b64File* stream, const char* file, uint mode );


// create FILE* like 'de' or 'en' coding streams from either given, already allocated
// buffers or filles to be opened for either reading or writing via passing filenames.

// parameter (uint) src_len:
// - pass count on bytes planed to be streamed (..if exact size already is known when creating the stream)
// - pass EMPTY (if size of data which is planed to be streamed is NOT known when creating the stream).
// parameter (const char*) mode:
// expects a string consiting from a combination of following chars: 'w', 'r', 'd', 'e'
// "wd" or "dw": write/decode - means opening a stream for writing base64 encoded data to it,
//                              the data will arive stream destination as decoded binary data.
//                            - in case of a 'Data' stream, target buffer receives decoded binary data.
//                            - in case of a 'File' stream, file opened for writing stores binary data.
// "we" or "ew": write/encode - means opening a stream for writing binary data to it.
//                              stream destination receives a string, base64 encoded.
// "rd"/"dr" and "re"/"er"    - are equivalent but but vice versa 'reading' data de- or en- coded.
// 're'/'er'                  - for reading binary content - from files or buffers containing base64 string
// 'rd'/'dr'                  - for reading a base64 encoded string from either binary file or data buffers
//
//  short: reading ('r') or writing ('w') data from or to streams while at same time ('d') decoding or ('e') encoding
//

// create a memory stream in or out of a buffer.
BASE64_API b64Stream*  base64_createDataStream( void* src_dat, uint src_len, const char* mode );

// almost same like fopen
BASE64_API b64File*    base64_createFileStream( const char* fnam, const char* mode );

// create a stream into/outoff a pool
BASE64_API b64Pool*    base64_createPoolStream( const char* mode );

// bytes left till eos (if at least the stream provides such information,.. otherwise EMPTY is returned)
BASE64_API uint        base64_streamBytesRemaining( B64S* );

// is everything valid?, and.. are stream bytes available?
BASE64_API int         base64_canStream( B64S* );

// how many bytes measures one frame of data streamed in 'direction' through given 'stream' (returns either 3 or 4 byte)
BASE64_API int         base64_frameSize( char direction, B64S* stream );

// get a preinitialized B64Frame to be used for terminating transmissions through encoding streams
// (e.g. let stream recipients know about its safe closing the stream due to no data will follow
BASE64_API B64Nuller   base64_Nuller(void);

// check if some just received portion (b64Frame) is signaling EOS (end of stream)
BASE64_API int         base64_isEndFrame( b64Frame frame, B64S* );

// close a base64 stream and free any resources B64S stream pointed structure used
BASE64_API void        base64_destream( B64S* );

// like getch() but reads b64Frames instead of chars
BASE64_API b64Frame    base64_getFrame( B64S* );

// like putch() but writes b64Frames (either 3/4 bytes) instead of chars
BASE64_API int         base64_putFrame( B64S*, b64Frame );

// peaks the next frame which will be 'overwritten' in a wb+/rb+ stream  
BASE64_API b64Frame    base64_peakWrite( B64S* );

// read data from b64Stream into buffer 'dst' while either de or en-coding the stream
BASE64_API uint        base64_sread( byte* dst, uint size, uint count, B64S* );

// write data 'src' into the stream while de or en coding it
BASE64_API uint        base64_swrite( const byte* src, uint size, uint count, B64S* );


#endif
