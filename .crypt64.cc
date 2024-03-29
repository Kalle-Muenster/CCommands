/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      crypt64.cc                                 ||
||     Author:    Kalle                                      ||
||     Generated: 17.10.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".crypt64.h"
  #include ".commandLiner.h"
  #include ".junkYard.h"
  //#include ".hash64.h"
#else
#ifndef BASE64_ENABLE_STREAMING_API
#define BASE64_ENABLE_STREAMING_API
#endif
 // #include ".hash64.h"
 #include ".base64.h"
 #define WITH_JUNKYARD
 #include ".commandLiner.h"
 #ifdef crypt64
  int USAGE( crypt64 )
  {
    Synopsis("--k-<passkey> <[-s|--[d|e]-content]> [options]");
    printf("%s","Options:\n\n\
    --k-        : pass key (either passphrase or a 64bit integer value)\n\
    --e/d-      : content which shall be (d)e or (e)n crypted...\n\
    --o-*.yps   : explicitely define de/en-crytion output destination.\n\
    --bin       : output will be binary data of exact input size + 12 byte.\n\
    --b64*      : output will be string data of 4:3 times the input size.\n\
    --Hash      : enters 'Hash' modus\n\
      --set     : select which hash algorythm crypt64 uses for hashing passwords\n\
      --lst     : output a list of supported hash algorythem names (to be used as --set <alg>)\n\
      -v <data> : simply calculate hash value for given <data> (-t/-f file or text)\n\
     -t         : interpret command line paramer <content> being string input\n\
     -f*        : interpret command line paramer <content> being file input\n\
     -s         : stream input from stdin rather then passing <content> parameter\n\n\
    (* when output option (-o) is ommitted, output filename will gain '.yps' extension\n\
     so it defaults to '<content>.yps' when encrypting. When decrypting it will\n\
     try to remove '.yps' extension from filenames like '<content.yps>' )\n\n");
  #ifdef using_environMentor
    if( isModus( "verbose" ) ) {
        showPersistente( LOCAL );
    }
  #endif
    return CheckForError()
        || !hasOption('h');
  }
 #endif
#endif

/* TODO's:
    1. add possibility to use a rotating cryption table during de/en cryption. so each frame will be crypted by a different key, in relation to its possition in the encrypted data  differently effected due to this rotation
          - actual rotation shift for individual frames when seeking inside cryptic data stream can be evaluated via modulo (frameposition % shiftlength) then

    2. make possible restricting pass phrases to being only valid if containing a mandatory count on 'wrong' entered characters. - some rules like this:
        password must contain:
            at least 1 alphabetical character
            at least 1 numerical didgit character
            at least 1 special character
     'and': at least 1 wrong entered character
            (would lead to a maybe confusing behavior when seting up a password.... the second 'verify password' field then must be entered
            differently then the first 'set password' field - divergency between these varients of the password then is mandatorey errorlevel
            for entering valid passwords later)
        so, entering passwords correctly always would be invalid then. only inputs where validation returns some distinct, mandatory errorlevel would
        be treated 'valid' password for granting entrance then.
        this then introduces possibillity for "password always MUST be enterd differently then as like it was entered last time before" like rules...
        if entered same wrong character at same position, two times after another - the second time it won't be valid anymore. (could greatly
        increase security regarding automated attacks where algorythms trying to find valid entry by iterating random numbers... - because
        a password found by an algorythm like such, turns invalid within same moment the algorythinm finds entry via that password...

*/
// flg[1] stream type '?'  (check: 0x3F == 0x2E identifies a Non-Crypted FILE
// CRYPST                   check: 0x3F == 0x2F identifies a Non-Crypted In/Out/Err stream
//                          check: 0x3F == 0x3E identifies Encrypted FILE
//                          check: 0x3F == 0x3F identifies Encrypted In/Out/Err stream )


char* K64_table( K64* key ) {
    return &key->b64cc[4].i8[0];
}

int crypt64_isValidKey( K64* key ) {
    return *(int*)K64_table( key )
        && key->pass.value;
}

static int isContained( char idx, char* tbl )
{
    int pos = 0;
    while( pos < 64 ) {
        if( tbl[pos++] == idx )
            return pos;
    } return 0;
}

static void cleansenTheKey( K64* key64 )
{
    if( key64 ) {
        if( key64->pass.value )
            crypt64_invalidateKey( key64 );
    }
}

ulong crypt64_getHashFromKey( K64* key )
{
    return key->pass.value;
}

const char* createTableFromKey( ulong key )
{
    const char* table = base64_b64Table();
    K64 usedKey;
    usedKey.pass.value = key;
    byte overleap = ( (byte)(key&0x000000000000003F) << 2 );
    char* keyTable = K64_table( &usedKey );
    for( int i=0; i<66; ++i )
        keyTable[i] = -1;
    keyTable[64] = '=';
    keyTable[65] = '\0';
    char* crptbl = keyTable;
    int shift = 0;
    for(int i = 0; i < 64; ++i ) {
        uint pos = (uint)(key&0x000000000000003F);
        int  dir = ((pos&0x00000020)!=0)?1:-1;
        char c;
        while( isContained( c=(char)pos, keyTable ) ) {
            pos+= dir;
            pos = pos == 64 ? 0u
                : pos >= 65 ? 63u
                : pos;
        } *crptbl = (char)pos;
         ++crptbl;
        if( ++shift == 7 ) {
            key = key|((ulong)overleap << 56);
        } key = (key >> 1);
    } for( int i = 0; i < 64; ++i )
        keyTable[i] = table[keyTable[i]];
    char* t = (char*)pool_setb( keyTable, 66 );
    if ( pool_overlap() ) { t[74] = '\0';
        *(ulong*)&t[66] = usedKey.pass.value;
        return pool_get();
    } else pool_setb( &usedKey.pass, 8 );
    return pool_merge(2);
}

ulong findKeyInThePool( const char* poolTable )
{
    return *(ulong*)&poolTable[66];
}

ulong crypt64_calculateHashValue( const byte* input_data, int input_size )
{
    ulong hash = 0;
    if( input_data && input_size )
        while ( input_size-- )
           hash = 63 * hash + (ulong)*input_data++;
    return hash;
}

int haerteGradFromMerkeHart( ulong a, int hart, ulong b, ulong* grad )
{
    ulong merk = *grad;
    int haerte = 0;
    ulong hier = 0;
    for ( int i=0; i<64; ++i ) { hier = 1ull << i;
        int ist = (int)( (a&hier) ^ (b&hier) );
        if (ist) {
            ist = (*grad & hier) == 0;
            *grad |= hier;
        } else {
            *grad &= ~hier;
        } haerte += ist;
    } return merk != *grad
          && haerte == hart -1;
}

// crypt64 API: //////////////////////////////////////////

K64* crypt64_createKeyFromHash( ulong hash )
{
    const char* table = createTableFromKey( hash );
    DEBUGLOG(table)
    if( findKeyInThePool( table ) == hash ) {
        K64* newKey = crypt64_allocateNewKey();
        char* keyTable = K64_table( newKey );
        for( int i=0; i<66; ++i )
            keyTable[i] = table[i];
        newKey->pass.value = hash;
        return newKey;
    } else {
        setError( "table missmatch", TABLES_ERROR );
        return NULL;
    }
}

K64* crypt64_createKeyFromPass( const char* passphrase )
{
    return crypt64_createKeyFromHash(
        crypt64_calculateHashValue(
            (const byte*)passphrase,
            (int)strlen( passphrase ) )
    );
}

K64* crypt64_allocateNewKey( void )
{
    return (K64*)junk_allocateNewObject(
           (cmDtFunc)&cleansenTheKey, sizeof(K64) );
}

K64* crypt64_initializeKey( K64* key, ulong value )
{
    const char* table = createTableFromKey( value );
    if( findKeyInThePool( table ) == value ) {
        cleansenTheKey( key );
        char* keyTable = K64_table( key );
        for( int i=0; i<66; ++i )
            keyTable[i] = table[i];
        key->pass.value = value;
        return key;
    } else {
        setError( "table missmatch", TABLES_ERROR );
        return NULL;
    }
}

void crypt64_invalidateKey( K64* key64 )
{
    if( key64 ) {
        key64->table = 0;
        key64->pass.value = 21;
        uint* ptr = &key64->b64cc[0].u32;
        do { *ptr++ = 0;
        } while( --key64->pass.value );
    }
}

const char* crypt64_createValidator( K64* key, b64State* state )
{
    if(!state ) state = &EncoderState;
    if( state->Context == key->pass.value )
    {
        // create 'encryption key' verification header
        key->b64cc[0].u32 =  *(uint*)&key->pass.ccode[0];
        key->b64cc[0] = base64_EncodeFrame( state, key->b64cc[0] );
        key->b64cc[1].u32 =  *(uint*)&key->pass.ccode[3];
        key->b64cc[1] = base64_EncodeFrame( state, key->b64cc[1] );
        key->b64cc[2].u32 =  *(uint*)&key->pass.ccode[6];
        key->b64cc[2].i8[2] = 0; key->b64cc[2].i8[3] = 0;
        key->b64cc[2] = base64_EncodeFrame( state, key->b64cc[2] );

        char mod = key->b64cc[3].i8[1];
        key->b64cc[4].u32 = 0;

        // create 'encryption format' verification header
        key->b64cc[3].i8[0] = 'e';
        key->b64cc[3].i8[1] = 'b';
        key->b64cc[3].i8[2] = mod == BINARY ? 'i' : '6';
        key->b64cc[3].i8[3] = mod == BINARY ? 'n' : '4';

    return &key->b64cc[0].i8[0]; }

    else setError( "context", CONTXT_ERROR );
    return 0;
}

fourCC checkEncryptionFormat( K64* key, const byte* header )
{
    key->b64cc[4].u32 = 0;
    key->b64cc[0].u32 = ((const uint*)header)[0];
    key->b64cc[1].u32 = ((const uint*)header)[1];
    key->b64cc[2].u32 = ((const uint*)header)[2];
    key->b64cc[3].u32 = ((const uint*)header)[3];
    key->b64cc[4].i8[0] = 0;
    fourCC format = *(fourCC*)&key->b64cc[3].i8[1];
    key->b64cc[4].i8[0] = 0x3D;
    return format;
}

int crypt64_verifyValidator( K64* key, const byte* dat, b64State* state )
{
    if( !state ) state = &EncoderState;

    if( state->Context == key->pass.value )
    {
        char   ctx = key->b64cc[3].i8[1];
        fourCC fmt = checkEncryptionFormat( key, dat );
        char   dir = key->b64cc[3].i8[0] - 1;

        int valide = (dir == 'd');
        if( valide )
            valide = ( ctx == BINARY
                   ? ( ((fmt&0x0000ff00)>>8) == 'i')
                   : ( ((fmt&0x0000ff00)>>8) == '6') );
        if( valide ) { byte validator[24];
            validator[20] = validator[23] = 0;
            base64_DecodeData( state, &validator[0], &key->b64cc[0].i8[0], 16 );
            ulong check = byteOrder_stringTOlongCC( &validator[0] );
            if( !( valide = ( key->pass.value == check ) ) )
                setError( "phrase", PHRASE_ERROR );
        } else  setError( "format", fmt );
        key->b64cc[3].i8[1] = ctx;
    return valide; }
    else setError( "pending", CONTXT_ERROR );
    return false;
}

int crypt64_prepareContext( K64* key, byte mod, b64State* state )
{
    if( !state ) state = &EncoderState;
    if(!state->Context ) {
        state->Context = key->pass.value;
        if( !crypt64_isValidKey( key ) ) {
            const char* table = createTableFromKey( key->pass.value );
            char* keyTable = K64_table( key );
            for( int i=0; i<64; ++i ) keyTable[i] = table[i];
        } key->table = base64_SetTable( state, K64_table( key ) );
        key->b64cc[3].u8[1] = mod;
        return true;
    } else if ( state->Context == key->pass.value ) {
        key->table = base64_GetTable( state );
        key->b64cc[3].u8[1] = mod;
        return true;
    } setError( "context", CONTXT_ERROR );
    return false;
}

int crypt64_releaseContext( K64* key, b64State* state )
{
    if( !state ) state = &EncoderState;
    if( state->Context == key->pass.value ) {
        state->CodeTable = key->table;
        state->Context = NULL;
        key->b64cc[4].i8[0] = key->table[0];
        key->b64cc[4].i8[1] = key->table[1];
        key->b64cc[4].i8[2] = key->table[2];
        key->b64cc[4].i8[3] = key->table[3];
        key->table = &key->b64cc[4].i8[0];
    } return !state->Context;
}

bool crypt64_SetContext( b64State* state, K64* key, byte mod )
{
    if( !state->Context ) {
        if( crypt64_isValidKey( key ) ) {
            state->Context = key->pass.value;
            key->table = base64_SetTable( state, K64_table( key ) );
            key->b64cc[3].u8[1] = mod;
            return true;
        } setError( "Initialization", NOT_INITIALIZED );
        return false;
    } else if( state->Context == key->pass.value ) {
        key->table = base64_GetTable( state );
        key->b64cc[3].u8[1] = mod;
        return true;
    } setError( "state->Context", CONTXT_ERROR );
    return false;
}

bool crypt64_setContext( K64* key, byte mod )
{
    return crypt64_SetContext( &EncoderState, key, mod );
}

const char* crypt64_SwapTable( k64State* state, K64* key )
{
    if ( state->CodeTable != key->table )
         return base64_UseTable( state, key->table );
    else return base64_UseTable( state, base64_b64Table() );
}
const char* crypt64_swapTable( K64* key )
{
    return crypt64_SwapTable( &EncoderState, key );
}

uint crypt64_Encrypt( k64State* state, K64* key, const byte* data, uint size, char* dest )
{
    uint outlen = 0;
    if( crypt64_prepareContext( key, BASE64, state ) ) {
        outlen = 16;
        memcpy( &dest[0], crypt64_createValidator( key, state ), outlen );
        outlen += base64_EncodeData( state, &dest[outlen], data, size, 0 );
        crypt64_releaseContext( key, state );
    } return outlen;
}
uint crypt64_encrypt( K64* key, const byte* data, uint size, char* dest )
{
    uint outlen = 0;
    if( crypt64_prepareContext( key, BASE64, NULL ) ) {
        outlen = 16;
        memcpy(&dest[0], crypt64_createValidator(key, NULL), outlen);
        outlen += base64_encodeData( &dest[outlen], data, size, 0 );
        crypt64_releaseContext( key, NULL );
    } return outlen;
}

uint crypt64_Decrypt( k64State* state, K64* key, const char* data, uint size, byte* dest )
{
    uint outlen = 0;
    if( crypt64_prepareContext( key, BASE64, state ) ) {
        if( crypt64_verifyValidator( key, (const byte*)data, state ) ) {
            outlen = base64_DecodeData( state, &dest[0], &data[16], size - 16 );
        } crypt64_releaseContext( key, state );
    } return outlen;
}
uint crypt64_decrypt( K64* key, const char* data, uint size, byte* dest )
{
    uint outlen = 0;
    if( crypt64_prepareContext( key, BASE64, NULL ) ) {
        if( crypt64_verifyValidator( key, (const byte*)data, NULL ) ) {
            outlen = base64_decodeData( &dest[0], &data[16], size-16 );
        } crypt64_releaseContext( key, NULL );
    } return outlen;
}

uint crypt64_binary_Encrypt( k64State* state, K64* key, const byte* data, uint size, byte* dest )
{
    uint out_size = 0;
    int pos = size;
    if( crypt64_prepareContext( key, BINARY, state ) )
    {
        const char* CrpTable = base64_GetTable( state );
        const char* B64Table = base64_b64Table();
        const char* header = crypt64_createValidator( key, NULL );
        if ( header ) { out_size = 12;
            state->CodeTable = B64Table;
            base64_DecodeData( state, dest, header, EMPTY );
            dest += 12;
            b64Frame frame = {0};
            while ( pos > 0 ) {
                frame = asFrame(data);
                frame.u8[3] = 0;
                state->CodeTable = CrpTable;
                frame.u32 = base64_EncodeFrame( state, frame ).u32;
                state->CodeTable = B64Table;
                frame.u32 = base64_DecodeFrame( state, frame ).u32;
                asFrame(dest).u32 = frame.u32;
                out_size += 3;
                if( frame.u8[3] ) break;
                dest += 3;
                data += 3;
                pos -= 3;
            } clearAllErrors();
        } state->CodeTable = CrpTable;

    crypt64_releaseContext( key, state ); }
    return out_size;
}
uint crypt64_binary_encrypt( K64* key, const byte* data, uint size, byte* dest )
{
    return crypt64_binary_Encrypt( &EncoderState, key, data, size, dest );
}

uint crypt64_binary_Decrypt( k64State* state, K64* key, const byte* data, uint size, byte* dest )
{
    if( crypt64_prepareContext( key, BINARY, state ) )
    {
        const char* CrpTable = base64_GetTable( state );
        const char* B64Table = base64_b64Table();

        state->CodeTable = B64Table;
        char header[24];
        base64_EncodeData( state, &header[0], data, 12, 0 );
        int pos = size - 12;
        data += 12;
        size = 0;
        state->CodeTable = CrpTable;
        if( crypt64_verifyValidator( key, (const byte*)&header[0], state ) ) {
            b64Frame frame = {0};
            while ( pos > 0 ) {
                state->CodeTable = B64Table;
                frame.u32 = base64_EncodeFrame( state, asFrame( data ) ).u32;
                state->CodeTable = CrpTable;
                frame.u32 = base64_DecodeFrame( state, frame ).u32;
                asFrame(dest) = frame;
                size += 3;
                if( frame.u8[3] ) break;
                dest += 3;
                data += 3;
                pos -= 3;
            } clearAllErrors();
        } crypt64_releaseContext( key, state );
    } else size = 0;
    return size;
}
uint crypt64_binary_decrypt( K64* key, const byte* data, uint size, byte* dest )
{
    return crypt64_binary_Decrypt( &EncoderState, key, data, size, dest );
}

uint crypt64_EncryptFile( k64State* state, K64* key, const char* srcFile, const char* dstFile )
{
    ptval size = 0;
    if( crypt64_prepareContext( key, BASE64, state ) )
    {
        FILE* dst = fopen( dstFile, "w" );
        if  (!dst) {
            setError( setTempf("opening file %s", dstFile), OUTPUT_ERROR );
            return 0;
        }
        B64S* src = (B64S*)base64_CreateFileStream( state, srcFile, "re" );
        if ( !src ) {
            setError( setTempf("opening file %s", srcFile), INPUTS_ERROR );
            return 0;
        }
        size += fwrite( crypt64_createValidator( key, state ), 1, 16, dst );

        b64Frame data = {0};
        byte buf[1024];
        ptval siz = 0;

        do { siz = base64_sread( &buf[0], 4, 256, src );
            size += ( fwrite( &buf[0], 4, siz, dst ) * 4 );
        } while( siz == 256 );

        data.i8[0] = '=';
        size += fwrite( &data.i8[0], 1, 1, dst );
        fflush(dst); fclose(dst);
        base64_destream( src );

    crypt64_releaseContext( key, state ); }
    return (uint)size;
}
uint crypt64_encryptFile( K64* key, const char* srcFile, const char* dstFile )
{
    return crypt64_EncryptFile( &EncoderState, key, srcFile, dstFile );
}

uint crypt64_binary_EncryptFile( k64State* state, K64* key, const char* srcFile, const char* dstFile )
{
    DEBUGLOG("enter")
    ptval size = 0;
    const char* B64Table = base64_b64Table();
    if( crypt64_prepareContext( key, BINARY, state ) )
    {
        const char* validator = crypt64_createValidator( key, state );
        B64S* src = (B64S*)base64_CreateFileStream( state, srcFile, "re" );
        if ( !src ) { setError( setTempf("opening file %s", srcFile), INPUTS_ERROR ); return 0; }
        const char* CrpTable = base64_GetTable( state );
        state->CodeTable = B64Table;
        B64S* dst = (B64S*)base64_CreateFileStream( state, dstFile, "wd" );
        if ( !dst ) { setError( setTempf("opening file %s", dstFile), OUTPUT_ERROR ); return 0; }
        size += base64_swrite( (const byte*)validator, 1, 16, dst );

        byte buf[1024];
        ptval siz = 0;
        do{ state->CodeTable = CrpTable;
            siz = base64_sread( &buf[0], 4, 256, src );
            state->CodeTable = B64Table;
            size += (base64_swrite( &buf[0], 4, (uint)siz, dst ) * 3);
        } while( siz == 256 );
        state->CodeTable = CrpTable;

        base64_destream( (B64S*)src );
        base64_destream( (B64S*)dst );

    crypt64_releaseContext( key, state ); }
    return (uint)size;
}
uint crypt64_binary_encryptFile( K64* key, const char* srcFile, const char* dstFile )
{
    return crypt64_binary_EncryptFile( &EncoderState, key, srcFile, dstFile );
}

uint crypt64_DecryptFile( b64State* state, K64* key, const char* srcFileName, const char* dstFileName )
{
    ptval size = 0;
    if( crypt64_prepareContext( key, BASE64, state ) )
    {
        char header[24];
        FILE* src = fopen( srcFileName, "r" );
        if ( !src ) {
            setError( setTempf( "opening file %s", srcFileName), INPUTS_ERROR );
            return 0;
        }

        fread( &header[0], 1, 16, src );
        if( crypt64_verifyValidator( key, (const byte*)&header[0], state ) ) {
            B64S* dst = (B64S*)base64_CreateFileStream( state, dstFileName, "wd" );
            if ( !dst ) {
                setError( setTempf( "opening file %s", dstFileName ), OUTPUT_ERROR );
                return 0;
            }

            ptval siz = 0;
            byte buf[1024];
            do { siz = fread( &buf[0], 4, 256, src );
                size += (base64_swrite( &buf[0], 4, (uint)siz, dst ) * 3);
            } while( siz == 256 );

            base64_destream( (b64Stream*)dst );
        } fclose(src);

    crypt64_releaseContext( key, state ); }
    return (uint)size;
}
uint crypt64_decryptFile( K64* key, const char* srcFileName, const char* dstFileName )
{
    return crypt64_DecryptFile( &EncoderState, key, srcFileName, dstFileName );
}

uint crypt64_binary_DecryptFile( b64State* state, K64* key, const char* srcFileName, const char* dstFileName )
{
    uint size = 0;
    const char* B64Table = base64_b64Table();
    state->CodeTable = B64Table;
    B64S* src = (B64S*)base64_CreateFileStream( state, srcFileName, "re" );
    if ( !src ) { setError( setTempf("opening file %s", srcFileName), INPUTS_ERROR ); return 0; }
    char header[24];
    base64_sread( (byte*)&header, 1, 16, src );
    if( crypt64_prepareContext( key, BINARY, state ) )
    {
        const char* CrpTable = base64_GetTable( state );
        if( crypt64_verifyValidator( key, (const byte*)&header[0], state ) ) {
            B64S* dst = (B64S*)base64_CreateFileStream( state, dstFileName, "wd" );
            if ( !dst ) { setError( setTempf("opening file %s", srcFileName), OUTPUT_ERROR ); return size; }
            ptval siz = 0;
            byte buf[1024];
            do{ state->CodeTable = B64Table;
                siz = base64_sread( &buf[0], 4, 256, src );
                state->CodeTable = CrpTable;
                size += ( base64_swrite( &buf[0], 4, (uint)siz, dst ) * 3 );
            } while( siz == 256 );
            base64_destream( dst );
        }
    crypt64_releaseContext( key, state ); }
    base64_destream( src );
    return size;
}
uint crypt64_binary_decryptFile( K64* key, const char* srcFileName, const char* dstFileName )
{
    return crypt64_binary_DecryptFile( &EncoderState, key, srcFileName, dstFileName );
}

// decrypt before encrypted data from stdin to file 'destination'
// as soon a '\n' is encounterd in stdin, it returns count
// on bytes which have been decrypted during streaming and which
// should be now the content of the passed 'destination' file.
uint crypt64_decryptStdIn( K64* key, FILE* destination )
{
    uint size = 0;
    if( crypt64_prepareContext( key, BASE64, NULL ) )
    {
        char header[24];
        fread( &header[0], 1, 16, stdin );
        if( crypt64_verifyValidator( key, (const byte*)&header[0], NULL ) ) { b64Frame pass;
            b64Stream* b64in = (b64Stream*)base64_createFileStream( 0, "rd" );
            while ( !( pass = base64_getFrame( b64in ) ).u8[3] ) {
                size += (uint)fwrite( &pass, (ptval)1, (ptval)3, destination );
            } size = (uint)b64in->pos * 3;
            base64_destream( b64in );
        }

    crypt64_releaseContext( key, NULL ); }
    return size;
}

// encrypt non encrypted data read from stdin to file 'destination'
// as soon a '\n' is encounterd in stdin it returns size in bytes
// of encrypted data which has been streamed to file 'destination'.
uint crypt64_encryptStdIn( K64* key, FILE* destination )
{
    uint size = 0;
    if ( crypt64_prepareContext( key, BASE64, NULL ) )
    {
        fwrite( crypt64_createValidator( key, NULL ), 1, 16, destination );
        b64Stream* b64in = (b64Stream*)base64_createFileStream( 0, "re" );
        uint size = 16; b64Frame pass;
        do { pass = base64_getFrame( b64in );
            size += (uint)fwrite( &pass, 1, 4, destination );
        } while( pass.i8[0] != '=' && pass.i8[1] != '='
              && pass.i8[2] != '=' && pass.i8[3] != '=' );
        base64_destream( b64in );

    crypt64_releaseContext( key, NULL ); }
    return size;
}

uint crypt64_binary_decryptStdIn( K64* key, FILE* destination )
{
    setErrorText("not implemented yet");
    return 0;
}
uint crypt64_binary_encryptStdIn( K64* key, FILE* destination )
{
    setErrorText("not implemented yet");
    return 0;
}

k64Chunk crypt64_encryptFrame( K64* key64, k64Chunk threeByte )
{
    if( EncoderState.Context == key64->pass.value ) {
        EncoderState.CodeTable = key64->table;
        return base64_encodeFrame( threeByte );
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

k64Chunk crypt64_decryptFrame( K64* key64, k64Chunk fourChars )
{
    if( EncoderState.Context == key64->pass.value ) {
        EncoderState.CodeTable = key64->table;
        return base64_decodeFrame( fourChars );
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

k64Chunk crypt64_binary_encryptFrame( K64* key64, k64Chunk threeByte )
{
    if( EncoderState.Context == key64->pass.value ) {
        EncoderState.CodeTable = key64->table;
        threeByte = base64_encodeFrame( threeByte );
        if ( threeByte.u8[3] ) { EncoderState.CodeTable = base64_b64Table();
            return base64_decodeFrame( threeByte ); }
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

k64Chunk crypt64_binary_decryptFrame( K64* key64, k64Chunk threeByte )
{
    if( EncoderState.Context == key64->pass.value ) {
        EncoderState.CodeTable = base64_b64Table();
        threeByte = base64_encodeFrame( threeByte );
        if (threeByte.u8[3]) { EncoderState.CodeTable = key64->table;
            return base64_decodeFrame( threeByte ); }
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}



k64Chunk crypt64_EncryptFrame( k64State* state, K64* key64, k64Chunk threeByte )
{
    if( state->Context == key64->pass.value ) {
        state->CodeTable = key64->table;
        return base64_EncodeFrame( (b64State*)state, threeByte );
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

k64Chunk crypt64_DecryptFrame( k64State* state, K64* key64, k64Chunk fourChars )
{
    if( state->Context == key64->pass.value ) {
        state->CodeTable = key64->table;
        return base64_DecodeFrame( (b64State*)state, fourChars );
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

k64Chunk crypt64_binary_EncryptFrame( k64State* sta, K64* key64, k64Chunk threeByte )
{
    b64State* state = (b64State*)sta;
    if( state->Context == key64->pass.value ) {
        state->CodeTable = key64->table;
        threeByte = base64_EncodeFrame( state, threeByte );
        if ( threeByte.u8[3] ) { state->CodeTable = base64_b64Table();
            return base64_DecodeFrame( state, threeByte ); }
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

k64Chunk crypt64_binary_DecryptFrame( k64State* state, K64* key64, k64Chunk threeByte )
{
    if( state->Context == key64->pass.value ) {
        state->CodeTable = base64_b64Table();
        threeByte = base64_EncodeFrame( (b64State*)state, threeByte );
        if (threeByte.u8[3]) { state->CodeTable = key64->table;
            return base64_DecodeFrame( (b64State*)state, threeByte ); }
    } else setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}



static void cleansenCrypstream( K64F* stream ) {
    if (stream->key) {
        crypt64_releaseContext( stream->key, NULL );
        stream->key = NULL;
    } base64_destream( &stream->b64 );
}

K64F* crypt64_CreateFileStream( k64State* state, K64* key, const char* path, const char* mode )
{
    int    mod = ENCODE;
    fourCC set = BASE64;
    int    dir = OUTPUT;

    K64F* stream = (K64F*)junk_allocateNewObject( (cmDtFunc)&cleansenCrypstream,
                               sizeof(k64Stream) );
    char fmod[8];
    char* fm = strcpy( &fmod[0], mode );
    while (*fm) switch ( *fm++ ) {
    case 'b': set = BINARY; break;
    case 'd': mod = DECODE; break;
    case 'e': mod = ENCODE; break;
    case 'r': dir = INPUT;  break;
    case 'w': dir = OUTPUT; break;
    }

    fm = &fmod[0];
    if ( set == BINARY )
        do if (*fm == 'd') *fm = 'e';
           else if (*fm == 'e') *fm = 'd';
        while ( *++fm );

    uint m = base64_initNewB64StreamStruct( &fmod[0], &stream->b64 );
    ((B64S*)stream)->b64 = (b64State*)state;

    fm = (char*)&m;
    if (( fm[0] == 'd' && fm[1] == 'w' )
     || ( fm[0] == 'e' && fm[1] == 'r' )
     || ( set == BINARY ) ) fm[2] = 'b';

    int success = false;
    if ( base64_initB64FileStreamStruct( (B64F*)&stream->b64, path, m ) ) {
        stream->key = key;
        success = true;
    }

    stream->val = NULL;
    if ( success ) { success = false;
    switch ( dir ) {
    case INPUT: {
        //////////////////////////////////////////////////////////////////
        switch ( mod ) {
        case DECODE: {
            //////////////////////////////////////////////////////////
        if ( crypt64_prepareContext( stream->key, set == BINARY ? BINARY : BASE64, state ) ) {
            char header[24];
            switch ( set ) {
            case BINARY: {
                stream->enc = base64_b64Table();
                stream->dec = base64_getTable();
                state->CodeTable = stream->enc;
                base64_sread( (byte*)&header[0], 4, 4, &stream->b64 );
                state->CodeTable = stream->dec;
            } break;
            case BASE64: {
                fread( &header[0], 1, 16, (FILE*)stream->b64.dat );
            } break; }
            success = crypt64_verifyValidator( stream->key, (const byte*)&header[0], state );
        } ///////////////////////////////////////////////////////////
        } break;
        case ENCODE: {
            ///////////////////////////////////////////////////////////
            if( crypt64_prepareContext( stream->key, set == BINARY ? BINARY : BASE64, state ) ) {
                stream->val = (const byte*)crypt64_createValidator( stream->key, state );
                if ( set == BINARY ) {
                    stream->enc = base64_getTable();
                    stream->dec = base64_b64Table();
                } success = true;
            }
        } break;
        }   } break;
    case OUTPUT: {
        //////////////////////////////////////////////////////////////////
        switch (mod) {
        case DECODE: {
            //////////////////////////////////////////////////////////
            if( crypt64_prepareContext( stream->key, set == BINARY ? BINARY : BASE64, state ) ) {
                stream->val = (const byte*)"blub";
                if ( set == BINARY ) {
                    stream->enc = base64_getTable();
                    stream->dec = base64_b64Table();
                } success = true;
            }
            //////////////////////////////////////////////////////////
        } break;
        case ENCODE: {
            //////////////////////////////////////////////////////////
            switch (set) {
            case BINARY: {
                if( crypt64_prepareContext( stream->key, BINARY, state ) ) {
                    const char* validator = crypt64_createValidator( stream->key, state );
                    stream->dec = base64_getTable();
                    stream->enc = base64_b64Table();
                    state->CodeTable = stream->enc;
                    base64_swrite( (const byte*)validator, 1, 16, &stream->b64 );
                    stream->key->b64cc[3].i8[1] = BINARY;
                    state->CodeTable = stream->dec;
                    success = true;
                }
            } break;
                //////////////////////////////////////////////////////////
            case BASE64: {
                if ( crypt64_prepareContext( stream->key, BASE64, state ) ) {
                    fwrite( crypt64_createValidator( stream->key, state ), 1, 16, (FILE*)stream->b64.dat );
                    stream->key->b64cc[3].i8[1] = BASE64;
                    success = true;
                }
            } break;
            }   } break;
            //////////////////////////////////////////////////////////
        } break;
        //////////////////////////////////////////////////////////////////
    }
    }
    }

    if( !success ) return (K64F*)junk_drop( stream );
    else return stream;
}
K64F* crypt64_createFileStream( K64* key, const char* path, const char* mode )
{
    return crypt64_CreateFileStream( &EncoderState, key, path, mode );
}


int delayedVerification( K64F* cryps, byte* data )
{
    int size = 0;
    b64State* state = ((B64S*)cryps)->b64;
    if ( cryps->b64.flg[2] == 'r' ) {
        if( cryps->key->b64cc[3].i8[0] == ENCODE ) {
            if( cryps->key->b64cc[3].i8[1] == BINARY ) {
                state->CodeTable = base64_b64Table();
                size = base64_DecodeData( state, data, (const char*)cryps->val, EMPTY );
            } else {
                memcpy( data, cryps->val, size = 16 );
            }
        } cryps->val = 0;
    } else
    if ( cryps->b64.flg[2] == 'w' ) {
        if( cryps->key->b64cc[3].i8[0] == ENCODE ) {
            if( cryps->key->b64cc[3].i8[1] == BINARY ) {
                state->CodeTable = base64_b64Table();
                base64_swrite( (const byte*)cryps->val, 1, 16, &cryps->b64 );
            } else fwrite( cryps->val, 1, 16, (FILE*)cryps->b64.dat );
        } else { size = 16;
            if( cryps->key->b64cc[3].i8[1] == BINARY ) {
                size -= 4;
                char header[24];
                const char* stored = base64_getTable();
                state->CodeTable = base64_b64Table();
                base64_EncodeData( state, &header[0], data, 12, 0 );
                cryps->val = (const byte*)&header[0];
                state->CodeTable = stored;
            } else cryps->val = data;
            if( !crypt64_verifyValidator( cryps->key, cryps->val, state ) )
                size = 0;
        } cryps->val = 0;
    }
    return size;
}

// in case 'binary encrypted' reading, the actually passed 'dst' buffer MUST allocate
// at least 4/3 times that count on byte it shall gain acording to the passed size parameter
// so the destination buffer itself can act as de/en cryption buffer during the progress..
uint crypt64_nonbuffered_sread( byte* dst, uint size, uint count, K64F* cryps )
{
    b64State* state = ((B64S*)cryps)->b64;
    if ( state->Context == cryps->key->pass.value ) {
    int read = 0;

        if( cryps->val )
            dst += ( read = delayedVerification( cryps, dst ) );

        if( cryps->key->b64cc[3].i8[1] == BINARY ) {
            state->CodeTable = cryps->enc;
            int rsize = base64_sread( dst, size, ((count*4)/3), (B64S*)cryps );
            dst[ rsize ] = 0;
            state->CodeTable = cryps->dec;
            read += base64_DecodeData( state, dst, (const char*)dst, rsize );
        } else {
            state->CodeTable = cryps->key->table;
            read += base64_sread( dst, size, count, (B64S*)cryps );
        }

    return read; }

    setError( "context", CONTXT_ERROR );
    return 0;
}

// if sure the passed dst buffer would be allocated much larger then
// the count on bytes which are requested (via 'size' parameter) it
// is apropriate using crypt64_nonbuffered_sread() overload instead
uint crypt64_sread( byte* dst, uint size, uint count, K64F* cryps )
{
    b64State* state = cryps->b64.b64;
    if ( state->Context == cryps->key->pass.value ) {
    int read = 0;

        if( cryps->val )
            dst += ( read = delayedVerification( cryps, dst ) );

        if( cryps->key->b64cc[3].i8[1] == BINARY ) {
            uint siz = size * count;
            siz += ((3 - (siz%3)) % 3);
            b64Frame data = {0};
            byte* end = dst + siz;
            siz = 0;
            do{ state->CodeTable = cryps->enc;
                data = base64_getFrame( (B64S*)cryps );
                state->CodeTable = cryps->dec;
                data = base64_DecodeFrame( state, data );
                asFrame(dst) = data;
                dst += 3;
                if ( data.u8[3] == 0 ) siz += 3;
                else break;
            } while( dst < end );
            read += (siz / size);
        } else {
            state->CodeTable = cryps->key->table;
            read += base64_sread( dst, size, count, (B64S*)cryps );
        }

    return read; }

    setError( "context", CONTXT_ERROR );
    return 0;
}

uint crypt64_swrite( const byte* src, uint size, uint count, K64F* cryps )
{
    b64State* state = cryps->b64.b64;
    if( state->Context == cryps->key->pass.value ) {
    int write = 0;

        if( cryps->val ) {
            write = delayedVerification( cryps, (byte*)src );
            if ( write == 0 ) return (uint)junk_drop( cryps );
            else src += write;
        }

        if( cryps->key->b64cc[3].i8[1] == BINARY ) {
            uint siz = (size * count);
            siz += ((3 - (siz % 3)) % 3);
            const byte* end = src + siz;
            b64Frame data;
            do{ state->CodeTable = cryps->dec;
                data.u32 = *(uint*)src;
                data = base64_EncodeFrame( state, data );
                state->CodeTable = cryps->enc;
                base64_putFrame( &cryps->b64, data );
                src += 3;
            } while (src < end);
            write += (siz / size);
        } else {
            state->CodeTable = cryps->key->table;
            write += base64_swrite( src, size, count, &cryps->b64 );
        }

    return write; }

    setError( "context", CONTXT_ERROR );
    return 0;
}

b64Frame erstmal_noch_der_header( b64Frame huch, K64F* vonDa )
 {
    if ( vonDa->b64.flg[1] == OUTPUT ) {
        K64F* nachDa = vonDa;
        if ( nachDa->val ) {
            byte* header;
            if ( FourCC( nachDa->val ) != FourCC( "blub" ) ) {
                byte* header = (byte*)junk_allocateJunkChunk( 24 );
                *(uint*)header = FourCC("blub");
                nachDa->val = header;
                header += 4;
                *(uint*)header = FourCC("plop");
            }
            int fram = crypt64_frameSize( nachDa );
            int size = 0;
            header = (byte*)(nachDa->val + 4);
            while ( FourCC( header ) != FourCC( "plop" ) ) {
                header += fram;
                size += fram;
            } *(uint*)*header = huch.u32;
            header += fram;
            size += fram;
            if ( size == (4 * fram) ) {
                *header = '\0';
                header = (byte*)nachDa->val;
                delayedVerification( nachDa, header+4 );
                junk_drop( header );
            } else {
                *(uint*)header = FourCC("plop");
            } huch.u32 = (uint)fram;
        }
    } else if( vonDa->key->b64cc[3].i8[0] == ENCODE && vonDa->val ) {

        if ( FourCC( vonDa->val ) == FourCC( "blub" ) ) {
            byte* header = (byte*)junk_allocateJunkChunk( 24 );
            int size = delayedVerification( vonDa, header );
            *(uint*)&header[size] = FourCC( "plop" );
            vonDa->val = header;
        }
        if ( FourCC( vonDa->val ) == FourCC( "plop" ) ) {
            vonDa->val -= crypt64_frameSize( vonDa );
            junk_drop( (void*)vonDa->val );
            huch.u32 = byteOrder_stringTOfourCC( "plop" );
            vonDa->val = 0;
        } else {
            huch.u32 = *(uint*)vonDa->val;
            vonDa->val += crypt64_frameSize( vonDa );
        }
    } return huch;
 }


b64Frame crypt64_getYps( K64F* vonDa )
{
    b64State* state = vonDa->b64.b64;
    if( state->Context == vonDa->key->pass.value ) {
        b64Frame yps = {0};
        int bin = vonDa->key->b64cc[3].i8[1] == BINARY;
        int out = vonDa->b64.flg[1] != OUTPUT;
        int chk = out;
        if( chk ) {
            yps = erstmal_noch_der_header( yps, vonDa );
            if( yps.u32 && yps.u32 != FourCC("plop") ) {
                if( bin ) { state->CodeTable = vonDa->dec;
                    return base64_DecodeFrame( state, yps );
                } return yps;
            } else chk = false;
        }
        if( !chk ){ // if header received completely or if is OUTPUT stream:
            if( bin ) {
                state->CodeTable = vonDa->enc;
                yps = out ? base64_peakWrite( (b64Stream*)&vonDa->b64 )
                          : base64_getFrame( (b64Stream*)&vonDa->b64 );
                state->CodeTable = vonDa->dec;
                return base64_DecodeFrame( state, yps );
            } else {
                state->CodeTable = vonDa->key->table;
                return base64_getFrame( (b64Stream*)&vonDa->b64 );
            }
        }
    } setError( "context", CONTXT_ERROR );
    return base64_Nuller();
}

uint crypt64_putYps( k64Chunk dieses, K64F* nachDa )
{
    b64State* state = nachDa->b64.b64;
    if( state->Context == nachDa->key->pass.value ) {
        uint huch = erstmal_noch_der_header( dieses, nachDa ).u32;
        if ( huch ) return huch;
        // if complete header is written to the stream
        if( nachDa->key->b64cc[3].i8[1] == BINARY ) {
            state->CodeTable = nachDa->dec;
            b64Frame inter = base64_EncodeFrame( state, dieses );
            state->CodeTable = nachDa->enc;
            return base64_putFrame( &nachDa->b64, inter );
        } else {
            state->CodeTable = nachDa->key->table;
            return base64_putFrame( &nachDa->b64, dieses );
        }
    } setError( "context", CONTXT_ERROR );
    return 0;
}

int crypt64_frameSize( K64F* stream )
{
    if (stream->key->b64cc[3].i8[1] == 0x10) return 3;
    else return stream->b64.flg[3];
}

int crypt64_position( K64F* stream )
{
    if ((stream->b64.flg[2] & FILESTREAM) == FILESTREAM)
        return ftell( (FILE*)stream->b64.dat );
    else
        return (int)stream->b64.pos;
}

int crypt64_canStream( K64F* stream )
{
    return base64_canStream( &stream->b64 );
}

void crypt64_flush( K64F* stream )
{
    if ( (stream->b64.flg[2] & FILESTREAM) == FILESTREAM )
        fflush( (FILE*)stream->b64.dat );
}

void crypt64_close( K64F* stream )
{
    cleansenCrypstream( stream );
    junk_drop( stream );
}

ptval crypt64_sizeof( K64F* stream )
{
    return stream->b64.len;
}

void crypt64_Initialize( bool init )
{
    if( init ) {
        QuickCommandInit();
        base64_Initialize();
        srand( (uint)(ptval)pool_InitializeCycle() );
    } else {
        DestructCommander();
    }
}

k64State* crypt64_InitializeState( k64State* init )
{
    QuickCommandInit();
    init = base64_InitializeState( (b64State*)init );
    srand( (uint)(ptval)init );
    return init;
}

k64State* crypt64_GetDefaultState( void )
{
    return (k64State*)base64_State();
}

#ifdef crypt64
int main(int argc,char**argv)
{
    if( (!InitCommandLiner(argc,argv))
     || (!(hasOption('d')||hasOption('e')||hasOption('t')||hasOption('p')))
     || ((!hasOption('k'))&&(!hasOption('H'))) ) {
         exit(USAGE(crypt64));
    }

    DEBUGLOG("base64_Initialize()")
    base64_Initialize();
    DEBUGLOG("srand((uint)(ptval)pool_InitializeCycle())")
    srand((uint)(ptval)pool_InitializeCycle());

    if ( isModus("test") ) {
        printf("\nTest Modus!\n");
        ulong hashA;
        sscanf( rawNext('t'), "%llu", &hashA );
        printf( "hashA: %llu\n", hashA );
        int hartG;
        sscanf( nextRaw(), "%i", &hartG );
        ulong hashB;
        sscanf( nextRaw(), "%llu", &hashB );
        printf( "hashB: %llu\n", hashB );
        ulong lastF;
        sscanf( nextRaw(), "%llu", &lastF );

        if ( haerteGradFromMerkeHart( hashA, hartG, hashB, &lastF ) ) {
            setError("Haerte Hart!",1);
            printf("grad=%llu\n",lastF);
        } else {
            setError("Weiche Wech!",EMPTY);
            printf("grad=%llu\n",lastF);
        } ExitOnError("Teste Haertegrad:");
    }

    if (isModus("verbose")) {
        showOptions();
    }
    EncoderState.Context = NULL;
    K64* key = NULL;
    uint size = NULL;

    short Mode = 0;
    if( !isModus("bin") )
        setModus("b64");
    if( !isSwitch('t') )
        setSwitch('f');
    if( search('d') )
        Mode = 'd';
    if( search('e') )
        Mode = 'e';

    if ( isModus("Hash") ) {
        printf("\nHash Modus!\n");
        if( search('v') ) {
            const char* data = getName('v');
            if( isSwitch('t') ) {
                printf("hash value of string: '%s' is %llu\n",
                        data, crypt64_calculateHashValue( data, (int)strlen(data) ) );
            } else if ( isSwitch('f') ) {
                FILE* f = fopen( data, "rb" );
                if (f) {
                    fseek(f,0,SEEK_END);
                    const int fsize = ftell(f);
                    MakeArray( byte, loade, fsize )
                    fseek(f,0,SEEK_SET);
                    fread(&loade[0],1,fsize,f);
                    fclose(f);
                    printf( "hash value of file: '%s' is %llu\n",
                            data, crypt64_calculateHashValue( loade, fsize ) );
                    CleanArray( loade )
                } else
                    setError("File not found",FourCC("file"));
            }
        } else
        if ( isModus("set") ) {
            setError("Not implemented yet",FourCC("impl"));
        } else
        if ( isModus("lst") ) {
            setError("Not implemented yet",FourCC("impl"));
        }
        exit( CheckForError() );
    }

    if( exists('k') ) {
        cmLn input = getName('k');
        if( !(key = crypt64_createKeyFromPass( input )) )
            setErrorText("key initialization");
        else key->b64cc[3].i8[1] = isModus("bin") ? 0x10 : 0x40;
    }
    switch( Mode ) {
        case 'e': setOption('i',getName('e')); size = CRYPT64_ENCRYPTED_SIZE( strlen(getName('e')) ); break;
        case 'd': setOption('i',getName('d')); size = CRYPT64_DECRYPTED_SIZE( strlen(getName('d')) ); break;
            break;
        default:  setErrorText("unknown error");
            break;
    } ExitOnError( "Parameter" );

    if ( isModus( "bin" ) ) {
        Mode = (Mode|(0x10<<8));
    } else {
        Mode = (Mode|(0x40<<8));
    }
    DEBUGFMT("Mode %i",Mode)
    if( isSwitch('f') ) {
        if( !search('o') ) {
            if ( (Mode&0x00ff) == 'e' ) {
                pool_setf( "%s.yps", getName('i') );
            } else {
                char* input = (char*)getName('i');
                if( AsInt(".yps") == AsInt( &input[strlen(input)-4]) )
                    input[strlen(input)-4] = '\0';
                pool_set( input );
            } setOption( 'o', pool_get() );
        }
        switch( Mode ) {
        case 16485:
            size = crypt64_encryptFile( key, getName('i'), getName('o') );
            break;
        case 16484:
            size = crypt64_decryptFile( key, getName('i'), getName('o') );
            break;
        case 4197:
            size = crypt64_binary_encryptFile( key, getName('i'), getName('o') );
            break;
        case 4196:
            size = crypt64_binary_decryptFile( key, getName('i'), getName('o') );
            break;
        }  ExitOnError( "Crypting" );
        if( isModus( "verbose" ) ) {
            printf("\nSuccessfully %scrypted %i byte to file: %s\n",Mode=='e'?"en":"de",size,getName('o'));
        }
    } else
    if( isSwitch('t') ) {
        MakeArray( char, data, CRYPT64_ENCRYPTED_SIZE( size ) );
        if( Mode == 16485 ) {
            size = crypt64_encrypt( key, (byte*)getName('i'), (uint)strlen( getName('i') ), &data[0] );
        } else if ( Mode == 16484 ) {
            size = crypt64_decrypt( key, getName('i'), (uint)strlen( getName('i') ), (byte*)&data[0] );
        } else if ( Mode == 4197 ) {
            size = crypt64_binary_encrypt( key, (byte*)getName('i'), (uint)strlen( getName('i') ), (byte*)&data[0] );
        } else if ( Mode == 4196 ) {
            size = crypt64_binary_decrypt( key, (byte*)getName('i'), (uint)strlen( getName('i') ), (byte*)&data[0] );
        }
        ExitOnError( "Crypting" );
        FILE* output = hasOption('o')
                     ? fopen( getName('o'), "wb" )
                     : stdout;
        printf("\n");
        if( isModus( "verbose" ) )
            printf( "Successfully %scrypted %i byte:\n", ((Mode&0x00ff)=='e') ? "en" : "de", size );
        fwrite( &data[0], 1, size+1, output );
        printf( "\n" );
        CleanArray( data );
    } else
    if( isSwitch('s') ) {
        if( !search('o') ) {
            const uint one = 1;
            setOption( 'o', (cmLn)&one );
        }
        switch( Mode ) {
            case 16484: {
                FILE* output = fopen( getName('d'), "wb" );
                size = crypt64_decryptStdIn( key, output );
                fflush(output); fclose(output);
            } break;
            case 16485: {
                FILE* output = fopen( getName('e'), "w" );
                size = crypt64_encryptStdIn( key, output );
                fflush(output); fclose(output);
            } break;
            case 4196: {
                FILE* output = fopen( getName('d'), "wb" );
                size = crypt64_binary_decryptStdIn( key, output );
                fflush(output); fclose(output);
            } break;
            case 4197: {
                FILE* output = fopen( getName('e'), "w" );
                size = crypt64_binary_encryptStdIn( key, output );
                fflush(output); fclose(output);
            } break;
        }
    }

    crypt64_invalidateKey( key );
    exit( CheckForError() );
}

#endif
