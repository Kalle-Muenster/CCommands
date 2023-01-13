/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      bitField.cc                                ||
||     Author:    Kalle                                      ||
||     Generated: 31.10.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".bitField.h"
#else
  #define WITH_STRGPOOL
  #include <.environMentor.h>
  int USAGE(bitField)
  {
    Synopsis("[-d|-f|-i|-l] <value>");
    printf(" Persists:\n\n    ZERO  :  the character which represents No-bits\n             actually set: '%c'\n",BITFIELD_ZERO);
    printf("    ONES  :  the character which represents Yes-bits\n             actually set: '%c'\n\n\n",BITFIELD_ONES);
    return CheckForError()
        || !hasOption('h');
  }
#endif


byte bitField_flip8( byte value )
{
    return ~value;
}

word bitField_flip16( word value )
{
    return ~value;
}

uint bitField_flip32( uint value )
{
    return ~value;
}

ulong bitField_flip64( ulong value )
{
    return ~value;
}

void* bitField_flip( void* data, int size )
{
    if(!(size%8)) {
        ulong* value = (ulong*)data;
        while(size) {
            *value = bitField_flip64(*value);
           --value;
        }
    } else if(!(size%4)) {
        uint* value = (uint*)data;
        while(size) {
            *value = bitField_flip32(*value);
           --value;
        }
    } else if(!(size%2)) {
        word* value = (word*)data;
        while(size) {
            *value = bitField_flip16(*value);
           --value;
        }
    } else {
        byte* value = (byte*)data;
        while(size) {
            *value = bitField_flip8(*value);
           --value;
        }
    } return data;
}

ulong bitField_byteToString( byte val )
{
    ulong data;
    char* text = ((char*)&data);
    for( int i=7; i>=0; --i ) {
        text[i] = ( val & 1 )
                ? BITFIELD_ONES
                : BITFIELD_ZERO;
        val >>= 1;
    } return data;
}

typedef union TwoInt {
    ulong data;
    uint  ints[2];
} TwoInt;

const char* bitField_wordToString( word val )
{
    ulong text[3]; text[2] = 0;
    text[0] = bitField_byteToString( ( (val & 0x0000ff00u) >> 8 ) & 0x000000ffu );
    text[1] = bitField_byteToString( val & 0x000000ffu );

    return pool_set( (const char*)&text[0] );
}

const char* bitField_uintToString( uint val )
{
    TwoInt text;
    pool_ensure( 32 );
    for( int i = 3; i >= 0; --i ) {
        uint shifti = i * 8;
        uint maski = 0x000000ffu << shifti;
        byte value = (byte)((val & maski) >> shifti);
        text.data = bitField_byteToString( value );
        pool_set4( text.ints[0] );
        pool_set4( text.ints[1] );
    } return pool_merge( 8 );
}

const char* bitField_longToString( ulong val )
{
    TwoInt text;
    pool_ensure( 64 );
    for( int i = 7; i >= 0; --i ) {
        const ulong shiftl = (ulong)(i * 8);
        const ulong maskl = (ulong)0xffu << shiftl;
        const byte value = (byte)( (ulong)(val & maskl) >> shiftl );
        text.data = bitField_byteToString( value );
        pool_set8( text.data );
    } return pool_merge( 8 );
}

char* bitField_dataToString( void* data, int size )
{
    char* dst = (char*)junk_allocateJunkChunk(1+(size<<3));
    char* field = dst;
    if(!(size%8)) {
        ulong* value = (ulong*)data;
        while(size) {
            strcpy(field,bitField_longToString(*value));
            field += 64;
            size -= 8;
        }
    } else if(!(size%4)) {
        uint* value = (uint*)data;
        while(size) {
            strcpy(field,bitField_uintToString(*value));
            field += 32;
            size -= 4;
        }
    } else if(!(size%2)) {
        word* value = (word*)data;
        while(size) {
            strcpy(field,bitField_wordToString(*value));
            field += 16;
            size -= 2;
        }
    } else {
        byte* value = (byte*)data;
        while(size) {
            *(ulong*)field = bitField_byteToString(*value);
            field += 8;
            --size;
        }
    } return dst;
}

byte bitField_stringToByte( const char* zerosAndOnes )
{
    const byte ONE = 1u;
    byte retval = 0;
    for( int i = 0; i < 8; ++i ) {
        if ( *zerosAndOnes == BITFIELD_ONES )
            retval |= ( ONE << i );
    } return retval;
}

word bitField_stringToWord( const char* zerosAndOnes )
{
    return bitField_stringToByte( zerosAndOnes+8 ) << 8
         | bitField_stringToByte( zerosAndOnes );
}

uint bitField_stringToUInt( const char* zerosAndOnes )
{
    return bitField_stringToWord( zerosAndOnes+16 ) << 16
         | bitField_stringToWord( zerosAndOnes );
}

ulong bitField_stringToLong( const char* zerosAndOnes )
{
    return bitField_stringToUInt( zerosAndOnes+32 ) << 32
         | bitField_stringToUInt( zerosAndOnes );
}

byte* bitField_stringToData( const char* zerosAndOnes, uint* outSize )
{
    uint length = strlen( zerosAndOnes );
    uint datale = length/8;
    if ( datale != length ) setErrorText( "abgeköpft: multiple of 8");
    byte* data = junk_allocateJunkChunk( datale+1 );
    if(!(datale%64)) { length = 0;
        while( length < datale ) {
            *(ulong*)(data+length)=bitField_stringToLong( zerosAndOnes+(length<<3) );
            length += 8;
        }
    } else if(!(datale%32)) { length = 0;
        while( length < datale ) {
            *(uint*)(data+length)=bitField_stringToUInt( zerosAndOnes+(length<<3) );
            length += 4;
        }
    } else { length = 0;
        while( length < datale ) {
            *(data+length) = bitField_stringToByte( zerosAndOnes+(length<<3) );
            ++length;
        }
    } if ( outSize ) {
        *outSize = datale;
    } return data;
}


#ifdef bitField
// just only main kram...

const static char* indexview[] = {
    "00000000", "76543210",
    "11111100", "54321098",
    "3322222222221111",
    "1098765432109876",
    "66665555555555444444444433333333",
    "32109876543210987654321098765432",
};

const char* renderIndices( int size )
{
    pool_setCheckpoint();
    pool_set("\n  ");
    int line = 0;
    do switch( size ) {
        case 8: pool_set(indexview[line+6]);
        case 4: pool_set(indexview[line+4]);
        case 2: pool_set(indexview[line+2]);
        case 1: pool_set(indexview[line+0]);
        default: pool_set("\n  "); break;
    } while( ++line < 2 );
    return pool_collectCheckpoint();
}

const char* bitsOfTheData( void* data, int size )
{
    switch( size ) {
        case 1: return bitField_byteToString( *(byte*)data );
        case 2: return bitField_wordToString( *(word*)data );
        case 4: return bitField_uintToString( *(uint*)data );
        case 8: return bitField_longToString( *(ulong*)data );
       default: return bitField_dataToString( data, size );
    break; }
}

void drawThisBits( int chart, void* bits )
{
    printf( renderIndices( chart ) );
    printf( "\n  %s\n\n", bitsOfTheData( bits, chart ) );
}

void setViewSigns( cmLn bitSigns )
{
    if ( bitSigns ) {
        if ( strlen(bitSigns) >= 2 ) {
            beginPersistChange(LOCAL);
            setPrefixed("ZERO",setTempf("'%c'",bitSigns[0]));
            printf("Set %s character to be used for 0 bits\n",getTemp());
            setPrefixed("ONES",setTempf("'%c'",bitSigns[1]));
            printf("Set %s character to be used for 1 bits\n",getTemp());
            commitPersistChange();
        } else setError("at least 2 characters",2);
    } else {
        beginPersistChange(LOCAL);
        removePersistEntry("BITFIELD_ZERO");
        removePersistEntry("BITFIELD_ONES");
        commitPersistChange();
    } setError("Successfully set Bit Signs!",0);
}

int main( int argc, char**argv )
{
    if( !InitCommandLiner(argc,argv) ) {
        exit( USAGE(bitField) );
    }

    if( hasOption('v') ) {
        setOption('v',"verbose");
    }

    if( isModus("verbose") ) {
        showOptions();
    }

    if ( hasOption('h') ) {
        setOption('h',"x");
    }

    if ( isModus("set") ) {
        setViewSigns( rawNext('s') );
        exit( CheckForError() );
    }

    int size     = 0;
    ulong fieldL = 0;
    uint  fieldI = 0;
    if( search('f') ) {
      DEBUGLOG("float")
        const char* flupp = getName('f');
      DEBUGLOG("flupp")
        float florp;
        sscanf(flupp,"%f",&florp);
        fieldI = *(uint*)&florp;
        printf( "32bits of float: %f\n\n ", florp );
        if (hasOption('h'))
            size = FourCC("8\0\0\0");
        else size = 4;
    } else
    if( search('d') ) {
        const char* dblrp = getName('d');
        long double florp = strtod( dblrp, dblrp+strlen(dblrp) );
        fieldL = *(ulong*)&florp;
        printf( "64bits of double: %f\n\n ", florp );
        if (hasOption('h'))
            size = FourCC("16\0\0");
        else size = 8;
    } else
    if ( search('i') ) {
        sscanf( getName('i'), "%u", &fieldI );
        printf( "32bits of integer: %u\n\n", fieldI );
        if (hasOption('h'))
            size = FourCC("8\0\0\0");
        else size = 4;
    } else
    if ( search('l') ) {
        sscanf( getName('l'), "%llu", &fieldL );
        printf( "64bit of integer: %llu\n\n", fieldL );
        if (hasOption('h'))
            size = FourCC("16\0\0");
        else size = 8;
    } else {
        cmLn raw = rawName(1);
        if ( !isEmptyArg(raw) ) {
            const char* check = raw-1;
            while( *++check ) if ( *check != BITFIELD_ZERO && *check != BITFIELD_ONES ) setErrorText("ZERO|ONES");
            ExitOnError("Parameter");
            uint dataSize = 0;
            byte* data = bitField_stringToData( raw, &dataSize );
            while (dataSize >= 8) {
                dataSize -= 8;
                printf( "\n%llu\n",*(ulong*)data);
                data += 8;
            }
            while (dataSize >= 4) {
                dataSize -= 4;
                printf( "\n%u\n",*(uint*)data);
                data -= 4;
            }
            while (dataSize >= 2) {
                dataSize -= 2;
                printf( "\n%u\n",*(word*)data);
                data -= 2;
            }
            while (dataSize >= 1) {
                --dataSize;
                printf( "\n%u\n",*data);
                ++data;
            }
        } else {
            setErrorText( "Synopsys: [-d|-f|-i|-l] <value>" );
        }
    }
    ExitOnError( "Parameter" );
    if ( size > 8 ) {
        setOption( 'h', setTempf( "0x%%%sx\n\n", (const char*)&size ) );
        if ( size == *"8" ) {
            printf( getName('h'), fieldI );
        } else {
            printf( getName('h'), fieldL );
        }
    } else {
        // setOption( 'h',"%s\n\n" );
        void* data = (size == 8)
                   ? (void*)&fieldL
                   : (void*)&fieldI;
        drawThisBits( size, data );
    }

    exit( CheckForError() );
}

#endif
