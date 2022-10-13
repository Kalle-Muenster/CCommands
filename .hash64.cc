/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      hash64.cc                                  ||
||     Author:    autogenerated                              ||
||     Generated: by Command Generator v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".hash64.h"
#else
int USAGE( hash64 ) {
    Synopsis("[options] <data>");
	printf("Options\n\n\
    -h <hash> : select hash algorythm to use..\n\
   --help     : show this help screen.\n\n" );
    return CheckForError()||!isModus('help');
}
#endif
#include "C:\LIBRARIES\INCLUDES\GeneralHashFunctions.c"

// regular includes here:
#include <stdlib.h>


typedef union HashValue {
	ulong value;
	uint  parts[2];
} HashValue;


MakeHash64From2xHash32(RSHash)
MakeHash64From2xHash32(JSHash)
MakeHash64From2xHash32(PJWHash)
MakeHash64From2xHash32(ELFHash)
MakeHash64From2xHash32(SDBMHash)
MakeHash64From2xHash32(DJBHash)
MakeHash64From2xHash32(DEKHash)
MakeHash64From2xHash32(BPHash)
MakeHash64From2xHash32(FNVHash)
MakeHash64From2xHash32(APHash)

ulong hash64_BKDRHash( const byte* data, slong size )
{
   //const ulong seed = 1313131313131313131ull;
   ulong hash = 0ull;
   const byte* end = data + size;
   while( data != end ) {
      hash = (hash * 1313131313131313131ull) + *data++;
   } return hash;
}

ulong hash64_BKDRSeed( const byte* data, const slong size, const ulong seed )
{
   ulong hash = 0ull;
   const byte* end = data + size;
   while( data != end ) {
      hash = (hash * seed) + *data++;
   } return hash;
}

ulong hash64_YPSHash( const byte* data, slong size )
{
    ulong hash = 0;
    if( data && size )
        while ( size-- )
           hash = 63 * hash + *data++;
    return hash;
}

Hash64Function
hash64_SelectFunction( fourCC name, ulong seed )
{
	seed &= 0x7fffffffffffffff;
	Hash64Function calc = {NULL,(ulong)name & 0x8000000000000000};
	switch( name ) {
		case 1632129874: calc.call.simple = &hash64_RSHash; break;
		case 1632129866: calc.call.simple = &hash64_JSHash; break;
		case 1213680208: calc.call.simple = &hash64_PJWHash; break;
		case 1212566597: calc.call.simple = &hash64_ELFHash; break;
		case 1380207426: calc.call.seeded = &hash64_BKDRSeed; calc.data.seed = seed; break;
		case 1296188499: calc.call.simple = &hash64_SDBMHash; break;
		case 1212303940: calc.call.simple = &hash64_DJBHash; break;
		case 1212892484: calc.call.simple = &hash64_DEKHash; break;
		case 1632129090: calc.call.simple = &hash64_BPHash; break;
		case 1213615686: calc.call.simple = &hash64_FNVHash; break;
		case 1632129089: calc.call.simple = &hash64_APHash; break;
		case 1213419609: calc.call.simple = &hash64_YPSHash; break;
		default: setError("giebs nich",FourCC("nix"));
	} return calc;
}

#define hash64_CalculateHash( calc, data, size ) calc.data.seed \
                 ? calc.call.seeded( data, size, calc.data.seed ) \
                 : calc.call.simple( data, size )

#ifdef hash64
int main(int argc,char**argv)
{
    if( !InitCommandLiner(argc,argv) ) {
        exit( USAGE(hash64) );
    }

	ulong seed = 0ull;
	if ( hasOption('s') ) {
		if ( isSwitch('s') ) setOption('s',rawNext('s'));
		sscanf( getName('s'),"%ll",&seed );
	}
	
	if (!hasOption('h')) {
		setOption('h',"YPSHash");
	} else
	if ( isSwitch('h') ) {
		setOption('h',rawNext('h'));
	}
	
    if (!search('d')) {
		setOption('d',rawNext('d'));
	}

	if (hasOption('v'))
		showOptions();
	
	
	Hash64Function func = hash64_SelectFunction( *(fourCC*)getName('h'), seed );
	          cmLn data = unQuoted( getName('d') );
	          uint size = strlen(data);
	
    printf( "\ncalculating %s 64bit hash for (%i byte) text:\n", getName('h'), size );
	printf( "\n%s\n\n", getName('d') );
	
	ulong value = hash64_CalculateHash( func, data, size );

	ExitOnError( "Datasize" );
	
	printf("\n%s64 hashvalue QWORD: dec %llu / hex %xll \n\n",getName('h'),value,value);
	exit( CheckForError() );
}
#endif
