// template strings and functions for generating a binlink

const byte WrapperCall[] = { "\
#define _OnTheFly_ ^COMMANDNAME^\n\
#define ^COMMANDNAME^ \"\"\n\
#include \".^COMMANDNAME^.h\"\n\
int main(int argc,char** argv);\n\
" };
const byte WrapperHead[] = { "\
#ifndef IncludesFor_^COMMANDNAME^\n\
#define IncludesFor_^COMMANDNAME^ (1)\n\
\n\n^SNIPPED.DATATYPE^\n\n\n\
#ifdef _OnTheFly_\n\
#include \".^COMMANDNAME^.cc\"\n\
#endif\n\
#endif\n\
" };
const byte WrapperCode[] = { "\
#ifndef _OnTheFly_\n\
#include \".^COMMANDNAME^.h\"\n\
#endif\n\n\
int main(int argc,char**argv)\n{\
" };
const byte SynchronWrapper[] = { "\
\n    int cmdLength = ^SNIPPED.DATASIZE^ + argc;\
\n    for(int i=1;i<argc;i++)\
\n        cmdLength += strlen(argv[i]);\
^SNIPPED.EXAMPLECODE^\
\n    for(int i=1;i<argc;i++)\
\n        strcat(strcat(command,\" \"),argv[i]);\
\n    int returnVal = system(command);\
\n    free(command);\
"};
const byte AsynchronWrapper[] = { "\
\n    int cmdLength = ^SNIPPED.DATASIZE^ + argc;\
^SNIPPED.EXAMPLECODE^\
\n    execvp( command, argv );\
\n    printf( \"bad command line: '%s'\\n\", command );\
\n    int returnVal = -1;\
"};
const byte WrapperClose[] = { "\
\n    return returnVal;\n}\n\
"};

void createWrapperHead( char* outputDir )
{
    char buffer[MAX_NAM_LEN]={'\0'};
    strcpy(Tokken[EXTENSION],".h");
    sprintf( &buffer[0],"%s.%s%s", outputDir,
             Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile( &WrapperHead[0], &buffer[0] );
}

void createWrapperCode( char* outputDir, char insertSnippet )
{
    char buffer[MAX_NAM_LEN]={'\0'};
    strcpy( Tokken[EXTENSION], ".cc" );
    sprintf( &buffer[0],"%s.%s%s", outputDir,
             Tokken[COMMANDNAME], Tokken[EXTENSION] );
	const byte* selected = hasOption('A')
	                     ? &AsynchronWrapper[0]
                         : &SynchronWrapper[0];
    generateFileConditionalInsert( &WrapperCode[0], &WrapperClose[0],
                                   selected, SNIPPED, &buffer[0] );
}

void createWrapperCall( char* outputDir )
{
    char buffer[MAX_NAM_LEN]={'\0'};
    strcpy( Tokken[EXTENSION], ".c" );
    sprintf( &buffer[0],"%s%s%s", outputDir,
             Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFileConditionalInsert( "#!/usr/bin/tcc -run -w\n",
                                   WrapperCall, InfoHeader,
                                   INSERT_NOINFO, &buffer[0] );
}

void InitWrapGenerator(void)
{
    if(ensure('c',"no name for wrapper was given!")) {
        assignTokken(&Tokken[0],COMMANDNAME,getName('c'));
        if(search('l')) {
            assignTokken( Snipped.Tokken, DATANAME,
                "\n    char* command = (char*)malloc(cmdLength); *command = '\\0';"
            );
            const char* checkWrappedLine = getName('l');
            assignTokken( Snipped.Tokken, EXAMPLECODE,
                "^SNIPPED.DATANAME^\n    strcat(command,\"^SNIPPED.WRAPPEDLINE^\");"
            );
            int Async = isModus("Async");
			// spawning processes via exec / spawn / process functions, 
			// on windows, does not (other then a system() call would do)
            // resolve any in file path's maybe contained '/' characters.
			// from proces.h doesn't handles other then   
			char* slash = (char*)getName('l');
			if ( WINDOWS ) {
				int slashes = 0;
				char* back = slash;
				do { if(*back == '/') {
						*back = '\0';
						pool_setf( "%s\\\\", slash ); 
					    slash = back + 1; ++slashes;
				} } while ( *++back );
				if ( slashes > 0 ) {
					pool_set( slash );
					slash = pool_merge( slashes+1 );
				} else slash = getName('l');
			} assignTokken( Snipped.Tokken, WRAPPEDLINE, slash );
            pool_set( "#include <stdlib.h>" );
            sprintf( setOption('S',""), "%i", strlen( slash ) );
            assignTokken( Snipped.Tokken, DATASIZE, getName('S') );
            if ( Async ) {
                pool_set( WINDOWS ? "\n#include <process.h>"
                                  : "\n#include <unistd.h>" );
            } assignTokken( Snipped.Tokken, DATATYPE,
                            pool_merge( Async+1 ) );
        } else {
            setErrorText("Wrap-Generator needs l-argument");
        }
    }
    generateHeaderFile = &createWrapperHead;
    generateCodeFile   = &createWrapperCode;
    generateExtraFile  = &createWrapperCall;
}