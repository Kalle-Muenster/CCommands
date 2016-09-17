#ifndef CLASS_FILE_GENERATER 
#define CLASS_FILE_GENERATER ClassFileGenerater.cc

#include "ClassFileGenerater.c"

typedef unsigned char byte;

enum TEMPLATE 
{
	INFO_HEADER,
	HEADER_FILE,
	CODE_FILE,
	CALLER_FILE
};

void 		generateHeaderFile(char*);
void 		generateCodeFile(char*,char);
void 		generateTestFile(char*);
const byte* getTemplate(int templateType);
void 		getCurrentDate(byte*);
int  		parseText(const byte*,byte*);
	
#ifdef _OnTheFly_
#include "ClassFileGenerater.cc"
#endif
#endif