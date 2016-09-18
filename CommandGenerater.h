#ifndef CLASS_FILE_GENERATER 
#define CLASS_FILE_GENERATER CommandGenerater.cc

typedef unsigned char byte;

enum TEMPLATE 
{
	INFO_HEADER,
	HEADER_FILE,
	CODE_FILE,
	CALLER_FILE
};

const byte* getTemplate(int templateType);

#ifdef _OnTheFly_
#include "CommandGenerater.cc"
#endif
#endif