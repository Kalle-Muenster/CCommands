// template strings and functions for generating binary recources or base64
// recources, which later can be compiled by using regular c/c++ compilers.

const byte ResourceHeader_partA[] = { "\
#ifndef _^COMMANDNAME^_Resource_Included_\n\
#define _^COMMANDNAME^_Resource_Included_\n\n\
^SNIPPED.WRAPPEDLINE^\
^Space.LOS^\n\n\
^Space.TAB^typedef unsigned char byte;\n\
^Space.TAB^typedef enum type {\n\
^Space.TAB^    BIN='b',B64='s'\n\
^Space.TAB^} type;\n\n\
" };

const byte ResourceDeclaration[] = { "\
^Space.TAB^^Space.LOS^\n\
^Space.TAB^^Space.TAB^unsigned    ^COMMANDNAME^^Space.SEP^getSize(void);\n\
^Space.TAB^^Space.TAB^const char* ^COMMANDNAME^^Space.SEP^getName(void);\n\
^Space.TAB^^Space.TAB^const byte* ^COMMANDNAME^^Space.SEP^getData(void);\n\
^Space.TAB^^Space.TAB^const type  ^COMMANDNAME^^Space.SEP^getType(void);\n\
^Space.TAB^^Space.END^\
" };

const byte ResourceHeader_partB[] = { "\
\n^Space.END^\n#endif\n\
" };

const byte ResourceCode_partA[] = { "\
#include \"^Space.NAM^_Resources.h\"\n\
^Space.USE^\n\
const char* ^COMMANDNAME^_name = \"^SNIPPED.DATANAME^\";\n\
const type  ^COMMANDNAME^_type = ^SNIPPED.DATATYPE^;\n\
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

const byte ResourceIteratorC[] = {"\n\
#include <string.h>\n\
#include \"ResourceIterator.h\"\n\
 >|Multi.LINE.0 |< \n\
\n\n\
const Resource ResourcenList[] = {\n\
 >|Multi.SPEC.0 |< \n\
};\n\n\
const Resource* resources_by_name(const char *nam)\n\
{\n\
    int it = -1;\n\
    while ( ++it < resources_how_many() )\n\
        if( !strcmp( ResourcenList[it].name(), nam ) )\n\
            return &ResourcenList[it];\n\
    return 0;\n\
}\n\n\
const Resource* resources_by_index(int idx)\n\
{\n\
    return idx < resources_how_many()\n\
         ? &ResourcenList[idx]\n\
         : 0;\n\
}\n\n\
uint resources_how_many(void)\n\
{\n\
    return (uint)^Multi.ZAHL.SPEC^; }\n\
" };

const byte ResourceIteratorH[] = {"\
#ifndef _ResourceIterator_Included_\n\
#define _ResourceIterator_Included_\n\
#ifdef __cplusplus\n\
extern \"C\" {\n\
#endif\n\
//---------------Resource Iterator type definitions------------------//\n\n\
    typedef unsigned char byte;\n\
    typedef unsigned int  uint;\n\n\
    typedef enum TYPE {\n\
        BIN = 'b',\n\
        B64 = 's'\n\
    } TYPE;\n\n\
    typedef unsigned(*ResourceIterator_Size)(void);\n\
    typedef const char*(*ResourceIterator_Name)(void);\n\
    typedef const byte*(*ResourceIterator_Data)(void);\n\
    typedef const TYPE(*ResourceIterator_Type)(void);\n\n\
    typedef struct Resource {\n\
        ResourceIterator_Name name;\n\
        ResourceIterator_Type type;\n\
        ResourceIterator_Size size;\n\
        ResourceIterator_Data data;\n\
    } Resource;\n\n\
#define RESOURCE_LIST_ENTRY(ResNam) {\\\n\
    &##ResNam##_getName,\\\n\
    &##ResNam##_getType,\\\n\
    &##ResNam##_getSize,\\\n\
    &##ResNam##_getData\\\n\
}\n\
#define RESOURCE_DATA_ACCES(ResNam) \\\n\
    unsigned    ResNam##_getSize(void);\\\n\
    const char *ResNam##_getName(void);\\\n\
    const byte *ResNam##_getData(void);\\\n\
    const TYPE  ResNam##_getType(void)\n\n\
//--------------Resource Iterator function declarations----------------//\n\n\
    const Resource* resources_by_index(int idx);\n\
    const Resource* resources_by_name(const char *nam);\n\
          unsigned  resources_how_many(void);\n\n\
#ifdef __cplusplus\n\
}\n\
#endif\n\
#endif\n\
" };

// define generater functions:
void createResourceHeader( char* outputDir )
{
    char fNameBuf[MAX_NAM_LEN]={'\0'};
    strcpy(Tokken[EXTENSION],".h");
    sprintf( &fNameBuf[0],"%s%s_Resources%s", outputDir,
             Space.Tokken[SEP][0] == '_' ? Tokken[COMMANDNAME] : Space.Tokken[NAM], Tokken[EXTENSION] );

    FILE* headerFile = fopen( &fNameBuf[0], "r+" );
    fseek(headerFile,0,SEEK_END);
    if( ftell(headerFile) < 100 ) {
        fclose(headerFile);

        generateFile( (const byte*)&ResourceHeader_partA[0], &fNameBuf[0] );
        headerFile = fopen( &fNameBuf[0], "a" );
    } else {
        fclose(headerFile);

		headerFile = fopen( &fNameBuf[0], "r" );
		fseek(headerFile,0,SEEK_END);
		int si = ftell(headerFile);
		fseek(headerFile,0,SEEK_SET);
		char theFile[si+1];
		fread(&theFile[0],1,si,headerFile);
		while(theFile[--si]!='}');
		theFile[si]='\0';
		fflush(headerFile); fclose(headerFile);
		headerFile = fopen( &fNameBuf[0], "w" );
		fprintf( headerFile, &theFile[0] );
    }

    char isCpp;
    if( (isCpp = ( Space.Tokken[SEP][0] != '_' )) ) {
        assignTokken(&Space.Tokken[0],SEP,NoString);
        assignTokken(&Tokken[0],COMMANDNAME,NoString);
        assignTokken(&Space.Tokken[0],NAM,getName('c'));
    }

    char headerEntry[ strlen( &ResourceDeclaration[0] ) + MAX_NAM_LEN ];
	int endpos = parseText( &ResourceDeclaration[0], &headerEntry[0] );
    headerEntry[ endpos ] = '\0';
    fprintf( headerFile, &headerEntry[0] );

    if( isCpp ) {
        assignTokken( &Space.Tokken[0], SEP, "::" );
        assignTokken( &Tokken[0], COMMANDNAME, getName('c') );
        assignTokken( &Space.Tokken[0], NAM, getName('n') );
    }
	endpos = parseText( &ResourceHeader_partB[0], &headerEntry[0] );
    headerEntry[ endpos ] = '\0';
    fprintf( headerFile, &headerEntry[0] );
    fflush( headerFile );
    fclose( headerFile );
}

void createResourceData( char* outputDir, char insertSnippet )
{
    unsigned fileSize=0;
    unsigned extraChars=0;
    byte* resourceBuffer = 0;
    char fileNameBuffer[MAX_NAM_LEN]={'\0'};

    FILE* dataFile;
	
	int IsB64 = 0;
#if USE_BASE64_ENCODING
	IsB64 = isModus("B64");

    if( IsB64 ) {
        dataFile = fopen(getName('r'),"rb");
        fseek( dataFile, 0, SEEK_END );
        fileSize = ftell(dataFile);
		fseek( dataFile, 0, SEEK_SET );
        printf("BINARY-DATASIZE: %i\n",fileSize);
        fileSize *= ( (1.0/64.0) + BASE64_COMPRESSIONRATE );
        printf("BASE64-DATASIZE: %i\n",fileSize);
        resourceBuffer = (byte*)malloc( fileSize+5 );
        fileSize = 0;
        
        resourceBuffer[extraChars++]='\"';
        resourceBuffer[extraChars++]='\\';
        resourceBuffer[extraChars++]='\n';
        //byte readBuffer[4]={0,0,0,0};
		b64Frame readBuffer;
		readBuffer.u32 = 0;
		int readSize = 0;
		printf("BEGINNING READ\n");
        while( (readSize = fread( &readBuffer.i8[0], 1, 3, dataFile )) > 0 ) {
            ((b64Frame*)&resourceBuffer[ fileSize + extraChars ])->u32
                   = base64_encodeFrame( readBuffer ).u32; // asFrame(&readBuffer[0]) );
            fileSize += 4;
            if( (fileSize%64) == 0 ) {
                resourceBuffer[fileSize+extraChars]='\\';
                resourceBuffer[fileSize+extraChars+1]='\n';
                extraChars+=2;
            }
        }
		printf( "FILE READ\n" );
		printf( "resourceBuffer size: %i\n", (fileSize+extraChars)+3 );
		resourceBuffer[fileSize+extraChars++]='=';
        resourceBuffer[fileSize+extraChars++]='\"';
		resourceBuffer[fileSize+extraChars] = '\0';
    } else
#endif
    {
        dataFile = fopen(getName('r'),"rb");
        fseek(dataFile,0,SEEK_END);
        fileSize = ftell(dataFile);
        fseek(dataFile,0,SEEK_SET);
        resourceBuffer = (byte*)malloc((fileSize*5)+((float)fileSize*(1.0/32.0)+0.5));
        byte readBuffer[2]={'\0','\0'};
        int counter = fileSize-1;
        fileSize=0;
        printf("BINARY-DATASIZE: %i\n",counter);
        resourceBuffer[0]='\n';
        fread(&readBuffer[0],1,1,dataFile);
        sprintf(&resourceBuffer[1],"0x%02X",readBuffer[0]);
        fileSize++; extraChars+=4;
        while(counter--) {
            fread(&readBuffer[0],1,1,dataFile);
            int lb = fileSize%32 ? 0 : 1;
            unsigned position = fileSize + extraChars;
            sprintf(&resourceBuffer[position],lb++?",\n":",");
            sprintf(&resourceBuffer[position+lb],"0x%02X",readBuffer[0]);
            extraChars += (lb+3); fileSize++;
        } resourceBuffer[fileSize+extraChars] = '\0';
    }
	
	printf("preparing for FILE WRITE\n");

    char filesizebuffer[32];
    sprintf( &filesizebuffer[0], "%i", fileSize );
    Snipped.Tokken[DATASIZE] = (byte*)&filesizebuffer[0];

    strcpy( Tokken[EXTENSION], ".c" );
	strcpy( &fileNameBuffer[0], outputDir );
	if ( Space.Tokken[SEP][0] != '_' ) {
		strcat( &fileNameBuffer[0], getName('n') );
		strcat( "_", getName('n') );
	}
	strcat( &fileNameBuffer[0], getName('c') );
	strcat( &fileNameBuffer[0], "_Resource" );
	strcat( &fileNameBuffer[0], Tokken[EXTENSION] );

	printf( "preparing for FILE WRITE: %s\n", &fileNameBuffer[0] );
	printf( "Writing Source File: %s\n", &fileNameBuffer[0] );
	
    generateFileConditionalInsert( &ResourceCode_partA[0],
                                   &ResourceCode_partB[0],
                                   &resourceBuffer[0],
                                   SNIPPED, &fileNameBuffer[0] );
    free( resourceBuffer );
}

void createResourceManager( char* outputDir )
{
	char fileNameBuffer[MAX_NAM_LEN*2];
	strcpy( &fileNameBuffer[0], outputDir );
	strcat( &fileNameBuffer[0], "ResourceIterator.c" );
	FILE* checke = fopen( &fileNameBuffer[0], "r+" );
	if ( !checke ) {
		fileNameBuffer[strlen(&fileNameBuffer[0])-1] = 'h';
		printf( "Writing Iterator File: %s\n", &fileNameBuffer[0] );
		generateFile( (const byte*)&ResourceIteratorH[0], &fileNameBuffer[0] );
		printf( "Done With Iterator.h File!\n" );
		fileNameBuffer[strlen(&fileNameBuffer[0])-1] = 'c';
		printf( "Writing Iterator File: %s\n", &fileNameBuffer[0] );
		generateFile( (const byte*)&ResourceIteratorC[0], &fileNameBuffer[0] );
		// strcpy( &fileNameBuffer[0], "notepad " );
		// strcat( &fileNameBuffer[0], outputDir );
		// strcat( &fileNameBuffer[0], "ResourceIterator.c" ); 
		// system( &fileNameBuffer[0] );
	} else {
		fseek(checke,0,SEEK_END);
		int size = ftell(checke);
		fseek(checke,0,SEEK_SET);
		printf( "Loading Iterator File: %s\n", &fileNameBuffer[0] );
		byte readebuffer[size+1];
		fread( &readebuffer[0], 1, size, checke );
		char* ab = (char*)&readebuffer[size-1];
		while(*--ab!='\n');
		*ab = '\0';
		fclose( checke );
		byte parsebuffer[size+(3*MAX_NAM_LEN)];
		int end = parseText( &readebuffer[0], &parsebuffer[0] );
		parsebuffer[end] = '\0';
		printf( "ReWriting Iterator File: %s\n", &fileNameBuffer[0] );
		checke = fopen( &fileNameBuffer[0], "w" );
		fwrite( &parsebuffer[0], 1, strlen( &parsebuffer[0] ), checke );
		end = parseText( "\n    return (uint)^Multi.ZAHL.SPEC^; }\n", &parsebuffer[0] );
		parsebuffer[end] = '\0';
		fwrite( &parsebuffer[0], 1, strlen( &parsebuffer[0] ), checke );
		fflush( checke );
		fclose( checke );
	}
}

void InitResourceGenerater(void)
{
    Assert('r',"no file was given to generate resource from!");
    Assert('c',"no resource name was given!");

    ExitOnError("parameter");

    char* resnam=(char*)getName('c');
    resnam = (resnam[0]>='0' && resnam[0]<='9')
           ? pool_setf("_%s",&resnam[0])
           : pool_set(&resnam[0]);
    do{ if((*resnam=='-')||(*resnam=='.'))
           *resnam='_';
    } while(*++resnam);
    setOption('c',pool_get());

    pool_setCheckpoint();
#if USE_BASE64_ENCODING
    if(isModus("B64")){
        pool_setf("#define BASE64_CODING_TABLE \"%s\"\n#include <.base64.h>\n",base64_getTable());
    }else
#endif
        setOption('B',"BIN");

    pool_set(" \n");
    assignTokken(&Snipped.Tokken[0],WRAPPEDLINE,pool_collectCheckpoint());
    assignTokken(&Snipped.Tokken[0],DATANAME,getName('r'));
    assignTokken(&Snipped.Tokken[0],DATATYPE,getName('B'));
    assignTokken(&Tokken[0],COMMANDNAME,getName('c'));
	
	strcpy( Multi.Tokken[MULTI(&Multi.LINE)], "\n#include \"^Space.NAM^_Resources.h\" //<|Multi.LINE.^Multi.ZAHL.LINE^|>\n" );
	strcpy( Multi.Tokken[MULTI(&Multi.SPEC)],  "\n    RESOURCE_LIST_ENTRY(^Space.NAM^), //<|Multi.SPEC.^Multi.ZAHL.SPEC^|>\n" );
	
    if(!hasOption('n')) {
        printf("Will generate 'C' code!\n");
        generate_setNamespace(NoString);
        assignTokken(Space.Tokken,SEP,"_");
        assignTokken(Space.Tokken,NAM,getName('c'));
    } else {
        printf("Will generate 'C++' code!\n");
        generate_setNamespace( (const byte*)getName('n') );
    }
    generateHeaderFile = &createResourceHeader;
    generateCodeFile   = &createResourceData;
    generateExtraFile  = &createResourceManager;
}
