/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      hallo.cc                                   ||
||     Author:    autogenerated                              ||
||     Generated: by Command Generator v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".hallo.h"
#else
  #include ".commandLiner.h"

  int USAGE(hallo)
  {
    Synopsis("[*names] [-b|--b-<byephrase>] [--location]");
    return CheckForError()
        || !hasOption('h');
  }
#endif

// regular includes here:
#include <stdlib.h>

//... ...

#ifdef hallo
int main(int argc,char**argv)
{
    if( !InitCommandLiner(argc,argv)
     || !rawNum() ) {
    /*
    // This implements an 'advanced' hello world example, which
    // shows up usage of commandLiner.h's parameter handling:
    //
    // The user can define additional individuals to let them greet:
    //    - by passing 'raw' parameters
    //
    // It optionally can say 'bye bye' by either a dafault phrase: 
    //    - by passing switch: '-b'
    //
    // or: by some explicite, userdefined phrase:
    //    - by passing option '--b-<byePhrase>'
    //
    // It can be called in a 'modus' where it's telling about it's actual location:
    //    - by passing modus '--location'
    */

        // if no individuums are defined by the command line, add
        // a default individuum to be greeted:
        setOption('w',"World");
    }

    // show help screen if 'modus' --help is given
    if( isModus("help") )
        exit( USAGE( hallo ) );

    // output any recognized userinputs when -v (verbose) is passed
    if( hasOption('v') )
        showOptions();

    // look if any idividuums are defined to be greeted by the command:
    cmLn individuum; // but lets filter out any options and switches...
    while( isValidArg(individuum = nextRaw()) ) //...these don't need any greeting.
        printf("hallo %s\n",individuum);

    // then greet the default 'World' individuum if we've defined it before:
    if( hasOption('w') )
        printf("Hallo %s\n",getName('w'));

    // check if maybe a modus 'location' is given. if so, aditionally
    // let tell about that actual location where we currently are
    if( isModus("location") )
        printf("I am Here!\n");

    // and say 'bye' if '-b' switch maybe is enabled...
    if(hasOption('b')) // by either the user given a bye phrase,
        printf("...%s\n", isSwitch('b') // or if non given, by the default...
                      ? HALLOWORLD_BYEPHRASE : getName('b') );
        /* were 'default' is a preproc evaluated expression macro declared in
           the (by '.hallo.h') included feature header, which may be changed
           permanently by passing an additional '--+' environMentor parameter:
              hallo.c --+HALLOWORLD_BYEPHRASE "<new default goodbye phrase>"
        */

    // and retun some greetings to the system if 'b' is either switch or option
    exit( hasOption('b') );
}

#endif
