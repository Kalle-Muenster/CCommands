/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      ByteOrder.cc                               ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_ 
#include "ByteOrder.h"
#endif 

// regular includes here: 
#include <tcclib.h>
#include "commandLiner.h"


#define asIntegr(stringo) *(unsigned*)&stringo[0]

typedef struct Stringo {
	union {
	unsigned longo;
	byte 	 strongo[9];
};} Stringo; 

const Stringo emptyStringo = {
	"\0\0\0\0\0\0\0\0\0"
};

char* asString(unsigned integer)
{
	((Stringo*)&emptyStringo)->longo = integer;
	return (char*)&emptyStringo.strongo[0];
}

Stringo doStringo(char* toStringo)
{
	toStringo[8]='\0';
	return *(Stringo*)&toStringo[0];
}

unsigned reverse(unsigned input)
{
	int nuller= -1;
	Stringo out = {0};
	Stringo in = {input};
	
	while(in.strongo[7-++nuller]==0);
	int end = 8-nuller;
	in.strongo[end]='\0';
	for(int i=0;i<end;i++)
		out.strongo[i]=in.strongo[(end-1)-i];
	out.strongo[end]='\0';
	return out;
}

int main(int argc,char**argv)
{
    CommandLineArgs(argc,argv);
	
	bool REVERSE = false;
	
	if(hasOption('r')){
		REVERSE = true;
		setOption('r'," reversed");
	}else{
		setOption('r',"");
	}
	
	if(hasOption('i'))
		printf( "\nwill reverse input %s:\n %i\n",
				getName('i'), reverse(atol(getName('i'))) );
	
	if(hasOption('c')) {	
		unsigned value = doStringo(getName('c'));
		value = REVERSE ? reverse(value) : value;                          	
		printf( "\nwill display%s value of string: %s\n %i\n",
				getName('r'), getName('c'), value );
	}
	
	if(hasOption('v')) { 
		unsigned value = !REVERSE ? 
		       atol(getName('v')) : reverse(atol(getName('v')));		   
		printf( "\nwill reinterprete%s integer %s as strig:\n %s\n",
				getName('r'), getName('v'), asString(value) );
	}
	
	if(hasOption('h')||(numGum()==1)) {
		printf("\nHelper-tool for reversing byte order.\n\nUsage:\n\
		--i-<DWord>   - will reverse the given integer...\n\
		--v-<DWord>   - will reinterprete a given integer value to being a string.\n\
		--c-<4char>   - will display the integer value of a string (but not reversed!)\n\
		    -r        - this will reverse while converting when using --c or --v conversion.\n\n");
	}
}//eof
