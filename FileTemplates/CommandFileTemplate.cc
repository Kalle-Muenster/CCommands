const byte HeaderFile[] = {
"#ifndef using_^CommandName^\n"	
"#define using_^CommandName^ ^CommandName^.cc\n"
"\n"
"\n"
"   		//Todo:...\n"
"\n"
"\n"
"#ifdef _OnTheFly_\n"
"#include \"^CommandName^.cc\"\n"
"#endif\n"
"#endif\n"
};

const byte CodeFile[] = {
"#ifndef _OnTheFly_ \n"
"#include \"^CommandName^.h\"\n"
"#endif \n"
"\n"
"// regular includes here: \n"
"#include <tcclib.h>\n"
"#include \"commandLiner.h\"\n"
"\n"
"//... ...\n"
"\n"
"int main(int argc,char**argv)\n"
"{\n"
"    CommandLineArgs(argc,argv);\n"
"\n"
"	// Todo: implement command\n"
"\n"
};

const byte CodeFileClose[] = {
"}//eof\n"
};
	
const byte CallerFile[] = {
"#define _OnTheFly_\n"
"#include \"^CommandName^.h\"\n"
"int main(int argc,char** argv);\n"
};


// define generater functions:
void createCommandoHeader(char* fileName)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".h");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFile( HeaderFile, &buffer[0] );
}

void createCommandoCode(char* fileName,char insertSnippet)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".cc");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFileConditionalInsert( &CodeFile[0],&CodeFileClose[0],
								   &HalloWorldSnippet[0],
								   insertSnippet,&buffer[0] );
}

void createCommandoCall(char* fileName)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".c");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFileConditionalInsert( "#!/usr/bin/tcc -run\n", 
								   CallerFile, InfoHeader, 
								   NO_INFO, &buffer[0] );
}

void InitCommandoGenerater(void)
{
	generateHeaderFile=&createCommandoHeader;
	generateCodeFile=&createCommandoCode;
	generateExtraFile=&createCommandoCall;
}

