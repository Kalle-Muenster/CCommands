/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      persist.cc                                 ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include ".persist.h"
#endif

// regular includes here:
// #include <tcclib.h>
#include ".commandLiner.h"
#include ".stringPool.h"

const char* DOKU(persist)="\
   --enable <command> : enables a command local namespace for <command>\n\
                        (where <command>.c must include .environMentor.h)\n\n\
    -s   :    [MacroValue] will be set as string.\n\
    -i   :    [MacroValue] will be set as number.\n\
    -c   :    [MacroValue] will be set as c-code.\n\
    -d   :    delete macro [MacroName].\n\
    -n   :    <command[,command2[,...]]> list local macros for given commands\n\
    -e   :    exports all global persisted macros to environment variables.\n\
    -h   :    displays this help message.\n\n\n";
UsageDefinition(persist,"<MacroName> [option] [MacroValue]")


int persistFunction(void)
{
    DEBUGLOG("Enter")

    char Mode = '\0';
    REFLECTOR currentScope = GLOBAL;

    if( hasOption('h') ) {
		printf( "option 'h' is: %s\n", getName('h') );
        exit(USAGE(persist));
    }
    if( hasOption('v') )
        currentScope=ANY;

    if(numGum()&&(!hasOption('i'))&&(!hasOption('c')&&(!hasOption('d'))&&(!hasOption('s'))&&(!hasOption('e'))))
        setSwitch('s');
	
	if( hasOption('e') ) {
		if( isModus("enable") ) {
			cmLn command = rawNext('e');
			DEBUGFMT("will ENABLE features for command: %s", command );
			enableFeaturesForCommand( command );
			printf("\n%s now supports Commando-features !", command );
		} else {
			printf("\nExporting all macros to Environment!\n");
			exportAll();
			printf("\nprevious Environment restored!\n");
		} exit(CheckForError());
	}
    if(numGum()) {
        if(search('n')) {
            DEBUGFMT( "Adding Feature to %s!", getName('n') );
            pool_setf("%s ",getName('n'));
            pool_setf("%s ",rawName(1));
            if(search('i')) {
                pool_setfi("(%i)",atoi(getName('i')));
            } else if(search('s')) {
                pool_setf( "\"%s\"", getName('s'));
            }
            char* dbgstring = pool_merge(3);
            printf("\nSet feature %s\n", dbgstring);
            setLocalMacro(dbgstring);
            return CheckForError();
        } if(rawNum()) {
            setOption('m',rawName(1));
            if(hasOption('s')) {
                if(isSwitch('s'))
                    switchIdx('s',indexOfName(rawName(2)));
                setMacroStr( getName('m'), getName('s') );
                Mode='s';
            }
        else
            if(hasOption('i')) {
                if(isSwitch('i'))
                    switchIdx('i',indexOfName(rawName(2)));
                setMacroNum( getName('m'), atoi(getName('i')) );
                Mode='i';
            }
        else
            if(hasOption('c')) {
                if(isSwitch('c'))
                    switchIdx('c',indexOfName(rawName(2)));
                setMacroRaw( getName('m'), getName('c') );
                Mode='c';
            }
        else
            if(hasOption('d')) {
                if(!isSwitch('d'))
                    setOption('m',getName('d'));
                removeMacro(getName('m'));
                printf("\nRemoved macro: %s",
                        getName('m') );
                exit(CheckForError());
            }
        printf("\nAdd macro: %s %s",
                getName('m'), getName(Mode) );
        } // if not (numgum-optnum)
        else
            if(hasOption('n')) {
                if(search('n'))
                    system(pool_setf("cLine.c --f-%s --verbose",getName('n')));
            }
    } else 
		showPersistente( currentScope );
    return CheckForError();
}

int persist_commandCall(const char* cmdline)
{
    DEBUGFMT("will call: %s",cmdline);
    PushCommandLine(cmdline);
    persistFunction();
    return PopCommandLine(true);
}

#ifdef persist
int main(int argc,char**argv)
{
    InitCommandLiner(argc,argv);
    int retval = persistFunction();
    exit(retval);
}//eof
#endif