const byte ClassHeader[] = {"\
#ifndef _^COMMANDNAME^_hpp_\n\
#define _^COMMANDNAME^_hpp_\n\
\n\
^Space.LOS^\
^Space.TAB^class ^COMMANDNAME^\n\
^Space.TAB^{\n\
^Space.TAB^public:\n\
^Space.TAB^    ^COMMANDNAME^(void);\n\
^Space.TAB^    virtual ~^COMMANDNAME^(void);\n\
^Space.TAB^};\
^Space.END^\
\n\
#endif\n\
"};

const byte ClassCode[] = {"\
\n#include \"^COMMANDNAME^.hpp\"\n\
\n\
^Space.NAM^^Space.SEP^^COMMANDNAME^::^COMMANDNAME^(void)\n\
{\n\
    // TODO: implement what ever you think...\n\
}\n\
\n\
^Space.NAM^^Space.SEP^^COMMANDNAME^::~^COMMANDNAME^(void)\n\
{\n\
    // TODO: implement what ever you think...\n\
}\n\
\n\
"};

const byte ClassTests[] = {"\
\n#include \"^COMMANDNAME^.hpp\"\n\
\n\
^Space.LOS^\
   //place test-code here:\n\
^Space.END^\
\n\
"};

// define generater functions:
void createClassHeader(char* outputDir)
{
    char buffer[128]={'\0'};
    strcpy(Tokken[EXTENSION],".hpp");
    sprintf( &buffer[0],"%s%s%s", outputDir,
             Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile( &ClassHeader[0], &buffer[0] );
}

void createClassCode(char* outputDir,char insertSnippet)
{
    char buffer[128]={'\0'};
    strcpy(Tokken[EXTENSION],".cpp");
        sprintf( &buffer[0],"%s%s%s", outputDir,
                 Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile(&ClassCode[0], &buffer[0]);
}

void createClassTests(char* outputDir)
{
    char buffer[128]={'\0'};
    strcpy(Tokken[EXTENSION],"_tst.cpp");
        sprintf( &buffer[0],"%s%s%s", outputDir,
                 Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile(&ClassTests[0], &buffer[0]);
}

void InitClassGenerator(void)
{
	if(hasOption('c'))
		assignTokken(&Tokken[0],COMMANDNAME,getName('c'));
	
    generateHeaderFile = &createClassHeader;
    generateCodeFile   = &createClassCode;
    generateExtraFile  = &createClassTests;
}
