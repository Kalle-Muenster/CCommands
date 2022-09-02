#ifndef _OnTheFly_
 #include ".CheckForProjectSettings.h"
 #include ".commandLiner.h"
 #include ".junkYard.h"
 #include ".stringPool.h"
 #include ".byteOrder.h"
 #include ".b64streams.h"
 #include ".base64.h"
#endif

/////////////////////////////////////////////////////////////////
// b64Stream API
//  B64S base64_createDataStream(void* dat_buf,uint dat_len,const char* mode) // mode : wd|we|rd|re -> read or write de or encoded
//  B64F base64_createFileStream(const char* filename,const char* openmode) -> wd|we|rd|re
//  B64P base64_createPoolStream(const char* mode) // creates stream writing in or reading from stringPool
//  base64_getFrame(B64S*) -> getch one single frame from a B64 input stream (would be 3byte or 4byte regarding mode);
//  base64_putFrame(B64S*) -> put a single frame into the given B64Stream (if supported.. FILE opened for reading won't.., POOL would fetch a slice to be written.. or even push a new instance if recycling disabled)
//  base64_sread( void*dst,uint Tsiz,uint cntTs, B64S* istr ) // like fread(d,s,c,f) but takes B64S* instead of FILE* f
//  base64_swrite( void*src,uint Tsiz,uint cntTs, B64S* ostr ) // like fwrite(s,s,c,f) but takes B64S* instead of FILE* f
//  base64_destream( B64S* ) -> clean up a stream (e.g. depending on type.. FILE may flush/close, POOL will pop, DATA ...does nothin )

const b64Frame nullFrame = { 0 };

int removeLBs(char* text)
{
    int lb = 0;
    char* pt = text;
    while (*pt = *(pt + lb)) {
        if (*pt == '\n') ++lb;
        else ++pt;
    } return (int)(pt - text);
}


////////////////////////////////////////////////////////////////////
// internal functions connected to B64S objects

static b64Frame* read_pool_get_enc(b64Stream* strm)
{
    uint* p = ((b64Pool*)strm)->pos;
    b64Frame* ret = (b64Frame*)((byte*)strm->buf + *p);
    *p += 3;
    return ret;
}
static b64Frame* read_pool_get_dec(b64Stream* strm)
{
    uint* p = ((b64Pool*)strm)->pos;
    b64Frame* ret = &strm->buf[*p / 4];
    *p += 4;
    return ret;
}
static b64Frame* pool_get_string(b64Stream* strm)
{
    return strm->buf = (b64Frame*)pool_setc_ex(((b64Pool*)strm)->dat, '\0', strm->flg[3]);
}

static b64Frame* data_get_enc(b64Stream* strm)
{
    return (b64Frame*)((byte*)strm->buf + strm->pos++ * 3);
}
static b64Frame* data_get_dec(b64Stream* strm)
{
    return &strm->buf[strm->pos++];
}

static b64Frame* file_get_enc( b64Stream* strm )
{
    b64Frame* fr = (b64Frame*)&((b64File*)strm)->buf;
    fr->u32 = 0;
    fr->u8[3] = (byte)(3 - fread( fr, 1, 3, (FILE*)((b64File*)strm)->dat ));
    return fr;
}

static b64Frame* file_get_dec(b64Stream* strm)
{
    b64Frame* fr = (b64Frame*)&((b64File*)strm)->buf;
    fr->u32 = 1027423549u;
#if BASE64_WITH_LINEBREAKS == 1
    int i=0;
    byte* fb = &fr->u8[0];
    while( i++ < 4 ) {
        fread(fb, 1, 1, (FILE*)((b64File*)strm)->dat );
        if ( *fb != '\n' ) { ++fb; } else { --i; } }
#else
    fread( fr, 1, 4, (FILE*)((b64File*)strm)->dat );
#endif
    return fr;
}

// read 4 chars b64 data to return 3 decoded bytes + 0 on success or +!=0 on bad b64 data input
static b64Frame read_nxt_dec(void* stream)
{
    return base64_decodeFrame(
        *((b64Stream*)stream)->get((b64Stream*)stream)
                               );
}

//read 3 bytes as 4 chars b64 data from 'stream' or returns NULL on stream empty
static b64Frame read_nxt_enc(void* stream)
{
    b64Frame frme = *((b64Stream*)stream)->get((b64Stream*)stream);
    if ( frme.u8[3] ) return base64_encEndFrame( frme );
    else return base64_encodeFrame( frme );
}

static b64Frame read_end_enc(void* stream)
{
    b64Stream* strm = (b64Stream*)stream;
    if ( base64_canStream(strm) ) { // <- TODO:  try to let done without doing this check
        b64Frame rdinp = *strm->get(strm);
        if (rdinp.i8[0] == '\n' || rdinp.i8[1] == '\n' || rdinp.i8[2] == '\n')
            return nullFrame;
        return base64_encodeFrame(rdinp);
    }
    else return nullFrame;
}


static b64Frame write_nxt_dec( void* frame )
{
    return base64_decodeFrame( *(b64Frame*)frame );
}

static b64Frame write_nxt_enc( void* frame )
{
    return base64_encodeFrame( *(b64Frame*)frame );
}


static int file_write_set( b64Stream* strm, b64Frame frame )
{
    fwrite( &frame.i8[0], strm->flg[3], 1, (FILE*)((b64File*)strm)->dat );
    return strm->flg[0];
}

static int pool_write_set( b64Stream* strm, b64Frame frame )
{
    pool_setb( &frame.i8[0], strm->flg[3] );
    return strm->flg[3];
}



ptval b64_pool_fill_frames(void* dat, ptval siz, ptval cnt, b64Pool* strm)
{
    byte* data = (byte*)dat;
    pool_attach( strm->dat );
    for (uint i = 0; i < siz; ++i, data += cnt) {
        pool_setb(data, (uint)cnt);
    } pool_detach();
    return cnt;
}

int b64_data_fill_frame(b64Stream* stream, b64Frame frame)
{
    if (stream->len == EMPTY && stream->pos == 0)
        stream->len = 0;
    switch (stream->flg[0]) {
    case 3: ((b64Frame*)((byte*)stream->buf + (stream->len * 3)))->u32 = frame.u32;
        return (int)++stream->len;
    case 4: stream->buf[stream->len++] = frame;
        return (int)stream->len;
    default:
        return 0;
    }
}

static uint enc_from_b64s( void* bufferDst, uint blocksize, uint blockcount, b64Stream* stream )
{
    if (stream->flg[0] == ENCODE) {
        uint endchunk = false;
        uint wantBytes = blocksize * blockcount;
        uint available = base64_streamBytesRemaining( stream );
        if ( available < wantBytes ) {
            endchunk = available % 4;
            if (endchunk != 0) endchunk = 4 - endchunk;
            wantBytes = available + endchunk;
        } else if (blocksize != 4) {
            while( (wantBytes % 4) && (wantBytes > blocksize) )
                    wantBytes -= blocksize;
        } uint* end = (uint*)((byte*)bufferDst + wantBytes);
        for( uint* dst = (uint*)bufferDst; dst != end; ++dst ) {
            *dst = stream->nxt( stream ).u32;
        } blockcount = wantBytes / blocksize;
    } else {
        blockcount = 0;
        tempf( "stream type '%s'",
            stream->flg[0] == DECODE ? "decode" :
            stream->flg[0] == ENCODE ? "encode"
            : "unknown" );
        setErrorText( getTemp() );
    } return blockcount;
}

static uint dec_from_b64s( void* bufferDst, uint blocksize, uint blockcount, b64Stream* stream )
{
    if (stream->flg[0] == DECODE) {
        int wantBytes = blocksize * blockcount;
        int available = base64_streamBytesRemaining(stream);
        wantBytes = wantBytes < available
                  ? wantBytes : available;
        if (blocksize != 3) {
            wantBytes -= (wantBytes % 3);
        } while (wantBytes % blocksize) {
            wantBytes -= 3;
        } available = wantBytes;
        for( byte* dst = (byte*)bufferDst; available > 0; dst += 3 ) {
            if( ( asFrame( dst ) = stream->nxt(stream) ).u8[3] ) {
                asFrame( dst ).i8[3] = '\0';
                break; }
            available -= 3;
        } blockcount = ((wantBytes - available) / blocksize);
    } else {
        blockcount = 0;
        tempf("stream type '%s'",
            stream->flg[0] == ENCODE ? "encode" :
            stream->flg[0] == DECODE ? "decode"
            : "unknown");
        setErrorText(getTemp());
    } return blockcount;
}

static uint enc_into_b64s( void* bufferSrc, uint size, uint count, b64Stream* stream )
{
    if (stream->len == EMPTY && stream->pos == 0)
        stream->len = 0;

    int modulo;
    const byte* Src = (const byte*)bufferSrc;
    int wantWrite = (int)(size * count) + 1;
    do { --wantWrite;
    } while (wantWrite >= stream->flg[0] && (modulo = wantWrite % stream->flg[0]));
    wantWrite -= modulo;
    size = (uint)stream->len;
    if (wantWrite >= stream->flg[0]) {
        const byte* End = Src + wantWrite;
        for (; Src != End; Src += stream->flg[0]) {
            stream->set( stream, base64_encodeFrame( *(b64Frame*)Src ) );
        }
    } size = wantWrite + modulo;
    if (modulo) {
        wantWrite;
        for (uint i = 0; i < stream->flg[0]; ++i) {
            b64Frame* cach = (b64Frame*)&stream->buf;
            if (!cach->u8[i]) {
                cach->u8[i] = *Src++;
                if (!--modulo) break;
                if (cach->u8[stream->flg[0] - 1]) {
                    stream->set( stream, base64_encodeFrame(*cach) );
                    i = cach->u32 = 0;
                }
            }
        }
    } return *(int*)stream->len - size;
}

static uint dec_into_b64s( void* bufferSrc, uint size, uint count, b64Stream* stream )
{
    if (stream->len == EMPTY && stream->pos == 0)
        stream->len = 0;
    int wantWrite = (int)(size * count);
    while( wantWrite >= stream->flg[0] && wantWrite % stream->flg[0] ) {
        wantWrite -= size;
    } if (wantWrite == 0) {
        return 0;
    } else if (wantWrite < stream->flg[0]) {
        b64Frame last = { 1027423549u };
        byte* buf = (byte*)bufferSrc;
        for (int i = 0; i < wantWrite; ++i) {
            last.u8[i] = buf[i];
        } stream->set( stream, base64_decodeFrame( last ) );
    return 1; }
    int frames = wantWrite / stream->flg[0];
    b64Frame* Src = (b64Frame*)bufferSrc;
    while (frames--)
        stream->set( stream, base64_decodeFrame( *Src++ ) );
    return wantWrite / size;
}

/////////////////////////////////////////////////////////////////////////////
// creating stream objects:

uint base64_initNewB64StreamStruct( const char* mode, b64Stream* stream )
{
    stream->pos = stream->len = 0;
    stream->flg[0] = 0;
    const char* modchars = mode;
    do { switch (*modchars) {
        case 'r':
        case 'w': stream->flg[1] = *modchars;
            break;
        case 'e':
        case 'd': stream->flg[2] = *modchars;
        }
    } while (*++modchars);
    stream->flg[3] = 0;
    char* waswieso = (char*)&stream->flg[0];
    uint Mode = byteOrder_stringTOfourCC( waswieso );
    if( Mode < 7000000 )
        Mode = byteOrder_reverse32( Mode );
    switch( Mode ) {
    case READ_DECODED:
    case WRITE_DECODE:
        stream->nxt = &read_nxt_dec;
        stream->get = &data_get_dec; break;
    case READ_ENCODED:
    case WRITE_ENCODE:
        stream->nxt = &read_nxt_enc;
        stream->get = &data_get_enc; break;
    default:
        tempf( "bad modus: '%s'", mode );
        setErrorText( getTemp() );
        AsInt( &stream->flg[0] ) = 0;
        stream->pos = EMPTY;
        stream->len = EMPTY;
        stream->buf = NULL;
        stream->dat = NULL;
        return 0;
    } const char* M = byteOrder_fourCCtoString( Mode );
    stream->flg[0] = (byte)(104 - M[1]);
    stream->flg[3] = (byte)(-97 + M[1]);
    stream->flg[1] = (byte)M[2];
    stream->len = EMPTY;
    stream->pos = 0;
    return Mode >> 8;
}


b64Stream* base64_createDataStream( void* src_dat, uint src_len, const char* mode )
{
    b64Stream* stream = (b64Stream*)junk_allocateNewObject( (cmDtFunc)&base64_destream,
                                         sizeof(b64Stream) );
    uint m = base64_initNewB64StreamStruct( mode, stream );
    char* Mode = (char*)&m;
    if ( Mode[0] == 'd' && Mode[1] == 'r' ) {
        if (src_len == EMPTY) {
            src_len = removeLBs( (char*)src_dat );
        }
    } if (!wasError()) {
        stream->buf = (b64Frame*)src_dat;
        if (src_len != EMPTY)
            stream->len = src_len / stream->flg[0];
        stream->pos = 0;
        stream->flg[2] = DATASTREAM;
    } return stream;
}

b64Pool* base64_createPoolStream( const char* mode )
{
    b64Pool* stream = (b64Pool*)junk_allocateNewObject(
           (cmDtFunc)&base64_destream, sizeof(b64Pool) );
    uint m = base64_initNewB64StreamStruct( mode, (b64Stream*)stream );
    if (wasError())
        return stream;
    StringPool* p = pool_push();
    stream->flg[2] = POOLSTREAM;
    if (stream->flg[0] == ENCODE) {
        stream->get = &read_pool_get_enc;
    } else {// flg[0] == DECODE
        stream->get = &read_pool_get_dec;
    }if(stream->flg[1] == INPUT) {
        stream->len = pool_setCheckpoint();
    } else {// flg[1] == OUTPUT
        stream->pos = pool_setCheckpoint();
        stream->len = &stream->dat->Pos[CYCLE_COUNT - 1];
        stream->get = &pool_get_string;
    } stream->dat = pool_detach();
    return stream;
}

uint prepare_file_mode( uint modeflags )
{
    char* Mode = (char*)&modeflags;
    if (((Mode[0] == 'd') && (Mode[1] == 'w'))
    ||   (Mode[0] == 'e') && (Mode[1] == 'r'))
          Mode[2] = 'b';
    return modeflags;
}

int base64_initB64FileStreamStruct( b64File* stream, const char* file, unsigned mode )
{
    if (!wasError()) {
        char* Mode = (char*)&mode;
        if ( (FILE*)file == stdin
          || (FILE*)file == stdout
          || (FILE*)file == stderr ) {
            stream->dat = (void*)file;
            stream->flg[2] = STDSTREAM;
            if (stream->dat == stdin) {
                if (Mode[1] != 'r') {
                    setErrorText("stdin only can be opened for reading");
                } else {
                    stream->get = Mode[0] == 'e'
                                ? &file_get_enc
                                : &file_get_dec;
                    stream->len = EMPTY_(ptval);
                    if (stream->flg[0] == ENCODE)
                        stream->nxt = read_end_enc;
                    stream->buf = (ptval)'\n';
                }
            } else {
                if (Mode[1] != 'w') {
                    setErrorText("to stdout/stderr only can be written");
                } else {
                    stream->set = &file_write_set;
                    stream->len = (ptval)&stream->pos;
                }
            } stream->pos = 0;
        } else {
            stream->dat = fopen( file, &Mode[1] );
            if (stream->dat) {
                FILE* f = (FILE*)stream->dat;
                stream->flg[2] = FILESTREAM;
                if (Mode[1] == 'r') {
#ifdef _MSC_VER
                    char   dummy[64];
                    char*  buf;
                    char** ptA = &buf;
                    char** ptB = (char**)&dummy;
                    int*   ptC = NULL;
                    _get_stream_buffer_pointers(f, &ptA, &ptB, &ptC);
                    stream->get = Mode[0] == 'e'
                                ? &file_get_enc
                                : &file_get_dec;
                    stream->len = (ptval)ptC;
                    stream->pos = (ptval)ptB[0];
                    stream->buf = 0;
#elif defined(__TINYC__)
                    char    ein[1];
                    stream->buf = NULL;
                    stream->get = Mode[0] == 'e'
                                ? &file_get_enc
                                : &file_get_dec;
                    fread(&ein[0], 1, 1, (FILE*)stream->dat);
#endif
                    fseek(f, 0, SEEK_END);
                    int len = ftell(f);
                    fseek(f, 0, SEEK_SET);
                    stream->len = len;
                } else {
#ifdef _MSC_VER
                    char   dummy[64];
                    char** ptA = NULL;
                    char** ptB = (char**)&dummy;
                    int*   ptC = NULL;
                    _get_stream_buffer_pointers(f, &ptA, &ptB, &ptC);
                    stream->len = (ptval)ptC;
                    stream->pos = (ptval)ptB[0];
                    stream->buf = (ptval)(b64Frame*)ptA[0];
#else
                    stream->len = NULL;
                    stream->pos = 0;
#endif
                    stream->nxt = Mode[0] == 'e'
                                ? &write_nxt_enc
                                : &write_nxt_dec;
                    stream->set = &file_write_set;
                }
            } else {
                tempf("bad file: '%s'", file);
                setErrorText(getTemp());
            }
        }
    } return !wasError();
}

b64File* base64_createFileStream( const char* fileName, const char* mode )
{
    b64File* stream = (b64File*)junk_allocateNewObject(
           (cmDtFunc)&base64_destream, sizeof(b64File) );
    uint mod = base64_initNewB64StreamStruct( mode, (b64Stream*)stream );
    mod = prepare_file_mode( mod );
    clearAllErrors();
    if ( base64_initB64FileStreamStruct( stream, fileName, mod ) ) {
        return stream;
    } else {
        return (B64F*)junk_drop( stream );
    }
}

///////////////////////////////////////////////////////////////////////////
// API for using stream objects

// from 'stream', it reads 'count' blocks of data, each 'size' byte large
// return: count on read blocks, now contained in passed buffer 'dst'.
uint base64_sread( byte* dst, uint elm_siz, uint elm_cnt, b64Stream* stream )
{
    if( stream->flg[1] == INPUT ) {
        switch ( stream->flg[0] ) {
        case ENCODE: return enc_from_b64s( dst, elm_siz, elm_cnt, stream );
        case DECODE: return dec_from_b64s( dst, elm_siz, elm_cnt, stream );
        default: setErrorText("invalid codec mode"); return 0;
        }
    } else { setErrorText("wrong stream direction"); return 0; }
}

// into 'stream', it writes 'count' blocks of data, each 'size' byte large
// return: count on blocks successfully transfered from 'src' into 'stream'
uint base64_swrite( const byte* src, uint elm_siz, uint elm_cnt, b64Stream* stream )
{
    if( stream->flg[1] == OUTPUT ) {
        switch ( stream->flg[0] ) {
        case ENCODE: return enc_into_b64s((void*)src, elm_siz, elm_cnt, stream );
        case DECODE: return dec_into_b64s((void*)src, elm_siz, elm_cnt, stream );
        default: setErrorText("invalid codec mode"); return 0;
        }
    } else { setErrorText("wrong stream direction"); return 0; }
}

// gets one single frame (3 byte when encoding,
// or 4 byte when decoding) from an 'r' stream
b64Frame base64_getFrame( b64Stream* stream )
{
    return stream->nxt( stream );
}

// puts one single frame (3 byte when encoding,
// or 4 byte when decoding) into a 'w' stream
int base64_putFrame( b64Stream* stream, b64Frame frame )
{
    return stream->set( stream, stream->nxt( &frame ) );
}

// get size of one frame <direction> data of b64 <stream>
int base64_frameSize( char direction, b64Stream* stream )
{
    return stream->flg[1] == direction
         ? (int)stream->flg[0]
         : (int)stream->flg[3];
}

B64Nuller base64_Nuller(void)
{
    return nullFrame;
}

int base64_isEndFrame( b64Frame frame, b64Stream* stream )
{
    return (stream->flg[0] == DECODE) ? frame.u8[3] != 0
         : frame.u32 == base64_Nuller().u32;
}

void base64_destream( b64Stream* stream )
{
    switch (stream->flg[2]) {
    case FILESTREAM:
        fflush((FILE*)stream->dat);
        fclose((FILE*)stream->dat);
    case STDSTREAM:
    case DATASTREAM:
        stream->len = stream->pos;
        break;
    case POOLSTREAM:
        stream->len = NULL;
        const uint end = pool_cyclesPushed();
        pool_attach((POOL*)stream->dat);
        for (; pool_cyclesPushed(); pool_pop());
        break;
    } AsInt(&stream->flg[0]) = 0;
    stream->dat = stream->buf = NULL;
}


uint base64_streamBytesRemaining( b64Stream* strm )
{
    switch (strm->flg[2]) {
    case DATASTREAM: return (uint)( (strm->len - strm->pos) * strm->flg[3] );
    case FILESTREAM: return (uint)( (strm->len - ftell((FILE*)strm->dat)) * BASE64_STREAM_RATE(strm) );
    case POOLSTREAM: return (*(uint*)strm->len - *(uint*)strm->pos);
    } return NULL;
}

int base64_canStream( b64Stream* stream )
{
    return stream->flg[2] == POOLSTREAM
         ? (*(uint*)stream->len > *(uint*)stream->pos)
         : (stream->flg[2] & FILESTREAM) == FILESTREAM
         ? ((ulong)stream->len > (ulong)ftell((FILE*)stream->dat))
         : stream->len > stream->pos;
}
