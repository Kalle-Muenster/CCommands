/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      bitField.h                                 ||
||     Author:    Kalle                                      ||
||     Generated: 31.10.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_bitField
#define IncludesFor_bitField (1)

#include <stdlib.h>
#include "eszentielle/.commandlinertypes.h"

#ifndef BITFIELD_ONES
#define BITFIELD_ONES '1'
#endif
#ifndef BITFIELD_ZERO
#define BITFIELD_ZERO '0'
#endif

byte  bitField_flip8( byte value );
word  bitField_flip16( word value );
uint  bitField_flip32( uint value );
ulong bitField_flip64( ulong value );
void* bitField_flip( void* value, int size );

ulong       bitField_byteToString( byte );
const char* bitField_wordToString( word );
const char* bitField_uintToString( uint );
const char* bitField_longToString( ulong );
char*       bitField_dataToString( void*, int );

byte        bitField_stringToByte( const char* );
word        bitField_stringToWord( const char* );
uint        bitField_stringToUInt( const char* );
ulong       bitField_stringToLong( const char* );
byte*       bitField_stringToData( const char*, uint* size );

#ifdef _OnTheFly_
#include ".bitField.cc"
#endif
#endif
