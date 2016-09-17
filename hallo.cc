/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      hallo.cc                                   ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerater v0.0                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_ 
#include "hallo.h"
#endif 

// regular includes here: 
#include <tcclib.h>
#include "commandLiner.h"

//... ...                                               

int main(int argc,char**argv)
{
	// parse the start command and recognize any given parameters 
	pargumnz(argc,argv);
	
	// an 'advanced' hello world example, 
	// it shows usage of commandLiner.h's parameter handling:
	// the user can define additional individuals to let them greet:
	//
	//    - by passing raw parameters
    // and it optionally says 'bye bye' also:
	//    - by passing switch '-b'  
	//
	// and to choose the phrase it will use for saying bye:
	//    - by passing option '--b-<byePhrase>'
	
	
	// add a default individuum to greet if no others where passed
    setOption('w',"World");

	// output any recognized userinputs when -v (verbose) was passed also
    if(hasOption('v'))
        showOptions();

	// look if any additional idividuums to greet where defined:
    if(numGum())// filter out options, like '-b' or '-v' switchs. 
        for(int i=0;i<numGum();i++)// thees don't need greetings.
            if( isModus(getNameByIndex(i))// 
            && !nameWasSwitched(getNameByIndex(i)))
                printf("hallo %s\n",getNameByIndex(i));
				
	// greet the default 'world' individuum
	// we just defined before
    printf("Hallo %s\n",getName('w'));

	// say 'bye' if '-b' switch was enabled
    if(hasOption('b')) // use the user choosen phrase,
        printf("...%s",getName('b')[0] ?// or if none, use the default
                          getName('b') : "bye bye!\n" );

	// retun some greetings to the system if '-b' switch enabled.
	return hasOption('b');
	
}//eof
