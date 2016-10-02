const byte ClassHeader[] = {
		"#ifndef _^CommandName^_hpp_\n"
		"#define _^CommandName^_hpp_\n"
		"\n"
		"^Space.BEGIN^"
		"^Space.TAB^class ^CommandName^\n"
		"^Space.TAB^{\n"
		"^Space.TAB^public:\n"
		"^Space.TAB^	^CommandName^(void);\n"
		"^Space.TAB^	virtual ~^CommandName^(void);\n"
		"^Space.TAB^};\n"
		"^Space.END^"
		"\n"
		"#endif\n"
};
const byte ClassCode[] = {
		"\n#include \"^CommandName^.hpp\"\n"
		"\n"
		"^Space.NAME^^Space.SEPARATOR^^CommandName^::^CommandName^(void)\n"
		"{\n"
		"    // TODO: implement what ever you think...\n"
		"}\n"
		"\n"
		"^Space.NAME^^Space.SEPARATOR^^CommandName^::~^CommandName^(void)\n"
		"{\n"
		"    // TODO: implement what ever you think...\n"
		"}\n"
		"\n"
};
const byte ClassTests[] = {
		"\n#include \"^CommandName^.hpp\"\n"
		"\n"
		"^Space.BEGIN^"
		"	//place test-code here:\n"
		"^Space.END^"
		"\n"
};


// define generater functions:
void createClassHeader(char* fileName)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".hpp");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFile( &ClassHeader[0], &buffer[0] );
}

void createClassCode(char* fileName,char insertSnippet)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".cpp");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFile(&ClassCode[0], &buffer[0]);
}

void createClassTests(char* fileName)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],"_tst.cpp");
	sprintf(&buffer[0],"%s%s",fileName ,Tokken[EXTENSION]);
	generateFile(&ClassTests[0], &buffer[0]);
}

void InitClassGenerater(void)
{
	generateHeaderFile=&createClassHeader;
	generateCodeFile=&createClassCode;
	generateExtraFile=&createClassTests;
}