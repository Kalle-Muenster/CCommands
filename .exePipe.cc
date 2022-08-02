/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      exePipe.cc                                 ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include ".exePipe.h"
#endif



#include ".stringPool.h"
#include ".commandLiner.h"


int USAGE(exePipe)
{
    Synopsis("[pre-pipe-token] [post-pipe tokken] [options]");
    printf("Options:\n\n\
    -0*|c [pretoken] :   define any [token] to paste before parsing the incomming pipe input at least.\n\
                         --0-[token] creates back pasting parsed stdin via system() call... \n\
                         --c-[token] does same but back pastes the pipe by resolving c-expressions\n\
                         (cLine style '=' escaped) when contained within the streamed pipe input.\n\
    --t-terminator   :   define a distinct character to be used for escaping from the pipe.\n\
                         when ommitted, command bx default returns on first '\\n' it encounters.\n\
    --+-postoken     :   define any [token] which should be pasted after any parsed pipe input...\n\
    --help | -h      :   output (this) usage information site.\n\
    --echo | -e      :   print out whole content before passing back to the command line.\n\n");
    return CheckForError()
        || !hasOption('h');
}

ulong prepareTerminator( char mode, ulong* out_mask, int* out_length )
{
    ulong termi, mask = 0x00000000000000ff;
    char* input = getName( mode );
    if ( mode == 'T' ) {
        termi = atoll( input );
        if ( !termi ) {
            *out_length = 1;
        } else {
            int bit = 64;
            while( !( termi >> --bit ) );
            *out_length = (bit+1)/8;
            while( bit ) mask = mask|(1<<bit--);
        }
    } else {
        *out_length = strlen( input );
        if ( *out_length > 8 ) { setErrorText("terminator size"); return 0; }
        for( int i = 0; i < *out_length; ++i )
            mask = mask|(0xff<<(i*8));
        termi = mask & *(ulong*)input;
    }
    *out_mask = mask;
    return termi;
}


int main(int argc,char**argv)
{
    InitCommandLiner(argc,argv);
    char echo = hasOption('e');
    ulong maski = 0x00000000000000ff;
    int   lengi = 1;
    ulong termi = '\n';

    if( hasOption('h') )
        exit(USAGE(exePipe));

    if((!hasOption('0') && (!hasOption('c'))))
        setOption( '0', rawName(1) );

    if( echo )
        showOptions();

    if( search('+') ) {
        char* postPipe = pool_setf(" %s\n",getName('+'));
        setOption('+',postPipe);
    }
    if( search('t') ) {
        termi = prepareTerminator( 't', &maski, &lengi );
    } else
    if( search('T') ) {
        termi = prepareTerminator( 'T', &maski, &lengi );
    }

	ExitOnError("Parameter");

    char  mode;
    if( search('0') ) mode = '0'; 
	else if( search('c') ) mode = 'c';
	
	char* input;
    uint* sizei = pool_setCheckpoint();
    input = getName(mode);
    if(!isEmptyArg(input)) {
        while( *input == 0x08 ) ++input;
        pool_setf(mode=='0'?"\"%s":"cLine.c \"%s\"",input);
    } else
        pool_set(mode=='0'?"\"":"cLine.c");

    while( *sizei < CYCLE_SIZE ) {
        input = getName(mode);
        scanf("%s\n",input);
        while( *input == 0x08 ) ++input;
        if( (*(ulong*)&input[0] & maski) == termi )
            break;
        pool_setf(mode=='0'?" %s":" \"%s\"",input);
    }
    if( hasOption('+') )
        pool_setf((hasOption('c') ?
                   " \"%s\"\n":" %s\"\n"),
                   getName('+'));
    else
        pool_set((hasOption('c')?"\n":"\"\n"));

    // fetch input from pool
    char* backpaste = pool_collectCheckpoint();
    if(echo) printf("%s\n",backpaste);

    // and execute it
    exit(system(backpaste));
}
