const byte HalloWorldSnippet[]={ "\n \
    || !rawNum() ) {\n\
    /*\n\n\
    // This implements an 'advanced' hello world example, which\n\
    // shows up usage of commandLiner.h's parameter handling:\n\
    //\n\
    // The user can define additional individuals to let them greet:\n\
    //    - by passing 'raw' parameters\n\
    //\n\
    // It optionally can say 'bye bye' by either a dafault phrase:\n\
    //    - by passing switch: '-b'\n\
    //\n\
    // or: by some explicite, userdefined phrase:\n\
    //    - by passing option '--b-<byePhrase>'\n\
    //\n\
    // It can be called in a 'modus' where it's telling about it's actual location:\n\
    //    - by passing modus '--location'\n\
    */\n\n\
        // if no individuums are defined by the command line, add\n\
        // a default individuum to be greeted:\n\
        setOption('w',\"World\");\n\
    }\n\n\
    // show help screen if 'modus' --help is given\n\
    if( isModus(\"help\") )\n\
        exit( USAGE( ^COMMANDNAME^ ) );\n\n\
    // output any recognized userinputs when -v (verbose) is passed\n\
    if( hasOption('v') )\n\
        showOptions();\n\n\
    // look if any idividuums are defined to be greeted by the command:\n\
    cmLn individuum; // but lets filter out any options and switches...\n\
    while( isValidArg(individuum = nextRaw()) ) //which need no greeting.\n\
        printf(\"hallo %s\\n\",individuum);\n\n\
    // then greet the default 'World' individuum if we've defined it before:\n\
    if( hasOption('w') )\n\
        printf(\"Hallo %s\\n\",getName('w'));\n\n\
    // check if maybe a modus 'location' is given. if so, aditionally\n\
    // let tell about that actual location where we currently are\n\
    if( isModus(\"location\") )\n\
        printf(\"I am Here!\\n\");\n\n\
    // and say 'bye' if '-b' switch maybe is enabled...\n\
    if( hasOption('b') ) // by either the user given a bye phrase,\n\
        printf(\"...%s\\n\", isSwitch('b') // or if non given, by the default...\n\
                      ? HALLOWORLD_BYEPHRASE : getName('b') );\n\
        /* were 'default' is a preproc evaluated expression macro declared in\n\
           the (by '.^COMMANDNAME^.h') included feature header, which may be changed\n\
           permanently by passing an additional '--+' environMentor parameter:\n\
             ^COMMANDNAME^.c --+HALLOWORLD_BYEPHRASE \"<new default goodbye phrase>\" */\n\n\
    // and retun some greetings to the system if 'b' is either switch or option\n\
    exit( hasOption('b') );\n\
" };

const byte HalloWorldIncludes[]={"\
#include <.environMentor.h>\n\n\
#ifndef HALLOWORLD_BYEPHRASE\n\
#define HALLOWORLD_BYEPHRASE \"bye bye...\"\n\
#endif\n\
" };
