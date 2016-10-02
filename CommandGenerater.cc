#ifndef _OnTheFly_
#include "CommandGenerater.h"
#endif

#include "commandLiner.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void generateFile(const byte*,char*);
void generateFileConditionalInsert( const byte*,const byte*,
							        const byte*,char, char* );
							   
void(*generateHeaderFile)(char*);
void(*generateCodeFile)(char*,char);
void(*generateExtraFile)(char*);



void getCurrentDate(byte*);
int  parseText(const byte*,byte*);

typedef struct tm Date;

enum TOKKEN
{
	COMMANDNAME,
	AUTHOR,
	DATE,
	EXTENSION,
	SNIPPED,

TOKKEN_COUNT};

const char* tokkenStrings[TOKKEN_COUNT] =
{
	"CommandName",
	"Author",
	"Date",
	"Extension",
	"Snipped"
};

byte snippedBuffer[2048]={'\0'};

enum SNIPPED_TOKKEN {
	NONE,
	HALLOWORLD,
	
      SNIPPED_COUNT };

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

typedef enum INSERT_OPTIONS {
	NONE = 0,
	SNIPPED = 1,
	NO_INFO = 2  
} INSERT_OPTIONS;

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


const byte InfoHeader[] = {
		"/*///////////////////////////////////////////////////////////*\\\n"
		"||                                                           ||\n"
		"||     File:      $43^CommandName^^Extension^$||\n"
		"||     Author:    $43^Author^$||\n"
		"||     Generated: $43^Date^$||\n"
		"||                                                           ||\n"
		"\\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"
		"\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/\n\0"
};
#include "FileTemplates/HalloWorldSnippet.cc"
#include "FileTemplates/CommandFileTemplate.cc"
#include "FileTemplates/ClassFileTemplate.cc"
const byte* getTemplate(int templateType)
{
	switch(templateType)
	{
		case INFO_HEADER: return &InfoHeader[0];
		case HEADER_FILE: return &HeaderFile[0];
		case CODE_FILE:   return &CodeFile[0];
		case CALLER_FILE: return &CallerFile[0];
		case CLASS_HEADER:return &ClassHeader[0];
		case CLASS_CODE:  return &ClassCode[0];
		case CLASS_TESTS: return &ClassTests[0];
	}
}

void assignTokken(byte** structure, int tkkn,const byte* stringo)
{
	strcpy(structure[tkkn],stringo);
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

int showHelpScreen(void)
{
	printf("\nusage: \n\n   options:\n\n");
	printf("  --o-path         -  output files to 'path' (otherwhise current directory would be used)\n");
	printf("  --a-author       -  set 'name' used for Author in info\n");
	printf("  --c-command      -  use string 'command' for COMMANDNAME\n");
	printf("      -n           -  creates a C++ class instead of a TCC command\n");
	printf("     --n-namespace -  surround C++ class by namespace 'namespace'\n");
	printf("      -d           -  write current date to info\n");
	printf("      -e           -  generate 'HalloWorld' example instead of empty class files\n");
	printf("      -s           -  silent (no feedback on stdout)\n\n");
	printf("or without dashes, given:\n\n");
	printf("   one parameter  :  parameter will be used as <ClassName>\n");
	printf("   two parameters :  first parameter will be <Namespace>, \n");
	printf("                     second will be <ClassName>\n\n\n");
	
	return hasOption('h') ? 0 : 255;
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


void initialize(void)
{
	for(int i = 0; i < TOKKEN_COUNT; i++)
		Tokken[i] = &_tokkenBuffer[i*128];
	
	for(int i = 0; i < SPACE_COUNT; i++)
		Space.Tokken[i] = &_tokkenBuffer[(int)(i+TOKKEN_COUNT)*128];
	
	Snipped.Tokken[NONE] = &snippedBuffer[0];
	Snipped.Tokken[NONE][0]='\0';
	
	Snipped.Tokken[HALLOWORLD] = &snippedBuffer[MAX_NAME_LENGTH];
	strcpy(Snipped.Tokken[HALLOWORLD],&HalloWorldSnippet[0]);
	
	byte** pTkkn = &Tokken[0];
	assignTokken(pTkkn,COMMANDNAME,"HalloWorld");
	assignTokken(pTkkn,AUTHOR,"Kalle");
	assignTokken(pTkkn,DATE,"by CommandGenerater v0.1");
	assignTokken(pTkkn,EXTENSION,".cpp");
	assignTokken(pTkkn,SNIPPED,"^Snipped.HalloWorld^");
	
	if(hasOption('n'))
		InitClassGenerater();
	else
		InitCommandoGenerater();
}

void getCurrentDate(byte* dst)
{
	sprintf(dst,"heute");
//	Date* t = (Date*)time(NULL);
//	strftime((char*)dst,31,"%d.%m.%Y - %H:%M",t);
}

int main(int argc, char* argv[])
{
	CommandLineArgs(argc,argv);
	
	if(!numGum()||hasOption('h')) 
		return showHelpScreen();

	SILENT_MODE = hasOption('s');
	char InsertOptions=NONE;
	
	getcwd(&outputDirectory[0],128);
	strcat(&outputDirectory[0],"\\");
	
	// handle raw input if 1 or 2 raw arguments given:
	int i = 1;
	if(argc > 2)
		if(argv[2][0] != '-') 
			setOption('n',&argv[i++][0]);
		
	if(argv[i][0] != '-')
		setOption('c',&argv[i++][0]);
	
	initialize();
	
	strcpy(Tokken[DATE],"by Command Generater v.0.1");
	strcpy(Tokken[AUTHOR],"unknown");
	
	
	// handle by commandLiner if tagged input given:
	if(hasOption('n'))
		setNamespace(getName('n'));
	
	if(hasOption('c'))
		Tokken[COMMANDNAME] = getName('c');
	
	if(hasOption('a'))
		Tokken[AUTHOR] = getName('a');
	
	if(hasOption('e'))
		InsertOptions = SNIPPED;
	
	if(hasOption('o'))		
		sprintf(&outputDirectory[0],"%s/",getName('o'));
	
	if(hasOption('d'))
		getCurrentDate(Tokken[DATE]);
	
	if(hasOption('v'))
		showOptions();
	
	// generate the files:
	strcat(&outputDirectory[0],Tokken[COMMANDNAME]);
	generateHeaderFile(&outputDirectory[0]);
	generateCodeFile(&outputDirectory[0],InsertOptions);
	generateExtraFile(&outputDirectory[0]);
}
