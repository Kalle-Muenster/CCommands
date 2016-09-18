#ifndef _OnTheFly_
#include "CommandGenerater.h"
#endif

#include "commandLiner.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void generateHeaderFile(char*);
void generateCodeFile(char*,char);
void generateCallerFile(char*);
void getCurrentDate(byte*);
int  parseText(const byte*,byte*);

typedef struct tm Date;

enum TOKKEN
{
	COMMANDNAME,
	DATE,
	NAME,
	EXTENSION,
	SNIPPED,

TOKKEN_COUNT};

const char* tokkenStrings[TOKKEN_COUNT] =
{
	"CommandName",
	"Date",
	"Name",
	"Extension",
	"Snipped"
};

byte snippedBuffer[2048]={'\0'};

const byte Snipped_HalloWorld[]={
"	// an 'advanced' hello world example,\n"
"	// it shows usage of commandLiner.h's parameter handling:\n"
"	//\n"
"	// the user can define additional individuals to let them greet:\n"
"	//    - by passing raw parameters\n	//\n"
"	// and it optionally says 'bye bye' also:\n"
"	//    - by passing switch '-b'\n	//\n"
"	// and to choose the phrase it will use for saying bye:\n"
"	//    - by passing option '--b-<byePhrase>'\n\n\n"
"	// add a default individuum to greet if no others where passed\n"
"    setOption('w',\"World\");\n\n"
"	// output any recognized userinputs when -v (verbose) was passed also\n"
"    if(hasOption('v'))\n"
"        showOptions();\n\n"
"	// look if any additional idividuums to greet where defined:\n"
"    if(numGum())// filter out options, like '-b' or '-v' switchs.\n"
"        for(int i=0;i<numGum();i++)// thees don't need greetings.\n"
"            if( isModus(getNameByIndex(i))\n"
"            && !nameWasSwitched(getNameByIndex(i)))\n"
"                printf(\"hallo %s\\n\",getNameByIndex(i));\n\n"
"	// greet the default 'world' individuum\n"
"	// we just defined before\n"
"    printf(\"Hallo %s\\n\",getName('w'));\n\n"
"	// say 'bye' if '-b' switch was enabled\n"
"    if(hasOption('b')) // use the user choosen phrase,\n"
"        printf(\"...%s\",getName('b')[0] ?// or if none, use the default\n"
"                          getName('b') : \"bye bye!\\n\" );\n\n"
"	// retun some greetings to the system if '-b' switch enabled.\n"
"	return hasOption('b');\n\n"
};

enum SNIPPED_TOKKEN {NONE,HALLOWORLD,SNIPPED_COUNT};

struct Snipped {
	const byte* tokkenNames[SNIPPED_COUNT]; 
	byte* Tokken[SNIPPED_COUNT];
} Snipped = {
	{"NONE","HalloWorld"},
	{"",""}
};

char SILENT_MODE = 0;
char WRITE_DATE = 0;
char TESTSFILE = 0;

char _generaterBuffer[2048] = {'\0'};

const char* templateStuecke[4];

const byte InfoHeader[] = {
		"/*///////////////////////////////////////////////////////////*\\\n"
		"||                                                           ||\n"
		"||     File:      $43^CommandName^^Extension^$||\n"
		"||     Author:    $43^Name^$||\n"
		"||     Generated: $43^Date^$||\n"
		"||                                                           ||\n"
		"\\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"
		"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/\n\0"
};

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
"    pargumnz(argc,argv);\n"
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

const byte* getTemplate(int templateType)
{
	return InfoHeader;
}

enum SPACE
{
	BEGIN,
	END, 
	NAME,
	TAB,
	SEPARATOR,

SPACE_COUNT};

char outputDirectory[128] = {'\0'};

byte _tokkenBuffer[(TOKKEN_COUNT+SPACE_COUNT)*128] = {'\0'};
byte* Tokken[TOKKEN_COUNT];



byte* TEMPLATE_NAMESPACE_BEGIN =
{
	"namespace ^Space.NAME^\n"
	"{\n"
};

byte* TEMPLATE_NAMESPACE_END =
{
	"\n}//end of ^Space.NAME^\n"
};



struct Space
{
	const byte* nameSrings[SPACE_COUNT];
	byte* Tokken[SPACE_COUNT];
} Space = {
	{ "BEGIN", "END", "NAME", "TAB", "SEPARATOR" }, 
	{ "", "", "", "", "" }
};

void assignTokken(byte** structure, int tkkn,const byte* stringo)
{
	strcpy(structure[tkkn],stringo);
}

void initialize(void)
{
	for(int i = 0; i < TOKKEN_COUNT; i++)
		Tokken[i] = &_tokkenBuffer[i*128];
	
	for(int i = 0; i < SPACE_COUNT; i++)
		Space.Tokken[i] = &_tokkenBuffer[(int)(i+TOKKEN_COUNT)*128];
	
	Snipped.Tokken[NONE] = &snippedBuffer[0];
	Snipped.Tokken[NONE][0]='\0';
	
	Snipped.Tokken[HALLOWORLD] = &snippedBuffer[MAX_NAME_LENGTH];
	strcpy(Snipped.Tokken[HALLOWORLD],&Snipped_HalloWorld[0]);
	
	byte** pTkkn = &Tokken[0];
	assignTokken(pTkkn,COMMANDNAME,"HalloWorld");
	assignTokken(pTkkn,NAME,"Kalle");
	assignTokken(pTkkn,DATE,"by ClassFileGenerator v0.1");
	assignTokken(pTkkn,EXTENSION,".cpp");
	assignTokken(pTkkn,SNIPPED,"^Snipped.HalloWorld^");
}

int 
containedTokkenIndex(const byte* prop)
{
	int number = 0;
	do{ if(!strcmp(prop, tokkenStrings[number]))
			return number;
	} while(++number != TOKKEN_COUNT);
	return -1;
}

int
spaceContains(const byte* spaceTokken)
{
	int point = 0, st = 0;
	while(spaceTokken[point++] != '.');
	do{ if(!strcmp(&spaceTokken[point], Space.nameSrings[st]))
		return st;
	} while(++st < SPACE_COUNT);
	return -1;
}

int containsSnippedTokken(const byte* txt)
{
	int point = 0;
	while(txt[point++]!='.');
	for(int i=0;i<SNIPPED_COUNT;i++)
		if(!strcmp(&txt[point],Snipped.tokkenNames[i]))
			return i;
	return -1;
}

void showHelpScreen(void)
{
	printf("\nusage: \n\n   options:\n\n");
	printf("   -d             -  write current date to info\n");
	printf("   -s             -  silent (no feedback on stdout)\n");
	printf("  --c-CommandName -  use string 'CommandName' for COMMANDNAME\n");
//	printf("  --n-namespace   -  surround Class by namespace 'namespace'\n");
	printf("  --a-author      -  set 'name' used for Author in info\n");
	printf("   -e             -  use 'HalloWorld' example instead of generating empty class files\n");
	printf("  --o-path        -  output files to 'path' instead to the current directory\n\n");
	printf("or without dashes, given:\n\n");
	printf("   one parameter  :  parameter will be used as <ClassName>\n");
	printf("   two parameters :  first parameter will be <Namespace>, \n");
	printf("                     second will be <ClassName>\n\n\n");
}

void setNamespace(const byte* nameSpace)
{
	byte** ptkkn = &Space.Tokken[0];
	assignTokken(ptkkn,NAME,nameSpace);
	assignTokken(ptkkn,BEGIN,&TEMPLATE_NAMESPACE_BEGIN[0]);
	assignTokken(ptkkn,END,&TEMPLATE_NAMESPACE_END[0]);
	assignTokken(ptkkn,TAB,"    ");
	assignTokken(ptkkn,SEPARATOR,"::");
}

byte* Expand(byte* tokkentext)
{
	int t; 
	if((t = containedTokkenIndex(tokkentext))>=0)
		return Tokken[t];
	else if((t = spaceContains(tokkentext))>=0)
		return Space.Tokken[t];
	else if((t = containsSnippedTokken(tokkentext))>=0)
		return Snipped.Tokken[t];
	return "";
}

unsigned outPosition=0;

int
parseText(const byte* text,byte* parsed)
{
	unsigned i, lengthCount;
	lengthCount = i = 0;
	byte parserBuffer[2048]={'\0'};
	int parsedPosition = 0;
	while (text[i]!='\0')
	{
		if (text[i] == '^')
		{
			byte tokken[256]={'\0'};
			unsigned search = i;
			byte sign = text[search];
			int pT=0;
			while (text[++search] != sign)
				tokken[pT++]=text[search];
			tokken[pT]='\0';
			i = search;
			search = parseText(Expand(&tokken[0]),&parserBuffer[0]);			
			parserBuffer[search]='\0';
			search = 0;
			do{	parsed[parsedPosition++]=parserBuffer[search];
			} while(parserBuffer[++search]!='\0');
		}
		else if(text[i] == '$')
		{
			byte digit;
			int length = 0;
			byte subTokken[256]={'\0'};
			int pT=0;
			while((digit=text[++i]) != '$') {
				if(digit >= 0x30 && digit <= 0x39)
					length = (digit - 48) + 10 * length;
				else
					subTokken[pT++] = digit;
			} 
			subTokken[pT]='\0';
			digit = parseText(&subTokken[0],&parserBuffer[0]);
			parserBuffer[digit] = '\0';
			
			pT=0;
			do{ parsed[parsedPosition++]=parserBuffer[pT];
			} while(parserBuffer[++pT]!='\0');
			length -=  pT;
			for(int c = 0; c < length; c++)
				parsed[parsedPosition++]=' ';
			parsed[parsedPosition]='\0';
		} 
		else 
			parsed[parsedPosition++]=text[i];
		
		i++;
	}
	parsed[parsedPosition]='\0';
	return parsedPosition;
}

typedef enum INSERT_OPTIONS {
	NONE = 0,
	SNIPPED = 1,
	NO_INFO = 2  
} INSERT_OPTIONS;

void
generateFile(const byte* text, char* fileName)
{
	FILE* file = fopen(fileName, "wb");
	
	char writeBuffer[2048] = {'\0'};
	parseText(InfoHeader,&writeBuffer[0]);
	fprintf(file, "%s", &writeBuffer[0]);
	
	int NullTerminator = parseText(text,&writeBuffer[0]);
	writeBuffer[NullTerminator] = '\0';
	fprintf(file, "%s", &writeBuffer[0]);
	
	fflush(file); fclose(file);
	if(!SILENT_MODE)
		printf("Created File: %s\n", fileName);
}

void
generateFileConditionalInsert( const byte* textA,
							   const byte* textB,
							   const byte* insert,
							   char  options,
							   char* fileName )
{
	FILE* file = fopen(fileName, "wb");
	
	char writeBuffer[4096] = {'\0'};
	char* outBuffer = &writeBuffer[0];
	
	if(!(options==NO_INFO))
	{
		parseText(InfoHeader,outBuffer);
		fprintf(file, "%s", outBuffer);
		fflush(file);
		outBuffer = &writeBuffer[0];
	}
	
	int NullTerminator = parseText(textA,outBuffer);
	outBuffer[NullTerminator] = '\0';
	fprintf(file, "%s", outBuffer);
	fflush(file);
	outBuffer = &writeBuffer[0];
	
	if(options)
	{
		NullTerminator = parseText(insert,outBuffer);
		outBuffer[NullTerminator] = '\0';
		fprintf(file, "%s", outBuffer);
		fflush(file);
		outBuffer = &writeBuffer[0];
	}
	
	NullTerminator = parseText(textB,outBuffer);
	outBuffer[NullTerminator] = '\0';
	fprintf(file, "%s", outBuffer);
	
	fflush(file); fclose(file);
	if(!SILENT_MODE)
		printf("Created File: %s\n", fileName);
}



void generateHeaderFile(char* fileName)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".h");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFile( HeaderFile, &buffer[0] );
}

void generateCodeFile(char* fileName,char insertSnippet)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".cc");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFileConditionalInsert( &CodeFile[0],&CodeFileClose[0],
								   &Snipped_HalloWorld[0],
								   insertSnippet,&buffer[0] );
}

void generateCallerFile(char* fileName)
{
	char buffer[128]={'\0'};
	strcpy(Tokken[EXTENSION],".c");
	sprintf(&buffer[0],"%s%s",fileName , Tokken[EXTENSION]);
	generateFileConditionalInsert( "#!/usr/bin/tcc -run\n", 
								   CallerFile, InfoHeader, 
								   NO_INFO, &buffer[0] );
}

void getCurrentDate(byte* dst)
{
//	Date* t = (Date*)time(NULL);
//	strftime((char*)dst,31,"%d.%m.%Y - %H:%M",t);
}

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{ showHelpScreen();	return 0; }

	initialize();
	
	pargumnz(argc,argv);

	SILENT_MODE = hasOption('s');
	char InsertOptions=NONE;
	
	getcwd(&outputDirectory[0],128);
	strcat(&outputDirectory[0],"\\");
	
	int i = 1;
	if(argc > 2)
		if(argv[2][0] != '-')
			setNamespace( &argv[i++][0] );
		
	if(argv[i][0] != '-')
		strcpy(Tokken[COMMANDNAME], &argv[i++][0] );
	
	if(hasOption('n'))
		setNamespace(getName('n'));
	
	if(hasOption('c'))
		Tokken[COMMANDNAME]=getName('c');
	
	if(hasOption('a'))
		Tokken[NAME]=getName('a');
	
	if(hasOption('e'))
		InsertOptions=SNIPPED;
	
	if(hasOption('o'))		
		sprintf(&outputDirectory[0],"%s/",getName('o'));
	
	if(hasOption('d'))
		getCurrentDate(Tokken[DATE]);
	
	if(hasOption('v'))
		showOptions();
	

	strcat(&outputDirectory[0],Tokken[COMMANDNAME]);
	generateHeaderFile(&outputDirectory[0]);
	generateCodeFile(&outputDirectory[0],InsertOptions);
	generateCallerFile(&outputDirectory[0]);
}
