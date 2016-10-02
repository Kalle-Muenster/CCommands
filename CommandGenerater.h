#ifndef CLASS_FILE_GENERATER 
#define CLASS_FILE_GENERATER CommandGenerater.cc
//#include "CommandGenerater.c"

typedef unsigned char byte;

enum TEMPLATE 
{
	INFO_HEADER,
	HEADER_FILE,
	CODE_FILE,
	CALLER_FILE,
	CLASS_HEADER,
	CLASS_CODE,
	CLASS_TESTS
};

const byte* getTemplate(int templateType);

#ifdef _OnTheFly_
#include "CommandGenerater.cc"
#endif
#endif