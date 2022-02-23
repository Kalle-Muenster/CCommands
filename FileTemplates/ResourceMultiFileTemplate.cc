// template strings and functions for generating binary recources or base64
// recources, which later then can be compiled by regular c or c++ compilers.

const byte ResourceHeader[] = { "\
#ifndef _^COMMANDNAME^_Resource_Included_\n\
#define _^COMMANDNAME^_Resource_Included_\n\
^SNIPPED.WRAPPEDLINE^RESOURCE_DATA_ACCES(^COMMANDNAME^);\n\n\
#ifdef __cplusplus\n}\n#endif\n\
#endif\n\
" };

const byte ResourceCode_partA[] = { "\
#include \"ResourceIterator.h\"\n\
#include \"^Space.NAM^_Resource^COMMANDNAME^^EXTENSION^\"\n\
^Space.USE^\n\
const char* ^COMMANDNAME^_name = \"^SNIPPED.DATANAME^\";\n\
const TYPE  ^COMMANDNAME^_type = ^SNIPPED.DATATYPE^;\n\
const byte  ^COMMANDNAME^_data[] = {\
" };

const byte ResourceCode_partB[] = { "\
\n};\n\n\
const unsigned ^COMMANDNAME^_size = ^SNIPPED.DATASIZE^;\n\n\
const char*\n\
^COMMANDNAME^^Space.SEP^getName(void)\n\
{\n    return ^COMMANDNAME^_name;\n\
}\n\nunsigned\n\
^COMMANDNAME^^Space.SEP^getSize(void)\n\
{\n    return ^COMMANDNAME^_size;\n\
}\n\nconst type\n\
^COMMANDNAME^^Space.SEP^getType(void)\n\
{\n    return ^COMMANDNAME^_type;\n\
}\n\nconst byte*\n\
^COMMANDNAME^^Space.SEP^getData(void)\n\
{\n    return &^COMMANDNAME^_data[0];\n\
}\n\
" };

const byte ResourceIteratorH[] = { "\
#ifndef _^COMMANDNAME^_Resource_Included_\n\
#define _^COMMANDNAME^_Resource_Included_\n\n\
^SNIPPED.WRAPPEDLINE^\n\
//*************Resource includig intrfaface******************//\n\n\
    typedef unsigned char byte;\n\
    typedef unsigned int  uint;\n\n\
    ^Space.LOS^\n\
    ^Space.TAB^typedef enum TYPE {\n\
    ^Space.TAB^    BIN = 'b',\n\
    ^Space.TAB^    B64 = 's'\n\
    ^Space.TAB^} TYPE;\n\n\
    ^Space.TAB^typedef unsigned(*^COMMANDNAME^Resource_Size)(void);\n\
    ^Space.TAB^typedef const char*(*^COMMANDNAME^Resource_Name)(void);\n\
    ^Space.TAB^typedef const byte*(*^COMMANDNAME^Resource_Data)(void);\n\
    ^Space.TAB^typedef const TYPE(*^COMMANDNAME^Resource_Type)(void);\n\n\
    ^Space.TAB^typedef struct Resource {\n\
    ^Space.TAB^    ^COMMANDNAME^Resource_Name name;\n\
    ^Space.TAB^    ^COMMANDNAME^Resource_Type type;\n\
    ^Space.TAB^    ^COMMANDNAME^Resource_Size size;\n\
    ^Space.TAB^    ^COMMANDNAME^Resource_Data data;\n\
    ^Space.TAB^} Resource;^Space.END^\n\
    #define RESOURCE_LIST_ENTRY(ResNam) {\\\n\
        &##ResNam##_getName,\\\n\
        &##ResNam##_getType,\\\n\
        &##ResNam##_getSize,\\\n\
        &##ResNam##_getData,\\\n\
    }\n\n\
    #define RESOURCE_DATA_ACCES(ResNam) \\\n\
        const char *ResNam##_getName(void); \\\n\
        unsigned    ResNam##_getSize(void); \\\n\
        const TYPE  ResNam##_getType(void); \\\n\
        const byte *ResNam##_getData(void)\n\n\
//*********Resource accessor function declarations***********//\n\n\
    const Resource* ^Space.NAM^^Space.SEP^resourceByIndex(int idx);\n\
    const Resource* ^Space.NAM^^Space.SEP^resourceByName(const char *nam);\n\
          unsigned  ^Space.NAM^^Space.SEP^resourcesHowMany(void);\n\n\
^SNIPPED.DATATYPE^#endif\n\
" };

const byte ResourceIteratorC[] = { "\
#include <string.h>\n\
#include \"ResourceIterator.h\"\n\
// include headers of resources in set: ^COMMANDNAME^\n\
>|Multi.LINE|<\n\
\n\
^Space.USE^\n\
const Resource ResourcenList[] = {\n\
    >|Multi.LIST|<\n\
};\n\n\
#define COUNT_ON_RESOURCES (^Multi.ZAHL.LINE^)\n\n\
const Resource* ^Space.NAM^^Space.SEP^resourceByName(const char *nam)\n\
{\n    int it = -1;\n\
    while (++it<COUNT_ON_RESOURCES)\n\
        if( !stringCompare( ResourcenList[it].name(), nam ) )\n\
            return &ResourcenList[it];\n\
    ^SNIPPED.WRAPPEDLINE^\
}\n\n\
const Resource* ^Space.NAM^^Space.SEP^resourceByIndex(int idx)\n\
{\n    if( idx<COUNT_ON_RESOURCES )\n\
        return &ResourcenList[idx];\n\
    ^SNIPPED.WRAPPEDLINE^\
}\n\n\
uint ^Space.NAM^^Space.SEP^resourcesHowMany(void)\n\
{\n    return (uint)COUNT_ON_RESOURCES;\n    }\
" };

// define generater functions:
void createResourceHeader(char* outputDir)
{
    char isCpp = Space.Tokken[SEP][0] != '_';
    char fNameBuf[MAX_NAM_LEN]={'\0'};
	sprintf( &fNameBuf[0], "%s%s_Resource%s%s", outputDir, getName('c'),
             isCpp ? pool_setf( "_%s", getName('n') ) : "", isCpp?".hpp":".h" );
    generateFile( &ResourceHeader[0], &fNameBuf[0] );
}

void createResourceData( char* outputDir,char insertSnippet )
{
    DEBUGLOG("entering createResourceData()")
    unsigned fileSize=0;
    unsigned extraChars=0;
	char isCpp = Space.Tokken[SEP][0] != '_';
    byte* resourceBuffer = 0;
    char fileNameBuffer[MAX_NAM_LEN]={'\0'};

    FILE* dataFile;
#if USE_BASE64_ENCODING
    if( isModus("B64") ) {
        dataFile = fopen(getName('r'),"rb");
        fseek(dataFile,0,SEEK_END);
        fileSize = ftell(dataFile);
        printf("BINARY-DATASIZE: %i\n",fileSize);
        fileSize *= ( (1.0/64.0) + BASE64_COMPRESSIONRATE );
        printf("BASE64-DATASIZE: %i\n",fileSize);
        resourceBuffer = (byte*)malloc( fileSize+2 );
        fileSize = 0;
        fseek(dataFile,0,SEEK_SET);
        resourceBuffer[extraChars++]='\"';
        resourceBuffer[extraChars++]='\\';
        resourceBuffer[extraChars++]='\n';
        byte readBuffer[4]={0,0,0,0};
        while(fread(&readBuffer[0],3,1,dataFile)) {
            asFrame(&resourceBuffer[fileSize+extraChars])
                   = base64_encodeFrame(asFrame(&readBuffer[0]));
            fileSize += 4;
            if( (fileSize%64) == 0 ) {
                resourceBuffer[fileSize+extraChars]='\\';
                resourceBuffer[fileSize+extraChars+1]='\n';
                extraChars+=2;
            }
        }  resourceBuffer[fileSize+extraChars++]='=';
        resourceBuffer[fileSize+extraChars++]='\"';
        resourceBuffer[fileSize+extraChars] = '\0';
        fileSize += ((extraChars-2)/2);
        fflush(dataFile); fclose(dataFile);
    } else
#endif
    { //if(isModus("BIN"))
        DEBUGLOG("opening file")
        dataFile = fopen( getName('r'), "rb" );
        fseek(dataFile,0,SEEK_END);
        fileSize = ftell(dataFile);
        fseek(dataFile,0,SEEK_SET);
        ptval buf_siz = (ptval)((fileSize*5)
                      + ((float)fileSize*(1.0/32.0)+0.5));
        resourceBuffer = (byte*)malloc(buf_siz);
        byte readBuffer[2] = {'\0','\0'};
        int counter = fileSize-1;
        fileSize = 0;
        printf( "BINARY-DATASIZE: %i\n", counter );
        resourceBuffer[0] = '\n';
        fread( &readBuffer[0], 1, 1, dataFile );
        sprintf( &resourceBuffer[1], "0x%02X", readBuffer[0] );
        fileSize++; extraChars += 4;
        while( counter-- ) {
            fread( &readBuffer[0], 1, 1, dataFile );
            int lb = fileSize%32 ? 0 : 1;
            unsigned position = fileSize + extraChars;
            sprintf( &resourceBuffer[position], lb++ ? ",\n" : "," );
            sprintf( &resourceBuffer[position+lb], "0x%02X", readBuffer[0] );
            extraChars += (lb+3); fileSize++;
        } fflush(dataFile); fclose(dataFile);
        resourceBuffer[fileSize+extraChars] = '\0';
    }

    DEBUGFMT( "file %s read!", getName('r') )
    char filesizebuffer[32];
    sprintf( &filesizebuffer[0], "%i", fileSize );
    assignTokken(Snipped.Tokken,DATASIZE,(byte*)&filesizebuffer[0]);

    
    sprintf( &fileNameBuffer[0], "%s%s_Resource%s%s", outputDir, getName('c'),
             isCpp ? pool_setf( "_%s", getName('n') ) : "",
             isCpp ? ".cpp" : ".c" );

    generateFileConditionalInsert( &ResourceCode_partA[0],
                                   &ResourceCode_partB[0],
                                   &resourceBuffer[0],
                                   SNIPPED, &fileNameBuffer[0] );
    free(resourceBuffer);
}

void createResorceIterator( char* outputDir )
{
	
    char fileNameBuffer[MAX_NAM_LEN] = {'\0'};
    int isC = ( Space.Tokken[SEP][0] == '_' );
	sprintf( &fileNameBuffer[0],"%sResourceIterator%s",
	         outputDir, isC ? ".h" : ".hpp" );
    assignTokken( Snipped.Tokken, WRAPPEDLINE, isC ? 
	"#ifdef __cplusplus\nextern \"C\" {\n#endif\n" : "namespace RESOURCES {\n" );
    assignTokken( Snipped.Tokken, DATATYPE, "#ifdef __cplusplus\n}\n#endif\n" );
    FILE* checke = fopen( &fileNameBuffer[0], "r" );
	if( ferror(checke) ) { fclose(checke);
		DEBUGLOG("error: file gibs nich")
		generateFile( (const byte*)&ResourceIteratorH[0], &fileNameBuffer[0] );
	} else {
		DEBUGLOG("ok: file gibs schon")
		fclose(checke);
	}
	sprintf( &fileNameBuffer[0],"%sResourceIterator%s", outputDir, isC?".c":".cpp" );
    assignTokken( Snipped.Tokken, WRAPPEDLINE, isC ? "return 0;\n" : "throw -1;\n" );
	FILE* loade = fopen(&fileNameBuffer[0], "r" );
	fseek(loade,0,SEEK_END);
	uint chunk = ftell(loade);
	if( chunk > PARSER_BUFFER_SIZE ) {
		//dann muss man sich erstmal was überlegen...
	} else { // ansonsten parsen wir einfach gleich das ganze file!
	fseek( loade, 0, SEEK_SET );
	ulong  reade[PARSER_BUFFER_SIZE/16];
	fread( reade, 8, chunk>>3, loade );
	ulong parse[PARSER_BUFFER_SIZE/8];
	fclose( loade );
	char* geparst = (byte*)&reade[0] + parseText( (const byte*)&reade[0], (char*)&parse[0] );
    *(ulong*)geparst = 0; geparst = &parse[0];
	FILE* savefe = fopen( &fileNameBuffer[0], "w" );
	fwrite( geparst, 8,(strlen(geparst)+8)>>3, savefe );
	fflush( savefe ); fclose( savefe );
	}
	//generateFile( (const byte*)&ResourceIteratorC[0], &fileNameBuffer[0] );
}

void InitResourceGenerater(void)
{
    Assert('r',"no file was given to generate resource from!");
    Assert('c',"no resource name was given!");

    ExitOnError("parameter");

    pool_InitializeCycle();
    char* resnam = (char*)getName('c');
    resnam = (resnam[0]>='0' && resnam[0]<='9')
           ? pool_setf( "_%s", &resnam[0] )
           : pool_set( &resnam[0] );
    do{ if((*resnam=='-')||(*resnam=='.'))
           *resnam='_';
    } while(*++resnam);
    setOption( 'c', pool_get() );

    pool_setCheckpoint();
#if USE_BASE64_ENCODING
    if( isModus("B64") ) {
        pool_setf( "#define BASE64_CODING_TABLE \"%s\"\n#include <.base64.h>\n",
                   base64_getTable() );
    } else
#endif
        setOption('B',"BIN");

    assignTokken( Snipped.Tokken, DATANAME, getName('r') );
    assignTokken( Snipped.Tokken, DATATYPE, getName('B') );
    assignTokken( Tokken, COMMANDNAME, getName('c') );


    int dbgvar = MULTI(&Multi.LIST);
    DEBUGFMT("MULTI(Multi.LIST) = %i",dbgvar)
    assignTokken( Multi.Tokken, dbgvar,
        "^Space.TAB^RESOURCE_LIST_ENTRY(^COMMANDNAME^),\n//||Multi.LIST ^Multi.ZAHL.LIST^\n" );


    dbgvar = MULTI(&Multi.LINE);
    DEBUGFMT("MULTI(Multi.LINE) = %i",dbgvar)

    if(!hasOption('n')) {
		assignTokken( Tokken, EXTENSION, ".h" );
        printf( "Will generate 'C' code!\n" );
        generate_setNamespace( NoString );
        assignTokken( Space.Tokken, SEP, "_" );
        assignTokken( Space.Tokken, NAM, getName('c') );
        assignTokken( Multi.Tokken, dbgvar,
            "#include \"^COMMANDNAME^_Resource.h\" //||Multi.LINE ^Multi.ZAHL.LINE^\n" );
    } else {
		assignTokken( Tokken, EXTENSION, ".hpp" );
        printf( "Will generate 'C++' code!\n" );
        pool_set( "\n\nnamespace RESOURCES {\n\n" );
        generate_setNamespace( (const byte*)getName('n') );
        assignTokken( Multi.Tokken, dbgvar,
            "#include \"^COMMANDNAME^_Resource_^Space.NAM^.hpp\" //||Multi.LINE ^Multi.ZAHL.LINE^\n" );
    } assignTokken( Snipped.Tokken, WRAPPEDLINE,
                    pool_collectCheckpoint() );

    generateHeaderFile = &createResourceHeader;
    generateCodeFile   = &createResourceData;
	if ( hasOption('n') )
		assignTokken( Tokken, COMMANDNAME, "" );
    generateExtraFile  = &createResorceIterator;
}
