#ifndef _OnTheFly_
 #include ".generate.h"
#endif

#include ".environMentor.h"
#include ".commandLiner.h"
#include ".stringPool.h"

#if !defined(CODE_AUTHOR)
     #define CODE_AUTHOR "autogenerated"
#endif

# ifndef USE_BASE64_ENCODING
 #define USE_BASE64_ENCODING (0)
# endif

# ifndef USE_MULTIPROP_LISTS
 #define USE_MULTIPROP_LISTS (0)
# endif

#if USE_BASE64_ENCODING > 0
 #include ".base64.h"
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>

# ifndef ESCAPE_CHAR_SIMPLE
 #define ESCAPE_CHAR_SIMPLE '^'
# endif
# ifndef ESCAPE_CHAR_LENGTH
 #define ESCAPE_CHAR_LENGTH '$'
# endif
# ifndef PARSER_BUFFER_SIZE
 #define PARSER_BUFFER_SIZE 1024
# endif
# ifndef TABULATOR_SEQUENCE
 #define TABULATOR_SEQUENCE "    "
# endif

const short MULTI_ESCAPERS[2] = { ('>'|('|'<<8)), ('|'|('<'<<8)) };

void(*generateHeaderFile)(char*);
void(*generateCodeFile)(char*,char);
void(*generateExtraFile)(char*);

void generateFile(const byte*,char*);
void generateFileConditionalInsert( const byte*,const byte*,
                                    const byte*,char, char* );
int   parseText(const byte*,byte*);
byte* Expand(byte* tokkentext);

typedef struct tm Date;

enum TOKKEN {
    COMMANDNAME,
    AUTHOR,
    DATE,
    EXTENSION,
    SNIPPED,
TOKKEN_COUNT
};

const char* tokkenStrings[TOKKEN_COUNT] = {
    "COMMANDNAME",
    "AUTHOR",
    "DATE",
    "EXTENSION",
    "SNIPPED"
};

enum SNIPPED_TOKKEN {
  NONE,
  EXAMPLECODE,
  DATASIZE,
  DATATYPE,
  DATANAME,
  WRAPPEDLINE,
SNIPPED_COUNT
};

#ifndef MAX_SNIPPED_LENGTH
#define MAX_SNIPPED_LENGTH ( MAX_NAM_LEN * SNIPPED_COUNT )
#endif

byte snippedBuffer[MAX_SNIPPED_LENGTH]={'\0'};

struct Snipped {
    const byte* tokkenNames[SNIPPED_COUNT];
    byte* Tokken[SNIPPED_COUNT];
} Snipped = {
    {
        "NONE",
        "EXAMPLECODE",
        "DATASIZE",
        "DATATYPE",
        "DATANAME",
        "WRAPPEDLINE"
    },{
        &snippedBuffer[0],
        &snippedBuffer[MAX_NAM_LEN],
        &snippedBuffer[DATASIZE*MAX_NAM_LEN],
        &snippedBuffer[DATATYPE*MAX_NAM_LEN],
        &snippedBuffer[DATANAME*MAX_NAM_LEN],
        &snippedBuffer[WRAPPEDLINE*MAX_NAM_LEN]
    }
};

char SILENT_MODE = 0;
char WRITE_DATE = 0;
char TESTSFILE = 0;

char _generaterBuffer[PARSER_BUFFER_SIZE] = {'\0'};

enum SPACE {
    LOS,END,NAM,
    TAB,SEP,USE,
SPACE_COUNT };

char outputDirectory[MAX_NAM_LEN] = {'\0'};

byte _tokkenBuffer[(TOKKEN_COUNT+SPACE_COUNT)*MAX_NAM_LEN] = {'\0'};
byte* Tokken[TOKKEN_COUNT];

typedef enum INSERT_OPTIONS {
    INSERT_NOTHING = 0,
    INSERT_SNIPPED = 1,
    INSERT_NOINFO = 2
} INSERT_OPTIONS;

byte* TEMPLATE_NAMESPACE_LOS =
      { "namespace ^Space.NAM^ {" };
byte* TEMPLATE_NAMESPACE_USE =
      { "using namespace ^Space.NAM^;\n" };
byte* TEMPLATE_NAMESPACE_END =
      { "} //end of ^Space.NAM^\n" };

struct Space
{
    const byte* nameSrings[SPACE_COUNT];
    byte* Tokken[SPACE_COUNT];
} Space = {
    { "LOS", "END", "NAM", "TAB", "SEP", "USE" },
    { &_tokkenBuffer[(TOKKEN_COUNT+LOS)*MAX_NAM_LEN],
      &_tokkenBuffer[(TOKKEN_COUNT+END)*MAX_NAM_LEN],
      &_tokkenBuffer[(TOKKEN_COUNT+NAM)*MAX_NAM_LEN],
      &_tokkenBuffer[(TOKKEN_COUNT+TAB)*MAX_NAM_LEN],
      &_tokkenBuffer[(TOKKEN_COUNT+SEP)*MAX_NAM_LEN],
      &_tokkenBuffer[(TOKKEN_COUNT+USE)*MAX_NAM_LEN], }
};

#define MULTI_COUNT 6
#define COUNTMULTI(i) ((6*MAX_NAM_LEN)+4*i)
byte  _multiTokkenData[6*MAX_NAM_LEN+6*4];
#define MULTI(off) (int)((((ptval)(byte*)off)-((ptval)(byte*)&Multi.LINE))/4)
#define CULTI(off) Multi.Tokken[6+MULTI(off)]
struct MultiStruct {
    const unsigned LINE;
    const unsigned LIST;
    const unsigned PASS;
    const unsigned PROP;
    const unsigned SPEC;
    const unsigned STOP;
    byte* Tokken[6*2];
    const unsigned ZAHL;
} Multi = {
    1162758476u,
    1414744396u,
    1397965136u,
    1347375696u,
    1128616019u,
    1347376211u,
    { &_multiTokkenData[0*MAX_NAM_LEN],
      &_multiTokkenData[1*MAX_NAM_LEN],
      &_multiTokkenData[2*MAX_NAM_LEN],
      &_multiTokkenData[3*MAX_NAM_LEN],
      &_multiTokkenData[4*MAX_NAM_LEN],
      &_multiTokkenData[5*MAX_NAM_LEN],
      &_multiTokkenData[COUNTMULTI(0)],
      &_multiTokkenData[COUNTMULTI(1)],
      &_multiTokkenData[COUNTMULTI(2)],
      &_multiTokkenData[COUNTMULTI(3)],
      &_multiTokkenData[COUNTMULTI(4)],
      &_multiTokkenData[COUNTMULTI(5)] },
    1279803738u
};


void assignTokken(byte** structure, int tkkn,const byte* replacement)
{
    strcpy( structure[tkkn], replacement );
}

const byte InfoHeader[] = {
        "/*///////////////////////////////////////////////////////////*\\\n"
        "||                                                           ||\n"
        "||     File:      $43^COMMANDNAME^^EXTENSION^$||\n"
        "||     Author:    $43^AUTHOR^$||\n"
        "||     Generated: $43^DATE^$||\n"
        "||                                                           ||\n"
        "\\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"
        "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/\n\0"
};

#include "FileTemplates/HalloWorldSnippet.cc"
#include "FileTemplates/CommandFileTemplate.cc"
#include "FileTemplates/ClassFileTemplate.cc"
#if USE_MULTIPROP_LISTS > 0
#include "FileTemplates/ResourceMultiFileTemplate.cc"
#else
#include "FileTemplates/ResourceFileTemplate.cc"
#endif
#include "FileTemplates/WrapperTemplate.cc"
#include "FileTemplates/BuildScriptTemplate.cc"
#include "FileTemplates/ControllerTemplate.cc"

int containedTokkenIndex(const byte* prop)
{
    int number = 0;
    do{ if( !stringCompare( prop, tokkenStrings[number] ) )
            return number;
    } while( ++number != TOKKEN_COUNT );
    return -1;
}

int spaceContains(const byte* spaceTokken)
{
    int point = 0, st = 0;
    while( spaceTokken[point++] != '.' );
    do { if( !stringCompare( &spaceTokken[point], Space.nameSrings[st] ) )
            return st;
    } while( ++st < SPACE_COUNT );
    return -1;
}

int containsSnippedTokken(const byte* txt)
{
    int point = 0;
    while( txt[point++] != '.' );
    for( int i = 0; i < SNIPPED_COUNT; i++ ) {
        if( !stringCompare( &txt[point], Snipped.tokkenNames[i] ) )
            return i;
    } return -1;
}

int multipTokkenSlots(byte* multiTokken)
{
    int point = 0;
	
    while( multiTokken[point++] != '.' );
	unsigned fourccvalue = FourCC( &multiTokken[point] );
	int counter = -1;
    if( fourccvalue == Multi.ZAHL ) {
		byte mtokbuf[64];
		strcpy( &mtokbuf[0], &multiTokken[point] );
		printf("Multi.ZAHL: %s\n",&mtokbuf[0]);
        return multipTokkenSlots( &mtokbuf[0] ) + MULTI_COUNT;
    } else {
	    if (multiTokken[point+4]=='.') {
			multiTokken[point+4]='\0';
			char* gum = (char*)&multiTokken[point+5];
			char num[4] = {gum[0],gum[1],gum[2],gum[3]};
			gum = &num[0];
			while( ++gum != '|' );
			*gum = '\0';
			counter = atoi( &num[0] );
			printf("Multi.Digit %i!\n",counter);
		}
	}
	printf("nicht: Multi.ZAHL...\n");
    for( const unsigned* it = &Multi.LINE; it <= &Multi.STOP; ++it ) {
		unsigned flup = *it;
		int idx = MULTI(it);
		printf( "check: index %i\n", idx );
		printf( "check: Multi.%c%c%c%c...\n", flup&0x000000ffu, (flup&0x0000ff00u)>>8, (flup&0x00ff0000u)>>16, (flup&0xff000000u)>>24 );
        if ( flup == fourccvalue ) {
			printf( "found!\n" );
			if ( counter >= 0 )
				itoa( counter+1, Multi.Tokken[idx+6], 10 );
			return idx;
		}
    } printf("not found!\n"); 
	return -1;
}

int USAGE(generate)
{
    Synopsis("[<command> | <namespace> <class>] [options]");
    printf("\n options:\n\n");
    printf("  --c-command      -  use string 'command' for COMMANDNAME/ClassName\n");
    printf("     -e            -  generate 'HalloWorld' example instead of an empty command\n\n");
    printf("  --n-space        -  creates C++ class in namespace <space> instead of tiny-C code\n");
    printf("     -n            -  creates C++ class without surrounding it by any namespace\n\n");
    printf("  --r-filename     -  creates a resource-set,and add resource: 'filename' to it\n");
#if USE_BASE64_ENCODING > 0
    printf("     --B64         -  creates base64 string resources instead of binary data.\n\n");
#endif
    printf("  --b-buildscript  -  create a build script named 'buildscript' (very experimental)\n\n");
    printf("  --l-wrapline     -  creates an executable which just wraps 'wrapline'\n");
    printf("     --Async       -  creates code which executes 'wrapline' by not waiting for return\n\n");
    printf("  --C-controller   -  creates a Stepflow::Controller mode-class:\n");
    printf("                      as --n-<namespace>::--C-<controller> derived from --c-<basemode>\n\n");
    printf(" or usage without dashes, given:\n\n");
    printf("   one parameter   -  parameter will be used as <Command>\n");
    printf("   two parameters  -  first parameter will be <Namespace>, \n");
    printf("                      second will be <ClassName>\n\n");
    printf(" these apply to all of the above:\n\n");
    printf("   --a-author      -  set name <author> to be used as: 'Author' in info-headers\n");
    printf("    -d             -  writes the current date to info-headers\n");
    printf("    -s             -  silent mode(no feedback on stdout)\n");
    printf("   --o-path        -  output files to <path> instead of placing them into cwd\n\n");
	printf(" persists:\n\n");
	printf("  --setBase64 <on|off>    -  enables support for base64 packaged resources\n");
	printf("  --setTabChars <string>  -  setup a sequence of chars used for line-einzug\n");
	printf("  --setMultiMode <on|off> -  enables support for multiple replacement lists\n\n");
    if( hasOption('v') )
        showOptions();
    return hasOption('h')
         ? EXIT_SUCCESS
         : getErrorCode();
}

void generate_setNamespace( const byte* nameSpace )
{
    const char nuller[2] = {17,'\0'};
    byte** ptkkn = &Space.Tokken[0];
    if((nameSpace[0]=='\0')||(&nameSpace[0]==NoString)) {
        assignTokken(ptkkn,NAM,NoString);
        assignTokken(ptkkn,LOS,NoString);
        assignTokken(ptkkn,END,NoString);
        assignTokken(ptkkn,TAB,NoString);
        assignTokken(ptkkn,SEP,NoString);
        assignTokken(ptkkn,USE,NoString);
    } else {
        assignTokken(ptkkn,NAM,nameSpace);
        assignTokken(ptkkn,LOS,&TEMPLATE_NAMESPACE_LOS[0]);
        assignTokken(ptkkn,END,&TEMPLATE_NAMESPACE_END[0]);
        assignTokken(ptkkn,USE,&TEMPLATE_NAMESPACE_USE[0]);
        assignTokken(ptkkn,TAB,TABULATOR_SEQUENCE);
        assignTokken(ptkkn,SEP,"::");
    }
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
    else if((t = multipTokkenSlots(tokkentext))>=0)
        return Multi.Tokken[t];
    else
        return "";
}

// unsigned outPosition=0;



int
parseText( const byte* src, byte* dst )
{
    DEBUGFMT("%s",src);
    const char nuller[2] = {17,'\0'};
    unsigned i, lengthCount;
    lengthCount = i = 0;
    byte parserBuffer[PARSER_BUFFER_SIZE]={'\0'};
    int parsedPosition = 0;
    while (src[i]!='\0')
    {
        if (src[i] == ESCAPE_CHAR_SIMPLE)
        {
            byte tokken[MAX_NAM_LEN]={'\0'};
            unsigned search = i;
            byte sign = src[search];
            int pT=0;
            while (src[++search] != sign)
                tokken[pT++]=src[search];
            tokken[pT]='\0';
            i = search;
            search = parseText(Expand(&tokken[0]),&parserBuffer[0]);
            parserBuffer[search]='\0';
            search = 0;
            do{ if ( *(unsigned short*)&parserBuffer[search] != *(unsigned short*)NoString ) {
                    dst[parsedPosition++]=parserBuffer[search++];
                } else search+=2;
            } while(parserBuffer[search]!='\0');
        }
        else if(src[i] == ESCAPE_CHAR_LENGTH)
        {
            byte digit;
            int length = 0;
            byte subTokken[MAX_NAM_LEN]={'\0'};
            int pT=0;
            while((digit=src[++i]) != ESCAPE_CHAR_LENGTH) {
                if(digit >= 0x30 && digit <= 0x39)
                    length = (digit - 48) + 10 * length;
                else
                    subTokken[pT++] = digit;
            }
            subTokken[pT]='\0';
            digit = parseText(&subTokken[0],&parserBuffer[0]);
            parserBuffer[digit] = '\0';
            pT=0;
            do{ dst[parsedPosition++]=parserBuffer[pT];
            } while(parserBuffer[++pT]!='\0');
            length -=  pT;
            for(int c = 0; c < length; c++)
                dst[parsedPosition++]=' ';
            dst[parsedPosition]='\0';
        }
        else if ( *(short*)&src[i] == MULTI_ESCAPERS[0] )
        {
			printf("MULTIP-TOKEN!\n");
            byte tokken[MAX_NAM_LEN]={'\0'};
            unsigned search = i+2;
            byte sign = src[search];
            int pT=0;
            while ( *(short*)&src[++search] != MULTI_ESCAPERS[1] ) {
                tokken[pT++]=src[search];
            } 
			printf("Multi -> close: pT%i\n",pT);
            tokken[pT]='\0';
            i = ++search;
            printf("%s\n",&tokken[0]);
            search = parseText( Expand( &tokken[0] ), &parserBuffer[0] );
            parserBuffer[search]='\0';
            printf("search: %i\n",search);
            search = 0;
            do{ dst[parsedPosition++]=parserBuffer[search++];
            } while(parserBuffer[search]!='\0');
            printf("buffer: %s\n",&parserBuffer[0]);
        }
        else
        {
            dst[parsedPosition++]=src[i];
        } i++;
    }
    dst[parsedPosition]='\0';
    DEBUGFMT("%s",&dst[0]);
    return parsedPosition;
}

const char*
generate_parseTemplate( const byte* teplt )
{
	char* parserbuffer = junk_allocateJunkChunk( strlen(teplt)*2 );
    parserbuffer[ parseText(teplt,parserbuffer) ] = '\0';
	return parserbuffer;
}

void
generateFile(const byte* text, char* fileName)
{
    DEBUGFMT("opening file for writing: %s",fileName)
    FILE* file = fopen(fileName, "w");
	int buffersize = strlen(text) * 2;
	buffersize = buffersize < 512 ? 512 : buffersize;
    DEBUGLOG("parsing")
	char writeBuffer[ buffersize ];
    parseText( InfoHeader, &writeBuffer[0] );
    DEBUGLOG("writing")
    fprintf( file, "%s", &writeBuffer[0] );
    DEBUGLOG("parsing")
    int NullTerminator = parseText( text, &writeBuffer[0] );
    DEBUGFMT("parser output: %i chars",NullTerminator)
    writeBuffer[NullTerminator] = '\0';
	char* output = &writeBuffer[0];
	while( *output ) {
		switch( *(word*)output ) {
			case '<'|('|'<<8): *(word*)output = MULTI_ESCAPERS[0]; break;
			case '|'|('>'<<8): *(word*)output = MULTI_ESCAPERS[1]; break;
			default: break;
		} ++output;
	}
    DEBUGLOG("writing")
    fprintf(file, "%s", &writeBuffer[0]);
    DEBUGFMT("closing file: %s",fileName)
    fflush(file); fclose(file);
    if(!SILENT_MODE)
        printf("Created File: %s\n", fileName);
}

void
generateFileConditionalInsert( const byte* textA,
                               const byte* textB,
                               const byte* insert,
                               char  insertions,
                               char* fileName )
{
    DEBUGFMT("opening file for writing: %s",fileName)
    FILE* file = fopen(fileName, "w");
	
	int buffersize = (strlen(textA) + strlen(textB) + strlen(insert)) * 2;
	buffersize = buffersize < 512 ? 512 : buffersize;
    char writeBuffer[buffersize];
    char* outBuffer = &writeBuffer[0];

    if(!(insertions==INSERT_NOINFO))
    {
        DEBUGLOG("parsing")
        parseText(InfoHeader,&writeBuffer[0]);
        DEBUGLOG("writing")
        fprintf(file, "%s", &writeBuffer[0]);
        fflush(file);
    }
    DEBUGLOG("parsing")
	outBuffer = &writeBuffer[0];
		while( *outBuffer ) {
			switch( *(word*)outBuffer ) {
			case '<'|('|'<<8): *(word*)outBuffer = MULTI_ESCAPERS[0]; break;
			case '|'|('>'<<8): *(word*)outBuffer = MULTI_ESCAPERS[1]; break;
			default: break;
			} ++outBuffer;
		}
    writeBuffer[parseText(textA,&writeBuffer[0])] = '\0';
    DEBUGLOG("writing")
    fprintf(file, "%s", &writeBuffer[0]);
    fflush(file);
    outBuffer = &writeBuffer[0];

    if(insertions) {
        DEBUGLOG("inserting")
        if(!hasOption('r')) {
            DEBUGLOG("parsing")
            outBuffer[parseText(insert,outBuffer)] = '\0';
            DEBUGLOG("writing")
            while( *outBuffer ) {
				switch( *(word*)outBuffer ) {
				case '<'|('|'<<8): *(word*)outBuffer = MULTI_ESCAPERS[0]; break;
				case '|'|('>'<<8): *(word*)outBuffer = MULTI_ESCAPERS[1]; break;
				default: break;
				} ++outBuffer;
			}
            fprintf(file, "%s", &writeBuffer[0]);
            fflush(file);
            outBuffer = &writeBuffer[0];
        } else { // if hasOption('r')
            unsigned size = strlen(insert);
            DEBUGLOG("writing")
            fwrite(insert,1,size,file);
            fflush(file);
        }
    }

    DEBUGLOG("parsing")
    outBuffer[parseText(textB,outBuffer)] = '\0';
    DEBUGLOG("writing")
	while( *outBuffer ) {
		switch( *(word*)outBuffer ) {
		case '<'|('|'<<8): *(word*)outBuffer = MULTI_ESCAPERS[0]; break;
		case '|'|('>'<<8): *(word*)outBuffer = MULTI_ESCAPERS[1]; break;
		default: break;
		} ++outBuffer;
	}
    fprintf(file, "%s",  &writeBuffer[0]);

    DEBUGFMT("closing file: %s",fileName)
    fflush(file); fclose(file);
    if(!SILENT_MODE)
        printf("Created File: %s\n", fileName);
}

#include <time.h>

const char* getCurrentDateTime(void)
{
    setOption('d',"Heute");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf( getName('d'), "%02i.%02i.%04i",
             tm.tm_mday, tm.tm_mon + 1,
             tm.tm_year + 1900 );
    DEBUGFMT( "date is: %s", getName('d') );
    return getName('d');
}

char prepareReplacements(void)
{
 // initialize buffers and setup any defaults:

    for(int i = 0; i < TOKKEN_COUNT; i++)
        Tokken[i] = &_tokkenBuffer[i*MAX_NAM_LEN];

  //  for(int i = 0; i < SPACE_COUNT; i++)
  //      Space.Tokken[i] = &_tokkenBuffer[(int)(i+TOKKEN_COUNT)*MAX_NAM_LEN];

    Snipped.Tokken[NONE] = &snippedBuffer[0];
    Snipped.Tokken[NONE][0]='\0';

  //  if(hasOption('e')) {
  //      Snipped.Tokken[EXAMPLECODE] = &snippedBuffer[MAX_NAM_LEN];
  //      strcpy(Snipped.Tokken[EXAMPLECODE],&HalloWorldSnippet[0]);
  //  }


  //  assignTokken(&Tokken[0],COMMANDNAME,"EXAMPLECODE");
    assignTokken(&Tokken[0],AUTHOR,CODE_AUTHOR);
    assignTokken(&Tokken[0],DATE,"by Command Generater v.0.1");
    assignTokken(&Tokken[0],EXTENSION,".cpp");
    assignTokken(&Tokken[0],SNIPPED,"^SNIPPED.EXAMPLECODE^");

    for ( const uint* it = &Multi.LINE; it <= &Multi.STOP; ++it )
        strcpy( CULTI(it), "0" );

 // decide which generator template to choose:
    if( hasOption('C') ) {
        InitControllerGenerator();
    } else if(hasOption('n') && (!hasOption('r')) ) {
        InitClassGenerator();
        generate_setNamespace(getName('n'));
    } else if(hasOption('r')) {
        InitResourceGenerater();
        if(hasOption('n'))
            generate_setNamespace(getName('n'));
    } else if(hasOption('l')) {
        InitWrapGenerator();
    } else if(hasOption('b')) {
        InitBuildScriptGenerator();
        if(hasOption('n'))
            generate_setNamespace(getName('n'));
    } else//use command generator
        InitCommandoGenerater();

    ExitOnError("initiation");

    strcpy( Tokken[DATE], "by Command Generator v.0.1" );

 // handle the inputs given:
    if(hasOption('a'))
        assignTokken(&Tokken[0],AUTHOR,getName('a'));

    if(hasOption('o'))
        sprintf(&outputDirectory[0],"%s/",getName('o'));

    if(hasOption('d')) {
        if(!isSwitch('d')) {
            strcpy(Tokken[DATE],getName('d'));
        } else {
            strcpy(Tokken[DATE],getCurrentDateTime());
        }
    }

    if(hasOption('v'))
        showOptions();

    return ( hasOption('e') || hasOption('r') )
         ? INSERT_SNIPPED
         : INSERT_NOTHING;
}

//takes either string "on"/"off" or int 1/0
const char* generate_setBase64Encoding(cmLn enabled)
{
    if(!beginPersistChange(LOCAL))
        return getError();

    uint enable = enabled!=NoString? *(uint*)&enabled[0] : 0;
    if( enable>1 ) {
        if(enabled[0]=='o') {
            if( enabled[1]=='n') {
                enable = 1;
            } else if( enabled[1]=='f' && enabled[2]=='f' ) {
                enable = 0;
            }
        } else setErrorText( "setBase64 takes 'on' or 'off' parameter!" );
    } enable = enable ? 2699560 : 2699304; // <- fourcc's "(0)" and "(1)"
    ExitOnError("argument");
    setPersistEntry( "USE_BASE64_ENCODING",(char*)&enable);
    commitPersistChange();
    return pool_setf( "Resource generator Base64 support: %s\n",
                      enabled );// ? "ENABLED":"DISABLED" );
}

const char* generate_setTabulatorString(cmLn tabString)
{
    if(strcmp(tabString,TABULATOR_SEQUENCE)>EMPTY) {
    if(beginPersistChange(LOCAL)) {
        setPersistEntry("TABULATOR_SEQUENCE",pool_setf("\"%s\"",tabString));
    commitPersistChange(); } else ExitOnError("environMentor");
    } return pool_setf("generate: Now using \"%s\" for Tabulator sequence",tabString);
}

const char* generate_setMultMode(cmLn enable)
{
	beginPersistChange(LOCAL);
	if( enable == 0 ) enable = "off";
	else if( enable == 1 ) enable = "on";
	if( (enable[0]=='o' && enable[1]=='n' ) || ( enable[0] == '1' || enable[1] == '1' ) ) {
		enable = "(1)";
    } else {
		enable = "(0)";
	} setPersistEntry( "USE_MULTIPROP_LISTS", enable );
	commitPersistChange();
	return pool_setf( "MULTIPROP_LISTS support: %s",
	            enable[1]=='1'?"Enabled":"Disabled");
}

generator
generate_Initialize( const char* options )
{
    if ( options )
        PushCommandLine( options );

    search('c');
    search('r');
    search('n');
    search('l');

    char mode = 'c';
    if( hasOption('r') )
         mode = 'r';
    else if( hasOption('n') )
         mode = 'n';
    else if( hasOption('l') )
         mode = 'l';

    prepareReplacements();

    generator currentActiveGenerator = { NULL };
    if( currentActiveGenerator.call = NULL ) {
        switch( mode ) {
            case 'C':
                currentActiveGenerator.func = &generate_newController;
            break;
            case 'c':
                currentActiveGenerator.call = &generate_newCommand;
            break;
            case 'r':
                currentActiveGenerator.call = &generate_newResource;
            break;
            case 'n':
                currentActiveGenerator.func = &generate_newClass;
            break;
            case 'l':
                currentActiveGenerator.call = &generate_newWrap;
            break;
        }
    } return currentActiveGenerator;
}

generator
generate_newWrap( const char* target )
{
    setOption('l',target);
    if( hasOption('c') && hasOption('o') ) {
        char Inserts = prepareReplacements();
        generateHeaderFile(getName('o'));
        generateCodeFile(getName('o'),Inserts);
        generateExtraFile(getName('o'));
    } generator nextcall = { &generate_newWrap };
    return nextcall;
}

generator
generate_newClass( const char* classname, const char* basename )
{
    setOption('c',classname);
    if(hasOption('n')&&hasOption('o')) {
        char Inserts = prepareReplacements();
        generateHeaderFile(getName('o'));
        generateCodeFile(getName('o'),Inserts);
        generateExtraFile(getName('o'));
    } generator nextfunc = { &generate_newClass };
    return nextfunc;
}

generator
generate_newCommand(const char* name)
{
    setOption('c',name);
    if(hasOption('o')) {
        char Inserts = prepareReplacements();
        generateHeaderFile(getName('o'));
        generateCodeFile(getName('o'),Inserts);
        generateExtraFile(getName('o'));
    } generator nextcall = { &generate_newCommand };
    return nextcall;
}

generator
generate_newResource(const char* filename)
{
    setOption('r',filename);
    if( hasOption('n') && hasOption('c') && hasOption('o') ) {
        char Inserts = prepareReplacements();
        generateHeaderFile(getName('o'));
        generateCodeFile(getName('o'),Inserts);
        generateExtraFile(getName('o'));
    } generator nextcall = { &generate_newResource };
    return nextcall;
}

generator
generate_newController( const char* usermode, const char* basemode )
{
    setOption('C',usermode);
    if ( isValidArg(basemode) )
        setOption('c',basemode);
    if ( hasOption('o') ) {
        char Inserts = prepareReplacements();
        generateHeaderFile(getName('o'));
        generateCodeFile(getName('o'),Inserts);
        generateExtraFile(getName('o'));
    } generator nextfunc = { &generate_newController };
    return nextfunc;
}

#ifdef generate
int main(int argc, char* argv[])
{
    InitCommandLiner(argc,argv);

    pool_InitializeCycle();

    if(!numGum()||hasOption('h'))
        exit(USAGE(generate));

#ifdef CURRENT_DATE
    if(!hasOption('d'))
        setSwitch('d');
    else if (isSwitch('d'))
        noOption('d');
#endif

    if( isModus("setBase64") ) {
        printf( generate_setBase64Encoding(rawNext('s')) );
        exit( getErrorCode() );
    }
    if( isModus("setTabChars") ) {
        printf( generate_setTabulatorString(rawNext('s')) );
        exit( getErrorCode() );
    }
	if( isModus("setMultiMode") ) {
		printf( generate_setMultMode(rawNext('s')) );
		exit( getErrorCode() );
	}
    SILENT_MODE = hasOption('s');

    getcwd( &outputDirectory[0], MAX_NAM_LEN );
    strcat( &outputDirectory[0], "/" );

 // handle raw input if 1 or 2 raw arguments given:
    int i = 1;
    if ( (!hasOption('c')) && (!hasOption('n')) ) {
        cmLn raw = rawName(2);
        if ( !isEmptyArg(raw) ) {
            switchIdx('c',indexOfName(raw));
            switchIdx('n',indexOfName(rawName(1)));
        } else if ( !isEmptyArg(raw = rawName(1)) ) {
            switchIdx('c',indexOfName(raw));
        }
    } else {
        if ( isSwitch('n') ) {
            cmLn nspc = rawNext('n');
            if ( !isEmptyArg(nspc) ) {
                switchIdx( 'n', indexOfName(nspc) );
            ++i; }
        }
        if ( isSwitch('c') ) {
            cmLn clss = rawNext('c');
            if ( !isEmptyArg(clss) ) {
                switchIdx( 'c', indexOfName(clss) );
            ++i; }
        }
    }

 // prepare replacements regarding the given input parameters:
    char InsertOptions = prepareReplacements();

 // generate the files:
    generateHeaderFile( &outputDirectory[0] );
    generateCodeFile( &outputDirectory[0], InsertOptions );
    generateExtraFile( &outputDirectory[0] );

    exit(getErrorCode());
}
#endif